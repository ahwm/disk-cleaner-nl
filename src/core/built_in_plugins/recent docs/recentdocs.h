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

#endif //__MRUH

//Registry keys for MRUs
//url
//#define TYPED_URL_KEY "Software\\Microsoft\\Internet Explorer\\TypedURLs"
////url
//#define ADDRESS_BAR_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU"
////classic
//#define RUN_MRU_KEY ADDRESS_BAR_KEY
////classic
//#define FIND_DOC_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Doc Find Spec MRU"
////classic
//#define FIND_COMP_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FindComputerMRU"
////url
//#define MEDIAPLAYER_URL_KEY "Software\\Microsoft\\MediaPlayer\\Player\\RecentURLList"
////File
//#define MEDIAPLAYER_URL_KEY2 "Software\\Microsoft\\MediaPlayer\\Player\\RecentFileList"
////extract
//#define WINZIP_EXTRACT_KEY "Software\\Nico Mak Computing\\WinZip\\extract"
////filemenu
//#define WINZIP_RECENT_FILES "Software\\Nico Mak Computing\\WinZip\\filemenu"
////File
//#define PAINT_RECENT_FILES "Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Paint\\Recent File List"
//
////File
//#define WORDPAD_RECENT_FILES "Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Wordpad\\Recent File List"

//Registry key for Shell Folders
//#define SHELL_FOLDERS_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"
//
//#define WORDXP_KEY "Software\\Microsoft\\Office\\10.0\\Word\\Data"


//HKEY_CURRENT_USER\Software\Microsoft\Office\10.0\Excel\Recent Files
//HKEY_CURRENT_USER\Software\Microsoft\Office\10.0\Access\Settings



