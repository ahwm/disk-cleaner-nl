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

#include <wx/log.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>

#include "prefs_dlg.h"
#include "dcApp.h"
#include "dcpresets.h"
#include "add_location_dlg.h"

#include <windows.h>
#include <shlobj.h>
#include <objidl.h>


const wchar_t* DCLinkName = L"\\Disk Cleaner.lnk";

prefs_dlg::prefs_dlg( wxWindow* parent, diskcleaner::dcsettings& prefs )
    : prefs_dlg_base( parent ), rsettings( prefs )
{

    InitializeGlobal();

    InitializeSystemTemporary();

    InitializeAutostartShortcut();

    InitializeUserLocations();

    // Temporary internet files settings
    tempinet_offline_cb->SetValue( prefs.tempinternetfiles.delete_offline );

//    cookie_filter_cb->SetValue( prefs.cookies.use_cookie_filter );
//   swprintf( age, L"%d", prefs.cookies.min_cookie_age );
//   cookie_age_combo->SetStringSelection( std::wstring( age ) );
//    ok_cancelOK->SetDefault();

    prefsbook->SetSelection( 0 );


}


void prefs_dlg::InitializeGlobal()
{
    dcApp& app = wxGetApp();

    // Set indicators to global settings
    delete_locked_cb->SetValue( rsettings.global.delete_locked );
    hide_empty_cb->SetValue( rsettings.global.hide_empty );
    hide_admin_items_cb->SetValue( rsettings.global.hide_admin );
    warn_open_processes_cb->SetValue( rsettings.global.show_running_processes );


    // Get installed languages and set choice box to current language
    wxArrayString names;
    wxArrayLong identifiers;
    app.GetInstalledLanguages( names, identifiers );
    for (int index = 0, size = identifiers.GetCount(); index < size; ++index )
    {
        LanguageChoice->Append( names[ index ], (void*) (identifiers[ index ] ) ) ;
        if ( identifiers[ index ] == rsettings.global.language_id )
        {
            LanguageChoice->SetSelection( index );
        }
    }

    // Check that the selected global.lang_id is actually selected in the choice box
    // Can fail if the language is removed. Then, select default language.

    if ( LanguageChoice->GetSelection() == wxNOT_FOUND )
    {
        // Bit of a hack. Assumes Default language is always present in combo box.
        LanguageChoice->SetSelection( 0 );
    }

}

void prefs_dlg::InitializeSystemTemporary()
{
    // System temp files settings
    delete_readonly_cb->SetValue( rsettings.systemp.delete_ro );
    delete_emptyfolder_cb->SetValue(rsettings.systemp.delete_subfolders );

    wchar_t age[17] = { 0 };
    swprintf( age, L"%d", rsettings.systemp.min_age );
    minage_combo->SetStringSelection( std::wstring( age ) );

}

void prefs_dlg::InitializeAutostartShortcut()
{
    // If we're in portable mode, don't allow the user to install an
    // autostart shortcut

    dcApp& app = wxGetApp();
    if ( app.IsPortable() )
    {
        shortcut_status_txt->SetLabel( _("The shortcut installation functionality is not available in portable mode." ) );
        m_panel2->Enable( false );
    }
    else
    {
        //Load (specified or last used) preset
        preset_box->Clear();
        preset_box->Append( _( "<last used>" ) );

        diskcleaner::dcpreset_handler preset_handler( wxConfigBase::Get( false ), NULL );

        //Get saved presets
        wxArrayString preset_list;
        preset_list.Empty();

        preset_handler.get_saved_preset_names( preset_list );

        //Add the saved preset list to the preset combo box
        preset_box->Append( preset_list );
        preset_box->SetSelection( 0 );

        // Set status text to ""
        shortcut_status_txt->SetLabel( L"" );
    }
}

void prefs_dlg::InitializeUserLocations()
{
    local_user_locations.clear();
    local_user_locations.reserve( rsettings.userlocations.size() );

    for ( unsigned int idx = 0; idx < rsettings.userlocations.size(); ++idx )
    {
        m_userlocation_box->Append( rsettings.userlocations[idx].Path );
        local_user_locations.push_back( rsettings.userlocations[idx] );
    }

    if ( rsettings.userlocations.size() > 0 )
    {
        m_userlocation_box->Select( 0 );
    }
}

void prefs_dlg::add_location_btn_clicked( wxCommandEvent& event )
{
    add_location_dlg addloc( this );
    if ( addloc.ShowModal() == wxOK )
    {
        m_userlocation_box->Append( addloc.GetValue() );
        diskcleaner::user_location loc;
        loc.Path = addloc.GetValue();
        loc.filesonly = loc.hidden = loc.includebasefolder = loc.readonly = loc.subfolders
                        = loc.subfoldersonly = loc.system = false;
        local_user_locations.push_back( loc );
    }


}

void prefs_dlg::cancel_btn_clicked( wxCommandEvent& event )
{
    EndModal( wxID_CANCEL );
}

void prefs_dlg::ok_btn_clicked( wxCommandEvent& event )
{

    // If a restart is necessary to comply with the chosen options below, set
    // the return code to wxID_OK. Else, the return code is wxID_CANCEL
    // Somehow, just setting the return code with SetReturnCode() doesn't work
    // and EndModal( returncode ) is necessary.

    int returncode = wxID_OK;

    if( ( rsettings.global.hide_admin == hide_admin_items_cb->IsChecked() ) &&
            rsettings.global.hide_empty == hide_empty_cb->IsChecked() &&
            rsettings.global.language_id == (long)  LanguageChoice->GetClientData( LanguageChoice->GetSelection() ) )
    {
        wxLogDebug( L"%hs: setting returncode to wxID_CANCEL ( %d )", __FUNCTION__, wxID_CANCEL );
        returncode = wxID_CANCEL;
    }

    rsettings.global.delete_locked              = delete_locked_cb->IsChecked();
    rsettings.global.hide_empty                 = hide_empty_cb->IsChecked();
    rsettings.global.hide_admin                 = hide_admin_items_cb->IsChecked();
    rsettings.global.show_running_processes     = warn_open_processes_cb->IsChecked();
    rsettings.global.language_id                = (long) LanguageChoice->GetClientData( LanguageChoice->GetSelection() );
    rsettings.systemp.delete_ro                 = delete_readonly_cb->IsChecked();
    rsettings.systemp.delete_subfolders         = delete_emptyfolder_cb->IsChecked();
    rsettings.userlocations.swap( local_user_locations );

    minage_combo->GetString( minage_combo->GetSelection() ).ToLong( &rsettings.systemp.min_age );

    rsettings.tempinternetfiles.delete_offline  = tempinet_offline_cb->IsChecked();

//    rsettings.cookies.use_cookie_filter         = cookie_filter_cb->IsChecked();
//    cookie_age_combo->GetString( cookie_age_combo->GetSelection() ).ToLong( &rsettings.cookies.min_cookie_age );

    rsettings.Save();

    wxLogDebug( L"%hs: EndModal ( %s ) ( returncode: %d )", __FUNCTION__, (returncode == wxID_OK)? L"wxID_OK":L"wxID_CANCEL", returncode  );
    EndModal( returncode );

}

// This code is largely based on the MSDN example at
// http://msdn.microsoft.com/en-us/library/bb776891%28VS.85%29.aspx
void prefs_dlg::autostart_install_btn_clicked( wxCommandEvent& event )
{
    HRESULT hres;
    IShellLink *psl;
    BOOL bUninitCom = FALSE;

    if ( SUCCEEDED(CoInitialize(NULL)))
    {
        bUninitCom = TRUE;
    }

    hres = CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **) &psl);

    if( SUCCEEDED(hres) )
    {
        IPersistFile *ppf;
        wxString preset_name = preset_box->GetString( preset_box->GetSelection() );
        if( preset_box->GetSelection() == 0 ) // Last used
        {
            preset_name = L"/q";
        }
        else preset_name = L"/q /r \"" + preset_name + L"\"";

        psl->SetPath( wxStandardPaths::Get().GetExecutablePath().c_str() );
        psl->SetDescription( L"Disk Cleaner autostart" );
        psl->SetArguments( preset_name.c_str() );
        psl->SetIconLocation( wxStandardPaths::Get().GetExecutablePath().c_str(), 0 );
        psl->SetWorkingDirectory( wxStandardPaths::Get().GetExecutablePath().c_str() );
        hres = psl->QueryInterface( IID_IPersistFile, (void **) &ppf );

        if( SUCCEEDED(hres))
        {
            wchar_t path[ MAX_PATH ];

            if (SHGetFolderPath( NULL, CSIDL_STARTUP,  NULL, 0, path ) != E_FAIL )
            {
                lstrcat( path, DCLinkName );
                hres = ppf->Save( path, TRUE );
                ppf->Release();
            }
        }

        psl->Release();
    }

    if (bUninitCom)
    {
        CoUninitialize();
    }

    if (SUCCEEDED( hres ) )
    {
        shortcut_status_txt->SetLabel( _( "Operation was successful." ) );
    }
    else
    {
        shortcut_status_txt->SetLabel( _( "Operation failed." ) );
    }
}

void prefs_dlg::autostart_remove_btn_clicked( wxCommandEvent& event )
{
    wchar_t path[ MAX_PATH ];

    if (SHGetFolderPath( NULL, CSIDL_STARTUP,  NULL, 0, path ) != E_FAIL )
    {
        lstrcat( path, DCLinkName );
        if ( DeleteFile( path ) )
        {
            shortcut_status_txt->SetLabel( _( "Operation was successful." ) );
        }
        else
        {
            shortcut_status_txt->SetLabel( _( "Operation failed." ) );
        }
    }
}

void prefs_dlg::userlocationbox_change( wxCommandEvent& event )
{
    int idx = m_userlocation_box->GetSelection();
    diskcleaner::user_location& loc = local_user_locations[idx];

    // Safe to start with
    m_no_option->SetValue( true );

    if ( loc.subfoldersonly ) m_option_so->SetValue( true );
    if ( loc.includebasefolder) m_option_si->SetValue( true );
    if ( loc.subfolders ) m_option_s->SetValue( true );

    if ( loc.readonly ) m_option_ar->SetValue( true );
    if ( loc.hidden ) m_option_ah->SetValue( true );
    if ( loc.system ) m_option_as->SetValue( true );


}
void prefs_dlg::userlocationbox_clicked( wxCommandEvent& event )
{

}
