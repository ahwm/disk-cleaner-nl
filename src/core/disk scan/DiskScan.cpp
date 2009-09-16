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

//---------------------------------------------------------------------------

#include <windows.h>
#include <wx/log.h>

#include "DiskScan.h"

namespace DiskScan
{
    bool RemoveOnReboot;
    __int64 FilesScheduled;
}

void SetRemoveOnReboot(bool Remove)
{
    DiskScan::RemoveOnReboot = Remove;
};
__int64 GetFilesScheduledRemoveOnReboot()
{
    return DiskScan::FilesScheduled;
};
void ResetFilesScheduledRemoveOnReboot()
{
    DiskScan::FilesScheduled = 0;
};

//Forward declaration
bool ProcessFile( const wchar_t * const the_file, std::vector<std::wstring>& FileList,
                         const TScanOptions * const so, const WIN32_FIND_DATA& find_data, bool Remove );

void ProcessFilesInFolder(const wchar_t* folder, const wchar_t* masks, const TScanOptions* so,
                            std::vector<std::wstring>& FileList, DSdata& scandata, bool Remove = false )
{
    HANDLE hFileSearch;
    WIN32_FIND_DATA sr;
    static wchar_t __thread localfolder[MAX_PATH];
    wchar_t* ptr;
    const wchar_t* maskfw = masks;

    TScanOptions* so_local;


    if (folder) //This is the first time the function is called. This is the base folder
    {
        ::wxLogDebug( L"%hs setting up %s...", __FUNCTION__ , folder );

        lstrcpy(localfolder,folder);

        scandata.files = 0;
        scandata.bytes = 0;
        so_local = new TScanOptions(*so);
        //*so_local = *so;

        //For further calls to GetFilesInFolder(), we *are* in subfolders
        if (so->SubFolderOnly) so_local->SubFolderOnly = false;
    }
    else
    {
        so_local = const_cast<TScanOptions*>(so);
    }


    //Set ptr to last backslash
    ptr = std::wcsrchr( localfolder, '\\' ); //Set up string for subfolders
    ++ptr; //Set ptr to first character after last backslash

    if ( !so->SubFolderOnly ) //Do scan in this folder (option:  /so)
    {
        //scan this folder with the masks provided
        while ( *masks )
        {

            *ptr=0;                     //Make sure the string is zero terminated
            lstrcat(localfolder,masks); //append mask

            ::wxLogDebug( L"%hs: FindFirstFile(%s)", __FUNCTION__, localfolder );
            hFileSearch = FindFirstFile(localfolder,&sr);

            if (hFileSearch != INVALID_HANDLE_VALUE)
            {
                do
                {
                    if ( !(sr.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY|
                                                 FILE_ATTRIBUTE_OFFLINE|
                                                 FILE_ATTRIBUTE_SYSTEM) ) )
                    {
                        ::wxLogDebug( L"%hs: FindFirstFile() found: %s ",__FUNCTION__, sr.cFileName );
                        *ptr = 0;
                        lstrcat( localfolder, sr.cFileName );

                        if ( ProcessFile( localfolder, FileList, so_local, sr, Remove ) )
                        {
                            scandata.bytes += (sr.nFileSizeHigh * MAXDWORD) + sr.nFileSizeLow;
                            scandata.files++;
                        }
                    }
                }
                while (FindNextFile(hFileSearch,&sr));
                FindClose(hFileSearch);
            }
            masks += lstrlen(masks) + 1;
        }
    } //if(!so->SubFoldersOnly)   //so

    if (so->Recursive )       //Scan subfolders?
    {
        //Next scan for all folders inside this one

        *ptr = 0; //Zero terminate string after last backslash

        ::wxLogDebug( L"%hs: Scan for subfolders of %s", __FUNCTION__, localfolder );

        lstrcat(localfolder,L"*.*");

        hFileSearch = FindFirstFile(localfolder,&sr);

        if (hFileSearch != INVALID_HANDLE_VALUE)
        {
            do
            {

                if ( (sr.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) &&
                        ( lstrcmp(sr.cFileName,L".") ) && ( lstrcmp(sr.cFileName,L"..") ) )
                {


                    *ptr = 0;
                    ::wxLogDebug( L"%hs: FindFirstFile(%s) has subfolder %s", __FUNCTION__, localfolder, sr.cFileName );

                    lstrcat(localfolder,sr.cFileName);
                    lstrcat(localfolder,L"\\");

                    ProcessFilesInFolder( NULL, maskfw, so_local, FileList, scandata, Remove ); //Use localfolder as is
                    if ( Remove && !so->FilesOnly )
                    {
                        *ptr = 0;
                        lstrcat(localfolder,sr.cFileName);
                        ::wxLogDebug( L"Removing folder %s", localfolder );
                        if ( !RemoveDirectory( localfolder ) )
                        {
                            ::wxLogWarning( _T( "Could not remove folder %s as promised. It is probably not empty" ), localfolder );
                        }
                    }

                }
            }
            while ( FindNextFile( hFileSearch, &sr) );

            FindClose( hFileSearch );
        } //Done with folders
    }

    if ( folder )    //Are we at the end of the scanning? (true if folder != NULL)
    {
        delete so_local; //Remove unneeded structure
        if ( so->DelBaseFolder )  //si
        {
            ::wxLogDebug( ( Remove ) ? L"%hs: Would try to delete %s" : L"%hs: Trying to delete %s", folder, localfolder);
            if ( Remove )
            {
                if ( !RemoveDirectory( folder ) )
                {
                    ::wxLogWarning( _T( "Unable to remove the base folder %s as requested (option /si)" ), folder );
                }
            }
        }

    }


}



DSdata GetFilesInFolder(const wchar_t* folder, const wchar_t* masks,
                         const TScanOptions* so, std::vector<std::wstring>& FileList)
{
    DSdata ds = { 0 };
    ProcessFilesInFolder( folder, masks, so, FileList, ds, false );
    return ds;
}


DSdata CleanFilesInFolder(const wchar_t* folder, const wchar_t* masks,
                           const TScanOptions* so, std::vector<std::wstring>& FileList)
{
    DSdata ds = { 0 };
    ProcessFilesInFolder (folder, masks, so, FileList, ds, true );
    return ds;
}

inline bool ProcessFile( const wchar_t * const the_file, std::vector<std::wstring>& FileList,
                         const TScanOptions* const so, const WIN32_FIND_DATA& find_data, bool Remove )
{
    //wxLogDebug( L"%hs: processing file %s", __FUNCTION__, the_file );

    //Get file age etc only if necessary
    if( so->minimum_age > 0 )
    {
        ::wxLogDebug( L"%hs: Checking file age of %s", __FUNCTION__, the_file );
        ULARGE_INTEGER ftime_int64 = { find_data.ftLastWriteTime.dwLowDateTime, find_data.ftLastWriteTime.dwHighDateTime };

        FILETIME systime;
        SYSTEMTIME debug;
        GetSystemTimeAsFileTime( & systime );

        ULARGE_INTEGER stime_int64 = { systime.dwLowDateTime, systime.dwHighDateTime };
#ifdef __WXDEBUG__
        FileTimeToSystemTime( &find_data.ftLastWriteTime, &debug);

        wxLogDebug( L"LastWriteTime: Year %d, Month %d, Day %d, Hour %d, Minute %d, Second %d", debug.wYear, debug.wMonth,
                    debug.wDay, debug.wHour, debug.wMinute, debug.wSecond );

        GetSystemTime( &debug );
        wxLogDebug( L"Current System Time: Year %d, Month %d, Day %d, Hour %d, Minute %d, Second %d", debug.wYear, debug.wMonth,
                    debug.wDay, debug.wHour, debug.wMinute, debug.wSecond );
#endif

        unsigned __int64 diff = stime_int64.QuadPart - ftime_int64.QuadPart;

        //TODO:Should specify unsigned __int64 type and make this one statement
        diff /= ( 10*1000*1000);     //diff is in 100 ns intervals, convert to seconds
        diff /= ( 60*60*24 );        //convert to a day

        ::wxLogDebug( L"%hs: file age = %I64d days, minimum age = %d",
                          __FUNCTION__, diff, so->minimum_age );

        if ( diff < so->minimum_age )
        {
            ::wxLogDebug( L"%hs: skipping %s", __FUNCTION__, the_file );
            return false;
        }

        ::wxLogDebug( L"%hs: adding %s", __FUNCTION__, the_file );
    }

    if ( !Remove )
    {



        FileList.push_back( std::wstring( the_file ) );
        return true;
    }
    else
    {
        int swresult = 0;
//              if(so->Secure)
//              {
//                swresult = SWWipeFileByName(localfolder,dcglobals::ctxt);
//                if(!swresult)
//                {
////                 addlog("Warning - unable to wipe file: "  + String(localfolder));
//                 FileList.push_back("Warning - unable to wipe file: " + String(localfolder));
//                }
//              }
        if ( DeleteFile( the_file ) )
        {

//                            if (swresult)
//                            {
//                                FileList.push_back( ( L"Wiped and deleted: ") +  std::wstring(localfolder));
//                            }
//                            else
//            {
            wxLogDebug( L"%hs: Deleted %s", __FUNCTION__, the_file );
            return true;
//            }
        }
        else //We might be allowed to try remove on reboot
        {
            if ( DiskScan::RemoveOnReboot )
            {
                bool mvresult = ( MoveFileEx( the_file, NULL,MOVEFILE_DELAY_UNTIL_REBOOT ) == TRUE );

                if ( !mvresult ) //Remove on reboot failed, need administrator priviliges
                {
                    ::wxLogWarning( _T( "Warning - unable to schedule for removal on reboot: %s" ), the_file );
                }
                else
                {
                    ++DiskScan::FilesScheduled;

//                    if ( swresult )
//                    {
//                        :wxLogMessage( L"Wiped and scheduled for removal on reboot: %s", localfolder );
//                    }
//                    else
                    {
                        ::wxLogMessage( _T( "Scheduled for removal on reboot: %s" ), the_file );
                    }
                    return true;
                }
            }
            else
            {
                ::wxLogWarning( _T( "Could not delete file: %s. It is probably in use." ), the_file );

                return false;
            }
        }
    }
    return false;
}
