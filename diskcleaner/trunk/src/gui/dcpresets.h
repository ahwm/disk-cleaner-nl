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
class wxCheckListBox;

namespace diskcleaner
{


class dcpreset_handler
{
    public:
        dcpreset_handler(wxConfigBase* const cf, wxCheckListBox* const checklistboxwindow,
                         std::vector<diskcleaner::PlugInfo*>& plugin_list );

        //Returns a vector of wstrings containing the names of all saved presets
        void get_saved_preset_names( std::vector<std::wstring>& presetlist );

        //bool is_enabled( const std::wstring& preset_name, const std::wstring& name );

        bool save_preset( const std::wstring preset_name );

        void load_preset( const std::wstring preset_name );

        bool save_last_used();

        void load_last_used();

    protected:
    private:
      wxConfigBase* cfg_file;
      wxCheckListBox* checklistbox;
      std::vector<diskcleaner::PlugInfo*>& pi_list;
      std::wstring current_path;

      void save_path();
      void restore_path();

};

}

#endif // DCPRESETS_H
