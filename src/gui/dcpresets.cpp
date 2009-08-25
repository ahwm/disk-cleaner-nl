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

#include "dcpresets.h"
#include "wx/wx.h"
#include "wx/fileconf.h"
#include <wx/arrstr.h>
#include <wx/log.h>
#include "wxCheckedListCtrl.h"

namespace diskcleaner
{

    dcpreset_handler::dcpreset_handler(wxConfigBase* const config, wxCheckedListCtrl* const checklist_ctrlwindow ) :
            cfg_file(config), checklist_ctrl(checklist_ctrlwindow)
    {

    }

    void dcpreset_handler::get_saved_preset_names( wxArrayString& presetlist )
    {
        csave_restore_path srp(this);


        cfg_file->SetPath( L"/SavedPresets" );

        wxString preset_name;
        long index;

        bool more = cfg_file->GetFirstGroup( preset_name, index );

        while ( more )
        {
            wxLogDebug( L"%hs: adding found preset %s", __PRETTY_FUNCTION__, preset_name.c_str() );
            presetlist.Add( preset_name );
            more = cfg_file->GetNextGroup( preset_name, index );
        }


    }

    bool dcpreset_handler::save_preset( const std::wstring preset_name )
    {
        csave_restore_path srp(this);

        cfg_file->SetPath( L"/SavedPresets/" +  preset_name );

        bool success = true;
        long count = checklist_ctrl->GetItemCount();
        for ( int k = 0; k < count; ++k )
        {
            diskcleaner::PlugInfo* pi = (diskcleaner::PlugInfo*) checklist_ctrl->GetItemData( k );
            wxString item( pi->GetShortDesc() );

            success &= cfg_file->Write( item, checklist_ctrl->IsChecked( k ) );

        };

        return success;
    }

    void dcpreset_handler::load_preset( const std::wstring preset_name )
    {
        csave_restore_path srp(this);

        cfg_file->SetPath( L"/SavedPresets/" +  preset_name );
        long count = checklist_ctrl->GetItemCount();
        for ( int k = 0; k < count; ++k )
        {

            bool is_checked = false;

            diskcleaner::PlugInfo* pi = (diskcleaner::PlugInfo*) checklist_ctrl->GetItemData( k );
            wxString item( pi->GetShortDesc() );

            cfg_file->Read( item, &is_checked, false );

            checklist_ctrl->Check( k, is_checked );
        };


    }

     bool dcpreset_handler::delete_preset( const std::wstring preset_name )
    {
        csave_restore_path srp(this);

        cfg_file->SetPath( L"/SavedPresets" );

        return cfg_file->DeleteGroup( preset_name );

    }

    bool dcpreset_handler::save_last_used( )
    {
        csave_restore_path srp( this );

        cfg_file->SetPath( L"/LastUsedPreset" );

        bool success = true;

        for ( int k = 0, count = checklist_ctrl->GetItemCount(); k < count; ++k )
        {
            diskcleaner::PlugInfo* pi = (diskcleaner::PlugInfo*) checklist_ctrl->GetItemData( k );
            wxLogDebug( L"%hs: saving state of %s", __FUNCTION__, pi->GetShortDesc().c_str() );
            wxString item( pi->GetShortDesc() );

            success &= cfg_file->Write( item, checklist_ctrl->IsChecked( k ) );


        };


        return success;
    }

    void dcpreset_handler::load_last_used( )
    {
        csave_restore_path srp( this );

        cfg_file->SetPath( L"/LastUsedPreset" );
        long count = checklist_ctrl->GetItemCount();
        for ( int k = 0; k < count; ++k )
        {
            bool is_checked = false;

            diskcleaner::PlugInfo* pi = (diskcleaner::PlugInfo*) checklist_ctrl->GetItemData( k );

            wxString item(pi->GetShortDesc() );

            cfg_file->Read( item, &is_checked, false );

            checklist_ctrl->Check( k, is_checked );
        };


    }

    csave_restore_path::csave_restore_path( const dcpreset_handler* const adph ) :
            dph( adph ), current_path ( dph->cfg_file->GetPath() ) {};

    csave_restore_path::~csave_restore_path()
    {
        dph->cfg_file->SetPath( current_path );
    };
}
