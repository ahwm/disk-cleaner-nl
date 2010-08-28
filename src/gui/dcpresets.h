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

#ifndef DCPRESETS_H
#define DCPRESETS_H

#include <string>
#include <vector>
#include "core\plugin_info_base.h"

class wxConfigBase;
class wxCheckedListCtrl;
class wxArrayString;

namespace diskcleaner
{

    /// utility class to handle loading presets
    class dcpreset_handler
    {
    public:
        dcpreset_handler( wxCheckedListCtrl* const checklist_ctrlwindow );

        // Returns a vector of wstrings containing the names of all saved presets
        // No interaction with wxCheckedListCtrl necessary
        static void get_saved_preset_names( wxArrayString& presetlist );

        // No interaction with wxCheckedListCtrl necessary
        static bool delete_preset( const std::wstring preset_name );

        bool save_preset( const std::wstring preset_name );

        void load_preset( const std::wstring preset_name );


        bool save_last_used();

        void load_last_used();

    protected:
    private:
        friend class csave_restore_path;

        wxCheckedListCtrl* checklist_ctrl;
    };

    /// A utility class to save and restore the path inside a config file.
    class csave_restore_path
    {
    private:
        wxConfigBase* const cfg;
        std::wstring current_path;

    public:
        csave_restore_path( wxConfigBase* const config_object );

        ~csave_restore_path();

    };
}

#endif // DCPRESETS_H
