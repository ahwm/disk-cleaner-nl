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
#include <stdlib.h>
#include "prefs_dlg.h"


prefs_dlg::prefs_dlg( wxWindow* parent, diskcleaner::dcsettings& prefs )
    : prefs_dlg_base( parent ), rsettings( prefs )
{
    // Global settings
    delete_locked_cb->SetValue( prefs.global.delete_locked );
    hide_empty_cb->SetValue( prefs.global.hide_empty );
    hide_admin_items_cb->SetValue( prefs.global.hide_admin );
    warn_open_processes_cb->SetValue( prefs.global.show_running_processes );

    // System temp files settings
    delete_readonly_cb->SetValue( prefs.systemp.delete_ro );
    delete_emptyfolder_cb->SetValue(prefs.systemp.delete_subfolders );

    wchar_t age[17] = { 0 };
    swprintf( age, L"%d", prefs.systemp.min_age );
    minage_combo->SetStringSelection( std::wstring( age ) );

    // Temporary internet files settings
    tempinet_offline_cb->SetValue( prefs.tempinternetfiles.delete_offline );

//    cookie_filter_cb->SetValue( prefs.cookies.use_cookie_filter );
//   swprintf( age, L"%d", prefs.cookies.min_cookie_age );
//   cookie_age_combo->SetStringSelection( std::wstring( age ) );
    ok_cancelOK->SetDefault();
    prefsbook->SetSelection( 0 );

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
            rsettings.global.hide_empty == hide_empty_cb->IsChecked() )
    {
        wxLogDebug( L"%hs: setting returncode to wxID_CANCEL ( %d )", __FUNCTION__, wxID_CANCEL );
        returncode = wxID_CANCEL;
    }

    rsettings.global.delete_locked              = delete_locked_cb->IsChecked();
    rsettings.global.hide_empty                 = hide_empty_cb->IsChecked();
    rsettings.global.hide_admin                 = hide_admin_items_cb->IsChecked();
    rsettings.global.show_running_processes     = warn_open_processes_cb->IsChecked();
    rsettings.systemp.delete_ro                 = delete_readonly_cb->IsChecked();
    rsettings.systemp.delete_subfolders         = delete_emptyfolder_cb->IsChecked();

    minage_combo->GetString( minage_combo->GetSelection() ).ToLong( &rsettings.systemp.min_age );

    rsettings.tempinternetfiles.delete_offline  = tempinet_offline_cb->IsChecked();

//    rsettings.cookies.use_cookie_filter         = cookie_filter_cb->IsChecked();
//    cookie_age_combo->GetString( cookie_age_combo->GetSelection() ).ToLong( &rsettings.cookies.min_cookie_age );

    rsettings.Save();

    wxLogDebug( L"%hs: EndModal ( %s ) ( returncode: %d )", __FUNCTION__, (returncode == wxID_OK)? L"wxID_OK":L"wxID_CANCEL", returncode  );
    EndModal( returncode );

}

