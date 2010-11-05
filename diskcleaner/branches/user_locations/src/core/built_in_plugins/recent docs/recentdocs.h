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

#ifndef __RECENTDOCSH
#define __RECENTDOCSH

#include "core/plugin_info_base.h"

namespace diskcleaner
{
    class recent_docs : public PlugInfo
    {
        std::vector<std::wstring> ItemList;

        public:
        recent_docs();
        virtual void Clean();
        virtual void Preview() { Scan(); };
        virtual void Scan();
        void GetFilesAsStrings(std::vector<std::wstring>& Messages);

    };
}

#endif //__RECENTDOCSH

//Registry key for Shell Folders
//#define SHELL_FOLDERS_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"
//



