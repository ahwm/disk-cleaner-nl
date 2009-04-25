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

namespace diskcleaner
{

    dcpreset_handler::dcpreset_handler(wxConfigBase* const config, wxCheckListBox* const checklistboxwindow,
                                       std::vector<diskcleaner::PlugInfo*>& plugin_list ) :
            cfg_file(config), checklistbox(checklistboxwindow), pi_list( plugin_list )
    {

    }

    void dcpreset_handler::get_saved_preset_names( std::vector<std::wstring>& presetlist )
    {
        save_path();

        wxString preset_name;
        long index;

        bool more = cfg_file->GetFirstEntry( preset_name, index );
        presetlist.push_back( preset_name );

        while ( more )
        {
            cfg_file->GetNextEntry( preset_name, index );
            presetlist.push_back( preset_name );
        }

        restore_path();
    }

    bool dcpreset_handler::save_preset( const std::wstring preset_name )
    {
        save_path();
        cfg_file->SetPath( L"SavedPresets/" +  preset_name );

        bool success = true;
        long count = checklistbox->GetCount();
        for ( int k = 0; k < count; ++k )
        {
            // Pray that pi_list and wxCheckListBox have the same items at the same index

            wxString item(pi_list[k]->GetShortDesc());

            success &= cfg_file->Write( item, checklistbox->IsChecked( k ) );


        };

        restore_path();

        return success;
    }

    void dcpreset_handler::load_preset( const std::wstring preset_name )
    {
        save_path();

        cfg_file->SetPath( L"SavedPresets/" +  preset_name );
        long count = checklistbox->GetCount();
        for ( int k = 0; k < count; ++k )
        {
            // Pray that pi_list and wxCheckListBox have the same items at the same index
            bool is_checked = false;
            wxString item(pi_list[k]->GetShortDesc());

            cfg_file->Read( item, &is_checked, false );

            checklistbox->Check( k, is_checked );
        };

        restore_path();
    }

    bool dcpreset_handler::save_last_used( )
    {
        save_path();
        cfg_file->SetPath( L"LastUsedPreset" );

        bool success = true;
        long count = checklistbox->GetCount();
        for ( int k = 0; k < count; ++k )
        {
            // Pray that pi_list and wxCheckListBox have the same items at the same index

            wxString item(pi_list[k]->GetShortDesc());

            success &= cfg_file->Write( item, checklistbox->IsChecked( k ) );


        };

        restore_path();

        return success;
    }

    void dcpreset_handler::load_last_used( )
    {
        save_path();

        cfg_file->SetPath( L"LastUsedPreset" );
        long count = checklistbox->GetCount();
        for ( int k = 0; k < count; ++k )
        {
            // Pray that pi_list and wxCheckListBox have the same items at the same index
            bool is_checked = false;
            wxString item(pi_list[k]->GetShortDesc());

            cfg_file->Read( item, &is_checked, false );

            checklistbox->Check( k, is_checked );
        };

        restore_path();
    }


    inline void dcpreset_handler::save_path()
    {
        current_path = cfg_file->GetPath();
    }

    inline void dcpreset_handler::restore_path()
    {
        cfg_file->SetPath( current_path );
    }

}
