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
#include <wx/intl.h>
#include <wx/log.h>
#include "core/disk scan/diskscan.h"
#include "gui/dcsettings.h"
#include "systemp.h"

namespace diskcleaner
{

    bool system_temp::SysTempAvailable = false;

    system_temp::system_temp( diskcleaner::systemp_& rsettings ) : PlugInfo(), settings( rsettings )
    {
        ShortDesc = _( "Temporary files" );
        LongDesc = _( "Removes temporary files from the current user's temporary folder, " );

        wchar_t* tmpenv = new wchar_t[MAX_PATH]; //Allocate memory, Environment string
        wchar_t *winpath = new wchar_t[MAX_PATH]; //Windows folder
        wchar_t *sysfolder = new wchar_t[MAX_PATH]; //System folder

        int getenv_returnval,winpath_returnval,sysfld_retval; //Return values

        if (!(tmpenv && winpath && sysfolder)) return; //Fail if no memory could be
        //allocated


        getenv_returnval = GetEnvironmentVariable( L"TMP", tmpenv, MAX_PATH );
        winpath_returnval = GetWindowsDirectory( winpath, sizeof( winpath ) );
        sysfld_retval = GetSystemDirectory( sysfolder, sizeof( sysfolder ) );

        if (!winpath_returnval || (winpath_returnval > MAX_PATH) ||
                !sysfld_retval || (sysfld_retval > MAX_PATH))
            return;                 //functions failed or not enough buffer space
        //simple failure handling: don't allow cleaning

        if ( getenv_returnval > MAX_PATH )
        {
            delete[] tmpenv;
            tmpenv = new wchar_t[getenv_returnval];
            getenv_returnval = GetEnvironmentVariable( L"TMP", tmpenv, sizeof( tmpenv ) );
        }
        if ( !getenv_returnval ) //No TMP variable found so try TEMP
        {
            getenv_returnval = GetEnvironmentVariable( L"TEMP", tmpenv, sizeof( tmpenv ) );
            if ( getenv_returnval > MAX_PATH )
            {
                delete[] tmpenv;
                tmpenv = new wchar_t[getenv_returnval];
                getenv_returnval = GetEnvironmentVariable( L"TEMP", tmpenv, sizeof( tmpenv ) );
            }
        }
        if ( !getenv_returnval ) return; //Still no Temp folder found. Disable option.
        if ( lstrlen( tmpenv ) < 4 ) return;  //Do not allow temp paths in root of drive.
        LongDesc += std::wstring( tmpenv );

        CharUpper( tmpenv ); //No '\' on end of tmpenv
        CharUpper( winpath );
        CharUpper( sysfolder );
        if ( !lstrcmpi( tmpenv, winpath ) || wcsstr( tmpenv, sysfolder ) )
            return;        //Do not delete windows folder or
        //(subfolder of) system folder


        tempfolder = std::wstring( tmpenv ) + L"\\";

        SysTempAvailable = true;

        delete[] sysfolder;
        delete[] winpath;
        delete[] tmpenv;
    }
//---------------------------------------------------------------------------
    void system_temp::GetFilesAsStrings(std::vector<std::wstring>& Messages)
    {
        unsigned int count = ItemList.size();
        Messages.reserve(count);

        for (unsigned int k = 0;k < count;++k)
        {
            Messages.push_back(ItemList[k]);
        }

    }

    void system_temp::Scan()
    {
        if ( SysTempAvailable )
        {

            TScanOptions so;
            so.ReadOnly = settings.delete_ro;
            so.Recursive = settings.delete_subfolders;
            so.minimum_age = settings.min_age;

            DSdata ds = GetFilesInFolder( tempfolder.c_str(), L"*.*\0", &so, ItemList );
            ItemsFound = ds.files;
            BytesFound = ds.bytes;
        }
    }

    void system_temp::Clean()
    {
        if ( !SysTempAvailable )
        {
            ::wxLogWarning( _( "Could not clean the temporary folder" ) );
            return; //Fail
        }

        TScanOptions so;
        so.ReadOnly = settings.delete_ro;
        so.Recursive = settings.delete_subfolders;

        DSdata ds = CleanFilesInFolder( tempfolder.c_str(), L"*.*\0", &so, ItemList );

        ItemsCleaned = ds.files;
        BytesCleaned = ds.bytes;


    }
//---------------------------------------------------------------------------

}
