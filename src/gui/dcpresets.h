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
#include "wx/fileconf.h"

#include "core\plugin_info_base.h"


class wxCheckedListCtrl;
class wxArrayString;

namespace diskcleaner
{


    class dcpreset_handler
    {
    public:

        /// Constructor. checklist_ctrlwindow can be NULL
        dcpreset_handler(wxConfigBase* const cf, wxCheckedListCtrl* const checklist_ctrlwindow );

        //Returns a vector of wstrings containing the names of all saved presets
        void get_saved_preset_names( wxArrayString& presetlist );

        //bool is_enabled( const std::wstring& preset_name, const std::wstring& name );

        bool save_preset( const std::wstring preset_name );

        void load_preset( const std::wstring preset_name );

        bool delete_preset( const std::wstring preset_name );

        bool save_last_used();

        void load_last_used();

    protected:
    private:
        friend class csave_restore_path;


        wxConfigBase* cfg_file;
        wxCheckedListCtrl* checklist_ctrl;
    };

    /// A utility class to save and restore the path inside a config file.
    class csave_restore_path
    {
    private:
        const dcpreset_handler* dph;
        std::wstring current_path;

    public:
        csave_restore_path( const dcpreset_handler* const adph );

        ~csave_restore_path();

    };
}

#endif // DCPRESETS_H
