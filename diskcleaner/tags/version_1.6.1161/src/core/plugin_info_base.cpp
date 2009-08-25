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

#include "plugin_info_base.h"

namespace diskcleaner
{


//Not thread safe accessing!!
    std::map<std::wstring,HANDLE> IconList;
    std::map<std::wstring,HANDLE>::iterator icon_iterator;

//--------------------------------------------------------------------------
//LoadIconShared - Keeps track of loaded icons. Load icons from disk
//if needed, otherwise returns handle of already loaded icon
//--------------------------------------------------------------------------
    HANDLE LoadIconShared(std::wstring& filename)
    {
//        transform(filename.begin(), filename.end(), filename.begin(), tolower);
//        icon_iterator = IconList.find(filename);
//
//        if (icon_iterator == IconList.end())
//        {
//            //addlog("Icon '" + filename + "' not found in in-memory list. Loading from file");
//            HANDLE hdl = LoadImage(NULL,filename.c_str(), IMAGE_ICON ,16,16, LR_LOADFROMFILE);
//
//            if (hdl)
//            {
//                IconList.insert(std::map<std::wstring,HANDLE>::value_type(filename,hdl));
//            }
//
//            return hdl;
//        }
//
//        else
//        {
//            //addlog("Found Icon '" + filename + "' in list.");
//            return icon_iterator->second;
//        }
        return NULL;
    }

}//namespace
