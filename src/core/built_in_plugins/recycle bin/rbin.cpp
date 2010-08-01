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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma hdrstop

#include <wx/log.h>
#include <wx/intl.h>
#include <string.h>
#include <shellapi.h>
#include "rbin.h"

namespace diskcleaner
{
    RecycleBinInfo::RecycleBinInfo() : PlugInfo()
    {
        ShortDesc = _( "Recycle Bin" );
        LongDesc =  _( "Empties the Recycle Bin on the desktop" );
    }

    void RecycleBinInfo::Preview()
    {
        Scan();
    }

    void RecycleBinInfo::Scan()
    {

        wxLogDebug( L"%hs: Scanning the recycle bin", __FUNCTION__ );

        SHQUERYRBINFO shq;
        wchar_t* drive_buff = new wchar_t[256];
        wchar_t* pdrive_buff = drive_buff;


        ZeroMemory(&shq,sizeof(shq));
        shq.cbSize = sizeof(shq);

        GetLogicalDriveStrings(256,drive_buff);
        while (*drive_buff)
        {
            if (GetDriveType(drive_buff) == DRIVE_FIXED)
            {
                wxLogDebug( L"%hs: scanning %s" , __FUNCTION__, drive_buff );
                SHQueryRecycleBin(drive_buff,&shq);
                ItemsFound += shq.i64NumItems;
                BytesFound += shq.i64Size;
            }
            drive_buff += wcslen(drive_buff) + 1;
        }

        delete[] pdrive_buff;
        return;
    }

    void RecycleBinInfo::Clean()
    {
        HRESULT result = SHEmptyRecycleBin(NULL,NULL,SHERB_NOCONFIRMATION|SHERB_NOPROGRESSUI|
                          SHERB_NOSOUND);
        wxLogDebug( (result == S_OK)? L"Recycle bin emptied succesfully" : L"Error while trying to empty the recycle bin" );
        ItemsCleaned = ItemsFound;
        BytesCleaned = BytesFound;
    }

    void RecycleBinInfo::GetFilesAsStrings(std::vector<std::wstring>& files)
    {
        files.push_back( std::wstring( _( "Recycle bin does not support file listing" ) )  );
    }
}
