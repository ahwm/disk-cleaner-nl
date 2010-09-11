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

#include "core/text_plugins/TextPlugInfo.h"
#include "core/built_in_plugins/recycle bin/rbin.h"
#include "core/built_in_plugins/firefox/firefox.h"
#include "core/built_in_plugins/system temp/systemp.h"
#include "core/built_in_plugins/recent docs/recentdocs.h"
#include "core/built_in_plugins/internet_explorer/internet_explorer.h"


#include "gui/dc_frame.h"
#include "processes_dlg.h"
#include "gui/about_frame.h"
#include "gui/wait_dlg.h"
#include "gui/prefs_dlg.h"
#include "gui/result_frame.h"
#include "gui/dcApp.h"
#include "gui/wxCheckedListCtrl.h"
#include "gui/wxListCtrlLog.h"

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

dc_frame::dc_frame( wxWindow* parent, diskcleaner::dcsettings& _settings ):
    dc_base_frame( parent ), settings(_settings)
{
    // Let the 'Select All/None/Invert' menu also show when the plugin list is right-clicked
    plugin_listctrl->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dc_base_frame::dc_base_frameOnContextMenu ), NULL, this );

    // Empty ListCtrl and add columns
    plugin_listctrl->ClearAll();
    plugin_listctrl->InsertColumn( 0, _( "Title" ) );
    plugin_listctrl->InsertColumn( 1, _( "Items" ), wxLIST_FORMAT_RIGHT );
    plugin_listctrl->InsertColumn( 2, _( "Size" ) , wxLIST_FORMAT_RIGHT );
    plugin_listctrl->InsertColumn( 3, _( "Description" ) );

    clean_btn->SetFocus();

    plugin_listctrl_hwnd = static_cast<HWND>( plugin_listctrl->GetHandle() );
    plugin_listctrl->SetBackgroundImage( L"background_icon" );

    // settings should be saved before exit
    settings_already_saved = false;

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

    //Only append the item if it's not already there (case insensitive compare,
    //ppreset_handler saves presets without regarding case).
    if ( preset_box->FindString( te.GetValue(), false ) == wxNOT_FOUND )
    {
        preset_box->Append( te.GetValue() );
    }


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

    // Will return wxID_OK if a restart is necessary, wxID_CANCEL if
    // *we* don't need to do anything.
    if( pd.ShowModal() == wxID_OK )
    {
        // Don't try to elevate our priviliges
        // If admin, then the new process will have admin rigthts too
        // If not, then the new process neither.
        run_diskcleaner( false );

        Close();
    }

    // Make sure that even when a normal user sets
    // 'delete files on reboot' to true, RemoveOnReboot is set to false
    dcApp& app = wxGetApp();
    if ( settings.global.delete_locked)
    {
        SetRemoveOnReboot( app.IsUserAdmin() );
    }
    else
    {
        SetRemoveOnReboot (false );
    }
}

// By definition (*click*), we're in GUI mode
void dc_frame::clean_btn_click( wxCommandEvent& event )
{

    __int64 total_bytes = 0, total_files = 0;

    //sets the file count of scheduled files to zero
    ResetFilesScheduledRemoveOnReboot();

    // Set AllFilesRemoved flag to true;
    SetAllFilesRemoved( true );

    rsframe.reset(new result_frame( this ) );

    if ( settings.ui.result_frame_size.topx && settings.ui.result_frame_size.topy &&
            settings.ui.result_frame_size.width && settings.ui.result_frame_size.height )
    {
        rsframe->SetSize( settings.ui.result_frame_size.topx, settings.ui.result_frame_size.topy,
                          settings.ui.result_frame_size.width, settings.ui.result_frame_size.height );
    }

    rsframe->set_progress_range( plugin_listctrl->GetItemCount() );

    // Hide the main window
    // Unfortunately also hides the taskbar button on Vista
    Hide();

    rsframe->disable_controls();
    rsframe->Show();

    //Set cursor to 'Hourglass'
    wxBeginBusyCursor();

    // Do the actual cleaning here
    clean(total_files, total_bytes);

    wxEndBusyCursor();

    if ( !GetAllFilesRemoved() )
    {
        wxLogMessage( _( "Could not remove all files. Please close all open applications and retry." ) );
    }

    wxString schedulestr;
    schedulestr.Printf( wxPLURAL( "Scheduled %I64d file for removal on reboot.",
                                  "Scheduled %I64d files for removal on reboot." , GetFilesScheduledRemoveOnReboot() ),
                        GetFilesScheduledRemoveOnReboot() );

    wxLogMessage(  schedulestr );


    schedulestr.Printf( wxPLURAL( "Cleaned total of %s in %I64d item",
                                  "Cleaned total of %s in %I64d items" , total_files ),
                        bytes_to_string( total_bytes ).c_str(), total_files );
    wxLogMessage(  schedulestr );

    rsframe->enable_controls();

}

void dc_frame::clean(__int64& total_files, __int64& total_bytes)
{
    PlugInfo* pinfo;

    // Iterate over all cleaning plug-ins, call Clean() function if checked
    for (int i = 0, num_items = plugin_listctrl->GetItemCount() ; i < num_items ; ++i )
    {

        // Hack! Needs to be better separated from GUI stuff
        // This function was created to ge rid of the IsQuietMode calls
        // all over the place. :/
        if ( rsframe.get() )
        {
            rsframe->Increment();
        }

        if ( plugin_listctrl->IsChecked( i ) )
        {
            pinfo = (PlugInfo* ) plugin_listctrl->GetItemData( i );
            wxLogMessage( L"Cleaning: %s", pinfo->GetShortDesc().c_str() );
            wxLog::GetActiveTarget()->Flush();  //Immdediately show what we're doing

            pinfo->Clean();

            total_files += pinfo->GetItemsCleaned();
            total_bytes += pinfo->GetBytesCleaned();

            // Produce result summary line
            wxString line;
            line.Printf(
                        wxPLURAL( "%s: Cleaned %s in %I64d item.",
                                  "%s: Cleaned %s in %I64d items.",
                                  pinfo->GetItemsCleaned() ),
                            pinfo->GetShortDesc().c_str(),
                            bytes_to_string(pinfo->GetBytesCleaned() ).c_str(),
                            pinfo->GetItemsCleaned() );
            wxLogMessage( line.c_str() );
            wxLogMessage( L"" ); //Skip a line for readability


        }
    }
}

void dc_frame::runasadmin_btn_click( wxCommandEvent& event )
{
    run_diskcleaner( true );

    //Close this instance
    Close();
}

void dc_frame::run_diskcleaner( bool as_admin )
{
    // First save the UI settings
    // done here to prevent clash with child process
    // sets settings_already_saved to true
    save_settings();

    // Create child process
    SHELLEXECUTEINFO   sei;
    ZeroMemory ( &sei, sizeof(sei) );

    dcApp& theApp = wxGetApp();

    wchar_t szAppPath[MAX_PATH] = {0};

    GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1);

    sei.cbSize          = sizeof(SHELLEXECUTEINFOW);
    sei.hwnd            = (HWND) this->GetHandle();
    sei.fMask           = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
    sei.lpVerb          = (as_admin)? _TEXT("runas"): _TEXT("open");
    sei.lpFile          = szAppPath;
    sei.lpParameters    = ( theApp.IsPortable() ) ? L"-p" : NULL;
    sei.nShow           = SW_SHOWNORMAL;

    if ( ! ShellExecuteEx ( &sei ) )
    {
        ::wxLogError( L"Error: ShellExecuteEx failed 0x%x\n", GetLastError() );

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
    //Dynamically link with SHGetKnownfolderPath
    InitializeSHGetKnownFolderPath();

    dcApp& app = wxGetApp();
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

    //Show progress of scan (wait_dlg)

    wait_dlg waitdlg( this );

    //Get the number of text plugins as input for the progress bar

    std::vector<std::wstring> plugin_list;

    if ( !app.NoTextPlugins() )
    {
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
    }

    //
    //Initialize build-in plugins
    //

    waitdlg.SetProgressRange( plugin_list.size() + ( (app.NoBuiltInPlugins() ) ? 0 : 9 ) );

    if ( !app.IsQuietMode() )
    {
        waitdlg.Show( true );
    }


    if ( !app.NoBuiltInPlugins() )
    {
        wxLogDebug( L"%hs: adding build-in plugins", __FUNCTION__ );

        PlugInfo* tempfiles  = new system_temp( settings.systemp );
        add_plugin_to_listctrl( tempfiles );
        waitdlg.Increment();

        PlugInfo* rbin = (new RecycleBinInfo() );
        add_plugin_to_listctrl( rbin );
        waitdlg.Increment();

        PlugInfo* recentdocs = (new recent_docs() );
        add_plugin_to_listctrl( recentdocs );
        waitdlg.Increment();

        PlugInfo* ffcache = (new firefox_cache() );
        add_plugin_to_listctrl( ffcache );
        waitdlg.Increment();

        PlugInfo* ffcookies = (new firefox_cookies() );
        add_plugin_to_listctrl( ffcookies );
        waitdlg.Increment();

        PlugInfo* ffhistory = (new firefox_history() );
        add_plugin_to_listctrl( ffhistory );
        waitdlg.Increment();

        PlugInfo* iecache = (new ie_cache( settings.tempinternetfiles.delete_offline ) );
        add_plugin_to_listctrl( iecache );
        waitdlg.Increment();

        PlugInfo* iehistory = (new ie_history() );
        add_plugin_to_listctrl( iehistory );
        waitdlg.Increment();

        PlugInfo* iecookies = (new ie_cookies() );
        add_plugin_to_listctrl( iecookies );
        waitdlg.Increment();
        waitdlg.Update();
    }

    //
    // Load text plug-ins
    //

    if ( !app.NoTextPlugins() )
    {
        if (plugin_list.size() > 0)
        {

            std::vector<std::wstring>::const_iterator it = plugin_list.begin();
            int counter = 0;
            while ( it!=plugin_list.end() )
            {
                std::wstring FullPath =  app.GetAppDirectory() + L"\\plug-ins\\"  + *it;
                ::wxLogDebug( FullPath.c_str() );

                PlugInfo*  pi = (new TextPlugInfo( FullPath ) );
                ::wxLogDebug( L"%s instantiated", it->c_str() );

                waitdlg.Increment();

                if ( counter > 9 )
                {
                    waitdlg.Update();
                    counter = 0;
                }
                add_plugin_to_listctrl( pi );
                ++it;
                ++counter;

            }

        }
    }

    // Check for running applications
    if ( !app.IsQuietMode() && settings.global.show_running_processes )
    {
        processes_dlg pd( NULL );

        // Iterate over all cleaning plug-ins, call Clean() function if checked
        for (int i = 0, num_items = plugin_listctrl->GetItemCount() ; i < num_items ; ++i )
        {

            PlugInfo* pinfo;
            std:: wstring process, processname;

            pinfo = (PlugInfo* ) plugin_listctrl->GetItemData( i );

            if( pinfo->RunningProcessCheck( process, processname ) )
            {
                pd.add_process_to_check( process, processname );
            }

        }
        waitdlg.Show( false );
        settings.global.show_running_processes = pd.do_process_check();
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


    //Load (specified or last used) preset
    preset_box->Clear();
    preset_box->Append( _( "<last used>" ) );

    ppreset_handler  = std::auto_ptr<diskcleaner::dcpreset_handler>
                       (new diskcleaner::dcpreset_handler( wxConfigBase::Get( false ), plugin_listctrl ) );

    //Get saved presets
    wxArrayString preset_list;
    preset_list.Empty();

    ppreset_handler->get_saved_preset_names( preset_list );

    //Add the saved preset list to the preset combo box
    preset_box->Append( preset_list );

    if ( app.GetPresetToBeRecalled() == L"" )
    {
        ppreset_handler->load_last_used();

        //Selects <last used> in the combo box
        preset_box->SetSelection( 0 );
    }
    else
    {
        ppreset_handler->load_preset( app.GetPresetToBeRecalled() );
        preset_box->SetSelection( preset_box->FindString( app.GetPresetToBeRecalled(), false ) );
    }

}

wxString dc_frame::bytes_to_string( __int64 bytes )
{
    wxString bytes_string;

    if ( bytes < 1024 )
    {
        bytes_string.Printf( _( "%I64d b" ), bytes );
    }
    else
    {
        bytes /= 1024;
        if ( bytes < 1024 )
        {
            bytes_string.Printf( _( "%I64d kB" ), bytes );
        }
        else
        {
            bytes /= 1024;
            if ( bytes < 1024 )
            {
                bytes_string.Printf( _( "%I64d MB" ), bytes );
            }
            else
            {
                bytes_string.Printf( _( "%5.2f GB" ), bytes/1024.0 );
            }
        }
    }

    return bytes_string;
}

// Takes a pointer to a PlugInfo class instance & lets it Scan().
// Depending on user preferences, the plugin is then added to the
// plugin_listctrl for the user to see (see below for exact circumstances)
void dc_frame::add_plugin_to_listctrl( diskcleaner::PlugInfo* pi)
{
    dcApp& app = wxGetApp();
    bool is_admin = app.IsUserAdmin();

    wxLogDebug( L"%hs: processing %s" , __FUNCTION__, pi->GetShortDesc().c_str() );
    pi->Scan();

    // Don't show the plug-in if:
    // a. We need admin priviliges and we're not admin
    // b. We don't have anything to show and the user wants to hide empty items
    wxLogDebug( L"%hs: We are admin: %s. We need admin: %s", __FUNCTION__, ( is_admin ) ? L"true": L"false",
                ( pi->AdminPriviligesRequired() ) ? L"true": L"false" );

    if ( ( !pi->AdminPriviligesRequired() || is_admin ) && (pi->GetItemsFound() > 0 || settings.global.hide_empty == false ) )
    {
        ::wxLogDebug( L"%hs: adding %s" , __FUNCTION__, pi->GetShortDesc().c_str() );

        wxString tmpString, bytes_string;

        int index = plugin_listctrl->GetItemCount();
        //index++;

        plugin_listctrl->InsertItem( index, pi->GetShortDesc() );


        tmpString.Printf( L"%I64d", pi->GetItemsFound() );
        plugin_listctrl->SetItem( index, 1, tmpString );

        bytes_string = bytes_to_string( pi->GetBytesFound() );

        plugin_listctrl->SetItem( index, 2, bytes_string );

        plugin_listctrl->SetItem( index, 3, pi->GetLongDesc() );

        plugin_listctrl->SetItemPtrData( index, (wxUIntPtr) pi );

        //plugin_listctrl->Check( index, true );

    }
    else //no items found and were hiding this item
    {
        wxLogDebug( L"%hs: deleting %s (unused)", __FUNCTION__, pi->GetShortDesc().c_str() );
        delete pi;
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
    // We used to save the settings here first, and then close the app.
    // Unfortunately this clashes with the back and run-as-admin buttons
    // on slow (flash) disks. Before Disk Cleaner has written the data, the new
    // process is created immediately accessing the config file. This causes the parent
    // process to be denied access to the config file, resulting in an error.
    //
    // Workaround: *before* starting the child process, save the settings.
    // Raise a flag that settings are already saved, so don't do it here again.

    if ( !settings_already_saved )
    {
      save_settings();
    }

    //THEN delete all PlugInfo objects
    for ( int k = 0, items = plugin_listctrl->GetItemCount(); k < items; ++k )
    {
        wxLogDebug( L"%hs: deleting %s", __FUNCTION__, plugin_listctrl->GetItemText( k ).c_str() );
        delete (PlugInfo*) plugin_listctrl->GetItemData( k );
        plugin_listctrl->SetItemData( k, 0 );

    }

    //Unload the dynamically loaded shell function SHGetKnownFolderPath()
    //which is Vista+ only.
    wxLogDebug( L"%hs: unloading SHGetKnownFolderPath", __FUNCTION__ );
    UninitializeSHGetKnownFolderPath();

    Destroy();
}

void dc_frame::select_all_click(wxCommandEvent& event )
{
    // Iterate over all cleaning plug-ins
    for (int i = 0, num_items = plugin_listctrl->GetItemCount() ; i < num_items ; ++i )
    {
        plugin_listctrl->Check(i, true );
    }
}

void dc_frame::select_none_click(wxCommandEvent& event )
{
    // Iterate over all cleaning plug-ins
    for (int i = 0, num_items = plugin_listctrl->GetItemCount() ; i < num_items ; ++i )
    {
        plugin_listctrl->Check(i, false );
    }
}

void dc_frame::invert_selection_click(wxCommandEvent& event )
{
    // Iterate over all cleaning plug-ins
    for (int i = 0, num_items = plugin_listctrl->GetItemCount() ; i < num_items ; ++i )
    {
        plugin_listctrl->Check(i, !plugin_listctrl->IsChecked( i ) );
    }
}

// Forward mouse wheel messages to the list control
// in order to let it scroll even when unfocused.
void dc_frame::handle_mousewheel( wxMouseEvent& event )
{
      // Let the win32 list control handle the actual scrolling
      // We don't need to fuzz with event.m_wheelDelta etc.
      SendMessage(plugin_listctrl_hwnd , WM_MOUSEWHEEL, MAKEWPARAM( 0 , event.m_wheelRotation ), MAKELPARAM( event.GetX(), event.GetY() ) );

}


void dc_frame::result_frame_finished_signal( bool restart )
{

    // rsframe must be a valid pointer to a result_frame instance
    // Therefore, this function should only be called from a result_frame instance
    rsframe->GetPosition( &settings.ui.result_frame_size.topx, &settings.ui.result_frame_size.topy );
    rsframe->GetSize( &settings.ui.result_frame_size.width, &settings.ui.result_frame_size.height );

    if ( restart )
    {
        //Run Disk Cleaner without explicit admin rights
        //If this process already has admin rights,
        //then so will the new process have.
        run_diskcleaner( false );
    }

    // We always end here.
    Close();
}

void dc_frame::save_settings()
{
    dcApp& app = wxGetApp();

    //Only bother with saving sizes, checked and unchecked items
    // and positions if we were run in interactive mode.
    if ( !app.IsQuietMode() )
    {

        //Save the size of the main window plus the column widths
        wxLogDebug( L"%hs: saving main window sizes", __FUNCTION__ );
        GetSize( &settings.ui.dc_frame_size.width, &settings.ui.dc_frame_size.height );
        GetPosition( &settings.ui.dc_frame_size.topx, &settings.ui.dc_frame_size.topy );
        settings.ui.col_sdesc_width = plugin_listctrl->GetColumnWidth( 0 );
        settings.ui.col_item_width = plugin_listctrl->GetColumnWidth( 1 );
        settings.ui.col_size_width = plugin_listctrl->GetColumnWidth( 2 );
        settings.ui.col_ldesc_width = plugin_listctrl->GetColumnWidth( 3 );

        settings.Save();

        //FIRST save the currently checked and unchecked items
        wxLogDebug( L"%hs: saving currently checked items", __FUNCTION__ );
        ppreset_handler->save_last_used();
    }
    else
    {
        wxLogDebug( L"%hs: skipping saving of presets and sizes, quiet mode active", __FUNCTION__ );
    }

    settings_already_saved = true;
}
