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

#ifndef TextPlugInfoH
#define TextPlugInfoH
//---------------------------------------------------------------------------
#include "..\plugin_info_base.h"

#include "..\disk scan\DiskScan.h"

#include <map>
#include <set>
#include <iterator>
#include <vector>


namespace diskcleaner {

class TextPlugInfo : public PlugInfo
{
        std::wstring FileName;           //Name of the text plugin on disk

        HICON IconHandle;              //Handle to icon to be shown
                                       //Can be loaded from disk

        std::vector<std::wstring> FolderList; //Holds the folder(s) to be scanned
        std::vector<std::wstring> FileList;   //Holds the found files
        TScanOptions GetScanOptions(wchar_t* folder);
public:
        TextPlugInfo(std::wstring& aFile);

        //virtual HICON GetIcon();
        virtual void Clean();
        virtual void Configure(){};
        virtual void Scan();
        virtual void Preview();
        virtual void GetFilesAsStrings(std::vector<std::wstring>&);

protected:
    void DoScan(bool Preview);



};


    bool InitializeSHGetKnownFolderPath();
    bool UninitializeSHGetKnownFolderPath();




} //namespace
#endif
