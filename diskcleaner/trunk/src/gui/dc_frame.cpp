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
#include <list>
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

#include <wx/log.h>
#include <wx/fileconf.h>
#include <wx/intl.h>
#include <wx/textdlg.h>

using namespace diskcleaner;

dc_frame::dc_frame( wxWindow* parent ):dc_base_frame( parent )
{
    wxImage::AddHandler( new wxJPEGHandler );

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

    boost::shared_ptr<PlugInfo> pinfo;
    dcApp& app = wxGetApp();

    result_frame rsframe( this );

    if ( settings.global.delete_locked && !app.IsUserAdmin() )
    {
        wxLogWarning( _("Warning: setting 'Delete locked files on reboot' ignored. "
                        "The required Administrator priviliges are missing.") );

    }


    __int64 total_bytes = 0, total_files = 0;

    ResetFilesScheduledRemoveOnReboot();

    //wchar_t files[255] = { 0 };

    for (int i = 0, num_items = plugin_checkbox->GetCount() ; i < num_items ; ++i )
    {

        if ( plugin_checkbox->IsChecked( i ) )
        {
            pinfo = plugin_list[i];
            wxLogVerbose( L"Cleaning: %s", plugin_list[i]->GetShortDesc().c_str() );
            pinfo->Clean();

            total_files+=pinfo->GetItemsCleaned();
            total_bytes+=pinfo->GetBytesCleaned();

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

    if ( !settings.global.show_description )
    {
        description_label->Show( false );
        description_box->Show( false );
    }
    //
    //Load text plugins
    //

    std::list<std::wstring> sortlist;

    boost::shared_ptr<PlugInfo> tempfiles (new system_temp( settings.systemp ) );
    add_plugin_to_lists( tempfiles );

    boost::shared_ptr<PlugInfo> rbin (new RecycleBinInfo() );
    add_plugin_to_lists( rbin );

    boost::shared_ptr<PlugInfo> recentdocs (new recent_docs() );
    add_plugin_to_lists( recentdocs );

    boost::shared_ptr<PlugInfo> ffcache (new firefox_cache() );
    add_plugin_to_lists( ffcache );

    boost::shared_ptr<PlugInfo> ffcookies (new firefox_cookies() );
    add_plugin_to_lists( ffcookies );

    boost::shared_ptr<PlugInfo> ffhistory (new firefox_history() );
    add_plugin_to_lists( ffhistory );

    WIN32_FIND_DATA finddata;
    HANDLE fHandle = FindFirstFile((app.GetAppDirectory() + L"\\plug-ins\\" + L"*.dct").c_str(),&finddata);

    if (fHandle != INVALID_HANDLE_VALUE)
    {
        do
        {

            ::wxLogDebug( L"%hs: found text plugin: %s", __FUNCTION__, finddata.cFileName );
            sortlist.push_front(std::wstring(finddata.cFileName));
        }
        while  (FindNextFile(fHandle,&finddata));
    }
    FindClose(fHandle);

    if (sortlist.size() > 0)
    {

        sortlist.sort();
        std::list<std::wstring>::const_iterator it = sortlist.begin();

        std::auto_ptr<wait_dlg> waitdlg( new wait_dlg(this) );
        waitdlg->SetProgressRange( sortlist.size() );
        waitdlg->Show( true );

        while ( it!=sortlist.end() )
        {
            std::wstring FullPath =  app.GetAppDirectory() + L"\\plug-ins\\"  + *it;
            ::wxLogDebug( FullPath.c_str() );

            boost::shared_ptr<PlugInfo> pi (new TextPlugInfo( FullPath ) );
            ::wxLogDebug( L"Instantiated!" );

            waitdlg->Increment();

            add_plugin_to_lists( pi );  //auto_ptr takes care of our unused TextPlugInfo objects
            ++it;
        }

    }

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
                       (new diskcleaner::dcpreset_handler( wxConfigBase::Get( false ), plugin_checkbox, plugin_list ) );

    ppreset_handler->load_last_used();

    wxArrayString preset_list;
    preset_list.Empty();

    ppreset_handler->get_saved_preset_names( preset_list );

    preset_box->Append( preset_list );

    set_items_selected_text();

}

void dc_frame::plugin_checkbox_itemselected( wxCommandEvent& event )
{
    int item = event.GetSelection();
    bool item_selected =  plugin_checkbox->IsSelected(item);

    if (!item_selected) return;

    boost::shared_ptr<PlugInfo> pi = plugin_list[item];

    description_box->ChangeValue( pi->GetLongDesc() );
}

void dc_frame::plugin_checkbox_toggled( wxCommandEvent& event )
{
    set_items_selected_text();
}

void dc_frame::set_items_selected_text()
{
    __int64 numbytes_checked = 0, numitems_checked = 0;
    __int64 totalbytes = 0, totalitems = 0;

    for (int n = 0, num_items = plugin_checkbox->GetCount(); n < num_items; ++n )
    {
        __int64 numfound = plugin_list[n]->GetItemsFound();
        __int64 bytesfound = plugin_list[n]->GetBytesFound();
        totalbytes += bytesfound;
        totalitems += numfound;

        if (plugin_checkbox->IsChecked( n ) )
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

void dc_frame::add_plugin_to_lists(boost::shared_ptr<PlugInfo>& pi)
{
    pi->Scan();
    if (pi->GetItemsFound() > 0 || settings.global.hide_empty == false )
    {
        ::wxLogDebug( L"Adding: %s" , SetItemText( pi.get() ).c_str() );

        int index = plugin_checkbox->Append(SetItemText( pi.get() ) );

        plugin_checkbox->Check(index, true);

        plugin_list.push_back( pi );
        //IniFile->ReadBool(SettingName,String(pi->GetShortDesc()),false);
    }
}

void dc_frame::dc_base_frame_onclose( wxCloseEvent& event )
{
    ppreset_handler->save_last_used();
    Destroy();
}

