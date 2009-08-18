/*
** Copyright (C) 2009 Robert Moerland
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation version 2.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include <string>
#include <vector>
#include <memory>

#include "core/text_plugins/TextPlugInfo.h"
#include "core/build_in_plugins/recycle bin/rbin.h"
#include "core/build_in_plugins/firefox/firefox.h"
#include "core/build_in_plugins/system temp/systemp.h"
#include "core/build_in_plugins/recent docs/recentdocs.h"


#include "gui/dc_frame.h"
#include "gui/about_frame.h"
#include "gui/wait_dlg.h"
#include "gui/prefs_dlg.h"
#include "gui/result_frame.h"
#include "gui/dcApp.h"
#include "gui/wxCheckedListCtrl.h"

#include <wx/log.h>
#include <wx/fileconf.h>
#include <wx/intl.h>
#include <wx/textdlg.h>

using namespace diskcleaner;

int wxCALLBACK listctrl_compare( long item1, long item2, long sortData )
{
    dcsettings* settings = ( dcsettings* ) sortData;
    PlugInfo* pi1 = ( PlugInfo* ) item1;
    PlugInfo* pi2 = ( PlugInfo* ) item2;

    int sortorder = 1;
    if ( settings->ui.sortorder ) //1 = desc
    {
        sortorder = -1;
    }

    switch ( settings->ui.sort_column )
    {
    case 0:
    {
        if (pi1->GetShortDesc() < pi2->GetShortDesc() )
            return -1*sortorder;
        if (pi1->GetShortDesc() > pi2->GetShortDesc() )
            return 1*sortorder;
        return 0;
    }
    case 1:
    {
        if (pi1->GetItemsFound() < pi2->GetItemsFound())
            return -1*sortorder;
        if (pi1->GetItemsFound() > pi2->GetItemsFound() )
            return 1*sortorder;
        return 0;
    }
    case 2:
    {
        if (pi1->GetBytesFound() < pi2->GetBytesFound())
            return -1*sortorder;
        if (pi1->GetBytesFound() > pi2->GetBytesFound() )
            return 1*sortorder;
        return 0;
    }
    case 3:
    {
        if (pi1->GetLongDesc() < pi2->GetLongDesc() )
            return -1*sortorder;
        if (pi1->GetLongDesc() > pi2->GetLongDesc() )
            return 1*sortorder;
        return 0;
    }
    default:
        return 0;
    }


}

dc_frame::dc_frame( wxWindow* parent ):dc_base_frame( parent )
{
    wxImage::AddHandler( new wxJPEGHandler );

    plugin_listctrl->ClearAll();
    plugin_listctrl->InsertColumn( 0, _( "Title" ) );
    plugin_listctrl->InsertColumn( 1, _( "Items" ), wxLIST_FORMAT_RIGHT );
    plugin_listctrl->InsertColumn( 2, _( "Size" ) , wxLIST_FORMAT_RIGHT );
    plugin_listctrl->InsertColumn( 3, _( "Description" ) );

}

void dc_frame::preset_save_btn_click( wxCommandEvent& event )
{
    wxTextEntryDialog te( this, _( "Please enter a name for the preset" ), L"Disk Cleaner" );

    te.SetValue( L"" );

    int result = te.ShowModal();

    if ( result == wxID_CANCEL || te.GetValue() == L"" ) //Canceled or nothing entered
    {
        return;
    }

    ppreset_handler->save_preset( te.GetValue().c_str() );

    preset_box->Append( te.GetValue() );


}

void dc_frame::preset_delete_btn_click( wxCommandEvent& event )
{
    if ( preset_box->GetSelection() != 0 ) //0 == <last used>
    {
        ppreset_handler->delete_preset( preset_box->GetStringSelection().c_str() );
        preset_box->Delete( preset_box->GetSelection() );

        preset_box->SetSelection( 0 );
    }
}

void dc_frame::preset_box_onchoice( wxCommandEvent& event )
{
    if ( preset_box->GetSelection() == 0 )
    {
        ppreset_handler->load_last_used();

    }
    else
    {
        ppreset_handler->load_preset( preset_box->GetStringSelection().c_str() );
    }
}



void dc_frame::config_btn_click( wxCommandEvent& event )
{
    prefs_dlg pd( this, settings );

    pd.ShowModal();
    if ( settings.global.delete_locked)
    {
        SetRemoveOnReboot( true );
    }
    else
    {
        SetRemoveOnReboot (false );
    }

}

void dc_frame::clean_btn_click( wxCommandEvent& event )
{

    PlugInfo* pinfo;
    dcApp& app = wxGetApp();

    result_frame rsframe( this );

    if ( settings.ui.result_frame_size.topx && settings.ui.result_frame_size.topy &&
                     settings.ui.result_frame_size.width && settings.ui.result_frame_size.height )
    {
        rsframe.SetSize( settings.ui.result_frame_size.topx, settings.ui.result_frame_size.topy,
                        settings.ui.result_frame_size.width, settings.ui.result_frame_size.height );
    }


    if ( settings.global.delete_locked && !app.IsUserAdmin() )
    {
        wxLogWarning( _("Warning: setting 'Delete locked files on reboot' ignored. "
                        "The required Administrator priviliges are missing.") );

    }


    __int64 total_bytes = 0, total_files = 0;

    ResetFilesScheduledRemoveOnReboot();

    //wchar_t files[255] = { 0 };

    for (int i = 0, num_items = plugin_listctrl->GetItemCount() ; i < num_items ; ++i )
    {

        if ( plugin_listctrl->IsChecked( i ) )
        {
            pinfo = (PlugInfo* ) plugin_listctrl->GetItemData( i );
            wxLogVerbose( L"Cleaning: %s", pinfo->GetShortDesc().c_str() );
            pinfo->Clean();

            total_files += pinfo->GetItemsCleaned();
            total_bytes += pinfo->GetBytesCleaned();

            //_i64tow(pinfo->GetItemsCleaned(),files,10);

            wxString line;
            line.Printf( L"%s: Cleaned %s in %I64d %s.", pinfo->GetShortDesc().c_str(),
                         BytesToString(pinfo->GetBytesCleaned() ).c_str(),
                         pinfo->GetItemsCleaned(), wxPLURAL( "item", "items", pinfo->GetItemsCleaned() ) );
            wxLogMessage( line.c_str() );
        }
    }


    wxString schedulestr;
    schedulestr.Printf( _( "Scheduled %I64d %s for removal on reboot." ), GetFilesScheduledRemoveOnReboot(),
                        wxPLURAL( "file", "files", GetFilesScheduledRemoveOnReboot() ) );

    wxLogMessage(  schedulestr );


    schedulestr.Printf( _( "Cleaned total of %s in %I64d %s") , BytesToString( total_bytes ).c_str(),
                        total_files, wxPLURAL( "item", "items", total_files) );
    wxLogMessage(  schedulestr );

    Hide();
    rsframe.ShowModal();

    rsframe.GetPosition( &settings.ui.result_frame_size.topx, &settings.ui.result_frame_size.topy );
    rsframe.GetSize( &settings.ui.result_frame_size.width, &settings.ui.result_frame_size.height );

    if ( rsframe.rerun )
    {
        //Run Disk Cleaner without explicit admin rights
        //If this process already has admin rights,
        //then so will the new process have.
        run_diskcleaner( false );
    }

    wxLogDebug( L"Ending application");
    Close();
}

void dc_frame::runasadmin_btn_click( wxCommandEvent& event )
{
    run_diskcleaner( true );
}

void dc_frame::run_diskcleaner( bool as_admin )
{
    SHELLEXECUTEINFO   sei;
    ZeroMemory ( &sei, sizeof(sei) );

    wchar_t szAppPath[MAX_PATH] = {0};

    GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1);

    sei.cbSize          = sizeof(SHELLEXECUTEINFOW);
    sei.hwnd            = (HWND) this->GetHandle();
    sei.fMask           = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
    sei.lpVerb          = (as_admin)? _TEXT("runas"): _TEXT("open");
    sei.lpFile          = szAppPath;
    sei.lpParameters    = NULL;
    sei.nShow           = SW_SHOWNORMAL;

    if ( ! ShellExecuteEx ( &sei ) )
    {
        ::wxLogError( L"Error: ShellExecuteEx failed 0x%x\n", GetLastError() );

    }
    else
    {
        Close();
    }
}
void dc_frame::about_btn_click( wxCommandEvent& event )
{
    about_frame af(this);

    af.ShowModal();

}

void dc_frame::exit_btn_click( wxCommandEvent& event )
{
    Close();
}

void dc_frame::init_dialog()
{
    dcApp& app = wxGetApp();
    //Call load method, config class has been set in dcApp::OnCmdLineParsed
    settings.Load();

    if ( settings.global.delete_locked)
    {
        SetRemoveOnReboot( app.IsUserAdmin() );
    }
    else
    {
        SetRemoveOnReboot (false );
    }

    if (settings.ui.dc_frame_size.height && settings.ui.dc_frame_size.topx &&
            settings.ui.dc_frame_size.topy && settings.ui.dc_frame_size.width )
    {
        SetSize( settings.ui.dc_frame_size.topx, settings.ui.dc_frame_size.topy,
                 settings.ui.dc_frame_size.width, settings.ui.dc_frame_size.height );
    }

    if ( settings.ui.col_sdesc_width ) plugin_listctrl->SetColumnWidth( 0, settings.ui.col_sdesc_width );
    if ( settings.ui.col_item_width ) plugin_listctrl->SetColumnWidth( 1, settings.ui.col_item_width );
    if ( settings.ui.col_size_width ) plugin_listctrl->SetColumnWidth( 2, settings.ui.col_size_width );
    if ( settings.ui.col_ldesc_width ) plugin_listctrl->SetColumnWidth( 3, settings.ui.col_ldesc_width );


    //
    //Load text plugins
    //

    std::vector<std::wstring> plugin_list;

    PlugInfo* tempfiles  = new system_temp( settings.systemp );
    add_plugin_to_listctrl( tempfiles );

    PlugInfo* rbin = (new RecycleBinInfo() );
    add_plugin_to_listctrl( rbin );

    PlugInfo* recentdocs = (new recent_docs() );
    add_plugin_to_listctrl( recentdocs );

    PlugInfo* ffcache = (new firefox_cache() );
    add_plugin_to_listctrl( ffcache );

    PlugInfo* ffcookies = (new firefox_cookies() );
    add_plugin_to_listctrl( ffcookies );

    PlugInfo* ffhistory = (new firefox_history() );
    add_plugin_to_listctrl( ffhistory );

    WIN32_FIND_DATA finddata;
    HANDLE fHandle = FindFirstFile((app.GetAppDirectory() + L"\\plug-ins\\" + L"*.dct").c_str(),&finddata);

    if (fHandle != INVALID_HANDLE_VALUE)
    {
        do
        {

            ::wxLogDebug( L"%hs: found text plugin: %s", __FUNCTION__, finddata.cFileName );
            plugin_list.push_back(std::wstring(finddata.cFileName));
        }
        while  (FindNextFile(fHandle,&finddata));
    }
    FindClose(fHandle);

    if (plugin_list.size() > 0)
    {


        std::vector<std::wstring>::const_iterator it = plugin_list.begin();

        std::auto_ptr<wait_dlg> waitdlg( new wait_dlg(this) );
        waitdlg->SetProgressRange( plugin_list.size() );
        waitdlg->Show( true );

        while ( it!=plugin_list.end() )
        {
            std::wstring FullPath =  app.GetAppDirectory() + L"\\plug-ins\\"  + *it;
            ::wxLogDebug( FullPath.c_str() );

            PlugInfo*  pi = (new TextPlugInfo( FullPath ) );
            ::wxLogDebug( L"Instantiated!" );

            waitdlg->Increment();

            add_plugin_to_listctrl( pi );
            ++it;
        }

    }

    //Sort items according to preferences of the user
    plugin_listctrl->SortItems( listctrl_compare, (long) &settings );
    wxListItem item;
    item.SetMask(wxLIST_MASK_IMAGE);
    item.SetImage( ( settings.ui.sortorder )? 3 : 2 );
    plugin_listctrl->SetColumn(settings.ui.sort_column , item);

    //Only support Run As Admin button in Vista or higher
    //But not necessary to show shield when user is admin

    if ( app.IsUserAdmin() ) wxLogDebug(L"Yeah! Admin baby!");

    if ( app.IsUserAdmin() || !app.IsVistaOrHigher() )
    {
        runasadmin_btn->Show( false );
    }
    else
    {
        int width = 0, height = 0;
        runasadmin_btn->GetSize( &width, &height );
        runasadmin_btn->SetSize( width+20, -1 );
        app.Button_SetShield(runasadmin_btn->GetHandle() );
    }



    preset_box->Clear();
    preset_box->Append( _( "<last used>" ) );
    preset_box->SetSelection( 0 );

    ppreset_handler  = boost::shared_ptr<diskcleaner::dcpreset_handler>
                       (new diskcleaner::dcpreset_handler( wxConfigBase::Get( false ), plugin_listctrl ) );

    ppreset_handler->load_last_used();

    wxArrayString preset_list;
    preset_list.Empty();

    ppreset_handler->get_saved_preset_names( preset_list );

    preset_box->Append( preset_list );

    //set_items_selected_text();

}

void dc_frame::plugin_checkbox_toggled( wxCommandEvent& event )
{
    set_items_selected_text();
}

void dc_frame::set_items_selected_text()
{
    __int64 numbytes_checked = 0, numitems_checked = 0;
    __int64 totalbytes = 0, totalitems = 0;

    for (int n = 0, num_items = plugin_listctrl->GetItemCount(); n < num_items; ++n )
    {
        PlugInfo* pi = (PlugInfo* )plugin_listctrl->GetItemData( n );
        __int64 numfound = pi->GetItemsFound();
        __int64 bytesfound = pi->GetBytesFound();
        totalbytes += bytesfound;
        totalitems += numfound;

        if (plugin_listctrl->IsChecked( n ) )
        {
            numbytes_checked += bytesfound;
            numitems_checked += numfound;
        }
    }

    wxString bytes_string;
    if ( totalbytes < 1024 )
    {
        bytes_string = _( "bytes" );
    }
    else
    {
        totalbytes /= 1024;
        numbytes_checked /= 1024;

        if ( totalbytes < 1024 )
        {
            bytes_string = _( "kB" );
        }
        else
        {
            totalbytes /= 1024;
            numbytes_checked /= 1024;
            bytes_string = _( "MB" );
        }
    }

    wxString String;
    String.Printf( _( "Selected %I64d of %I64d items for removal (%I64d/%I64d %s)" ), numitems_checked, totalitems,
                   numbytes_checked, totalbytes, bytes_string.c_str() );
    items_selected_text->SetLabel( String );
}

void dc_frame::add_plugin_to_listctrl( diskcleaner::PlugInfo* pi)
{
    pi->Scan();
    if (pi->GetItemsFound() > 0 || settings.global.hide_empty == false )
    {
        ::wxLogDebug( L"Adding: %s" , SetItemText( pi ).c_str() );

        wxString tmpString, bytes_string;

        int index = plugin_listctrl->GetItemCount();
        //index++;

        plugin_listctrl->InsertItem( index, pi->GetShortDesc() );


        tmpString.Printf( _( "%I64d" ), pi->GetItemsFound() );
        plugin_listctrl->SetItem( index, 1, tmpString );

        int64_t bytes_found  = pi->GetBytesFound();

        if ( bytes_found < 1024 )
        {
            bytes_string = _( "b" );
        }
        else
        {
            bytes_found /= 1024;
            if ( bytes_found < 1024 )
            {
                bytes_string = _( "kB" );
            }
            else
            {
                bytes_found /= 1024;
                if ( bytes_found < 1024 )
                {
                    bytes_string = _( "MB" );
                }
                else
                {
                    bytes_string = _( "GB" );
                }
            }
        }
        tmpString.Printf( _( "%I64d " ), bytes_found );
        plugin_listctrl->SetItem( index, 2, tmpString + bytes_string );

        plugin_listctrl->SetItem( index, 3, pi->GetLongDesc() );

        plugin_listctrl->SetItemPtrData( index, (wxUIntPtr) pi );

        plugin_listctrl->Check( index, true );

    }
}

void dc_frame::plugin_listctrl_column_clicked( wxListEvent& event )
{
    int col = event.GetColumn();

    if ( col == -1 ) return;

    //Remove the current sorting arrow
    wxListItem item;
    item.SetMask(wxLIST_MASK_IMAGE);
    item.SetImage( -1 );
    plugin_listctrl->SetColumn(settings.ui.sort_column , item);

    if ( col == settings.ui.sort_column ) //toggle order
    {
        settings.ui.sortorder = ( settings.ui.sortorder == 0 ) ? 1 : 0;

    }
    else
    {
        settings.ui.sortorder = 0;
    }
    settings.ui.sort_column = col;

    item.SetMask(wxLIST_MASK_IMAGE);
    item.SetImage( ( settings.ui.sortorder == 0 ) ? 2 : 3 );
    plugin_listctrl->SetColumn(settings.ui.sort_column , item);

    plugin_listctrl->SortItems( listctrl_compare, (long) &settings );
}


void dc_frame::dc_base_frame_onclose( wxCloseEvent& event )
{
    GetSize( &settings.ui.dc_frame_size.width, &settings.ui.dc_frame_size.height );
    GetPosition( &settings.ui.dc_frame_size.topx, &settings.ui.dc_frame_size.topy );
    settings.ui.col_sdesc_width = plugin_listctrl->GetColumnWidth( 0 );
    settings.ui.col_item_width = plugin_listctrl->GetColumnWidth( 1 );
    settings.ui.col_size_width = plugin_listctrl->GetColumnWidth( 2 );
    settings.ui.col_ldesc_width = plugin_listctrl->GetColumnWidth( 3 );

    settings.Save();

    ppreset_handler->save_last_used();
    Destroy();
}

