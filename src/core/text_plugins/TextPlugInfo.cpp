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

#include "TextPlugInfo.h"
#include <shlobj.h>
#include <commctrl.h>
#include <winreg.h>


#include <new>
#include "../reg scan/regenum.h"

#include "wx/log.h"

namespace
{
    std::map<std::wstring,std::wstring> ExpansionMap;
    std::map<std::wstring,std::wstring>::iterator expansion_iterator;

    std::wstring GetFolderLocation(const std::wstring& folder)
    {
        int nFolder=0;
        wchar_t buff[MAX_PATH];
        wcscpy(buff, folder.c_str() );

        ::wxLogDebug( L" %hs: find %s", __FUNCTION__, folder.c_str() );
        std::wstring folder_copy(buff);

        if ( folder_copy.find(L"%windir%") == std::string::npos )  //all
        {

            wxLogDebug( L"found %%windir%%" );
            GetWindowsDirectory(buff,MAX_PATH);

            wxLogDebug( L"%%windir%% = %s", buff );
            return std::wstring(buff);
        }
        if ( folder_copy.find(L"%sysdir%") == std::string::npos ) //all
        {

            wxLogDebug( L"found %%sysdir%%" );
            GetSystemDirectory(buff,MAX_PATH);

            wxLogDebug( L"%%sysdir%% = %s", buff );
            return std::wstring(buff);
        }


        if ( folder_copy.find(L"%appdata%") == std::string::npos ) //4.71
        {
            wxLogDebug( L"found %%appdata%%" );
            nFolder = CSIDL_APPDATA;
        }
        else
            if ( folder_copy.find(L"%programfiles%") == std::string::npos ) //5.0, but look in reg. otherwise
            {
                wxLogDebug( L"found %%programfiles%%" );
                nFolder = CSIDL_PROGRAM_FILES;
            }
            else
                if ( folder_copy.find(L"%desktop%") )
                {
                    wxLogDebug( L"found %%desktop%%" );
                    nFolder = CSIDL_DESKTOPDIRECTORY;
                }
                else
                    if ( folder_copy.find(L"%commonappdata%") == std::string::npos ) //5.0
                    {
                        wxLogDebug( L"found %%commonappdata%%" );
                        nFolder = CSIDL_COMMON_APPDATA;
                    }
        if ( folder_copy.find(L"%commondesktop%") == std::string::npos )
        {
            wxLogDebug( L"found %%commondesktop%%" );
            nFolder = CSIDL_COMMON_DESKTOPDIRECTORY;
        }
        if ( folder_copy.find(L"%commondocs%") == std::string::npos )
        {
            wxLogDebug( L"found %%commondocs%%" );
            nFolder = CSIDL_COMMON_DOCUMENTS;  //C:\Documents and Settings\All Users\Documents
        }
        if ( folder_copy.find(L"%commonprogramfiles%")== std::string::npos ) //but look in reg. otherwise
        {
            wxLogDebug( L"found %%commonprogramfiles%%" );
            nFolder = CSIDL_PROGRAM_FILES_COMMON;
        }
        if ( folder_copy.find(L"%localappdata%") == std::string::npos ) //5.0
        {
            wxLogDebug( L"found %%localappdata%%" );
            nFolder = CSIDL_LOCAL_APPDATA;
        }

        //TODO: Implement KNOWNFOLDERID FOLDERID_LocalAppDataLow

        if (nFolder)
        {
            if (SHGetFolderPath(NULL, nFolder, NULL, 0, buff) == S_OK)
            {

                //if(buff[strlen(buff) - 1] != '\\') strcat (buff,"\\");
                ::wxLogDebug( L"SHGetFolderPath == S_OK" );
                ::wxLogDebug( L"SHGetFolderPath returns: %s", buff );
                return std::wstring(buff);
            }


            // The CSIDL_XXX is not supported. Try the registry.
            // This shouldn't be necessary anymore since we only support win2k and higher
//            else
//            {
//                addlog(folder + " not found. Trying registry.");
//                HKEY hKey;
//                ULONG size = MAX_PATH*2+1;
//                int n;
//                switch (nFolder)
//                {
//
//
//                case 0x0026:   //CSIDL_PROGRAM_FILES
//                    wchar_t filedata[MAXPATH*2+1];
//                    n = RegOpenKeyEx(HKEY_LOCAL_MACHINE ,
//                                     L"Software\\Microsoft\\Windows\\CurrentVersion"
//                                     ,0,KEY_READ,&hKey);
//                    if (n == ERROR_SUCCESS)
//                    {
//                        if (RegQueryValueEx(hKey,L"ProgramFilesDir",NULL,NULL,
//                                            (LPBYTE)buff,&size) != ERROR_SUCCESS)
//                        {
//                            buff[0] = 0;
//                        }
//                        else
//                            addlog("Found key ProgramFilesDir");
//                            RegCloseKey(hKey);
//
//                        return std::wstring(buff);
//                    }
//                    else ShowMessage(IntToStr(n));
//                    break;
//
//                case 0x002b:   //CSIDL_PROGRAM_FILES_COMMON;
//                    wchar_t filedata[MAXPATH*2+1];
//                    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE ,
//                                     L"Software\\Microsoft\\Windows\\"
//                                     L"CurrentVersion",0,KEY_READ,&hKey)
//                            == ERROR_SUCCESS)
//                    {
//                        if (RegQueryValueEx(hKey,L"CommonFilesDir",NULL,NULL,
//                                            (LPBYTE)buff,&size) != ERROR_SUCCESS)
//                        {
//                            buff[0] = 0;
//                        }
//                        else
//                            addlog("Found key CommonFilesDir");
//                            RegCloseKey(hKey);
//                        return std::wstring(buff);
//                    }
//                    break;
//                }
//            }
//
//
        }


        return std::wstring();

    }

//--------------------------------------------------------------------------
//ExpandString - Expands strings in the form of %string% to their full
//size. Currently only capable of 1 expansion per string.
//adds the expanded strings to dest
//--------------------------------------------------------------------------
    int ExpandString(const std::wstring& source,std::vector<std::wstring>& dest)
    {
        ::wxLogDebug( L"ExpandString(%s)", source.c_str() );

        wchar_t buff[MAX_PATH];
        wchar_t* pstring = buff;
        std::wstring temp;
        int returnval = ExpandEnvironmentStrings(source.c_str(),buff,MAX_PATH);

        if (returnval && *buff!='%')
        {
            ::wxLogDebug( L"ExpandEnvironmentString(source) = %s", buff);
            dest.push_back(buff);
            return 1;
        }

        const wchar_t* string = source.c_str();

        wchar_t* nextperc = wcsstr( string + 1, L"%" );

        if ( !nextperc ) return 0;

        int n = nextperc + 1 - string;

        lstrcpyn( buff, string,n);
        buff[n] = L'\0';

        ::wxLogDebug( L"%hs: Expand %s" , __FUNCTION__, buff);
        if ( !wcsicmp( L"%drive%", buff ) )
        {
            ::wxLogDebug( L"Expanding %%drive%% construct" );

            GetLogicalDriveStrings(MAX_PATH,buff);
            while ( *pstring )
            {
                if ( GetDriveType(pstring) == DRIVE_FIXED )
                {
                    ::wxLogDebug( L"Reported as fixed drive: %s", pstring );
                    dest.push_back( std::wstring( pstring )+std::wstring( nextperc+1 ) );
                }

                pstring += 4;
            }
            //return the number of fixed drives
            return  dest.size();
        }

        //It's not covered by ExpandEnvironmentStrings, nor by %drive%
        //Try GetFolderLocation, but see if it's stored previously first
        expansion_iterator = ExpansionMap.find(buff);

        if ( expansion_iterator == ExpansionMap.end() )
        {
            temp = GetFolderLocation( std::wstring( buff ) );
            ::wxLogDebug( L"%hs: GetFolderLocation returned %s = %s" , __FUNCTION__, buff, temp.c_str() );
            if ( temp != L"" )
            {
                //ExpansionMap.insert(std::map<std::wstring,std::wstring>::value_type(std::wstring(buff),temp));
                dest.push_back( temp + std::wstring( nextperc + 1 ) );
            }
        }
        else
            temp =  expansion_iterator->second + std::wstring(nextperc+1);

        return dest.size();

    }


} //unnamed namespace

namespace diskcleaner
{

    TextPlugInfo::TextPlugInfo(std::wstring& aFile) : PlugInfo(), FileName(aFile)
    {
        IconHandle = 0;
        wchar_t strbuff[MAX_PATH+1];
        try
        {

            GetPrivateProfileString( L"Info", L"Title", L"Error: No title given", strbuff,MAX_PATH+1, aFile.c_str() );
            ShortDesc = strbuff;
            //addlog("Reading Info: " + ShortDesc);

            GetPrivateProfileString( L"Info", L"Description", L"Error: No description given", strbuff,MAX_PATH+1, aFile.c_str() );
            LongDesc = strbuff;
            //addlog("Reading description: " + LongDesc);

            //std::wstring IconName = inifile->ReadString("Info","Icon","");
            //addlog("Reading optional Icon name: " + IconName);

//    if(IconName!="")
//        IconHandle = dcstructs::LoadIconShared(ExtractFilePath(FileName)+IconName);


        }
        catch (std::bad_alloc& )
        {
            ShortDesc = FileName + L": Error opening file";

        }

        ItemsFound = 0;
        BytesFound = 0;
        ItemsCleaned = 0;
        BytesCleaned = 0;

    }
//------------------------------------------------------------------------------

    void TextPlugInfo::Scan()
    {
        DoScan(false);
    }
//------------------------------------------------------------------------------

    void TextPlugInfo::Preview()
    {
        DoScan(true);
    }
//------------------------------------------------------------------------------

    void TextPlugInfo::Clean()
    {



        wchar_t buff[32*1024-1]; //32k is limit of win95
        wchar_t* pstring = buff;
        wchar_t* subkey,*value;
        HKEY rootkey;

        FileList.clear();
        GetPrivateProfileSection(L"registry",buff,sizeof(buff),FileName.c_str());

        ItemsCleaned = 0;
        BytesCleaned = 0;


        while (*pstring)
        {
            int len = lstrlen(pstring);
            if (CrackRegKey(pstring,rootkey,subkey,value))
            {
                ::wxLogDebug(L"%s %hs: DelRegKey(%s,%s)", FileName.c_str(), __FUNCTION__, subkey, value);
                DelRegKey( rootkey, subkey, value,ItemsCleaned,BytesCleaned, FileList,pstring);
            }

            pstring+=len+1;
        }

        /* Do files */

        for (unsigned int i=0;i<FolderList.size();++i)
        {
            std::wstring& dummy = FolderList[i];
            wchar_t folder[MAX_PATH+1]={0};

            ::wxLogDebug(L"%s TextPlugInfo->Clean(): Cleaning %s", FileName.c_str(), dummy.c_str() );

            lstrcpy(folder,dummy.c_str());
            TScanOptions scanopt = GetScanOptions(folder);

            scanopt.Secure = SecureRemove;

            wchar_t* lastbkslash = wcsrchr(folder,'\\');
            lastbkslash++;

            std::wstring file(lastbkslash); //Get file from c-std::wstring
            *lastbkslash = 0; //Truncate folder after last backslash

            //file = file.Trim();

            ::wxLogDebug(L"%s TextPlugInfo->Clean(): CleanFilesInFolder(%s,%s)", FileName.c_str(), folder, file.c_str() );
            DSdata data = CleanFilesInFolder(folder,file.c_str(),&scanopt,FileList);

//            if (scanopt.DelBaseFolder)
//            {
//                ::wxLogDebug(L"%s TextPlugInfo->Clean(): RemoveDirectory %s", FileName.c_str() , folder);
//                RemoveDirectory(folder);
//            }

            BytesCleaned += data.bytes;
            ItemsCleaned += data.files;

        }

        /*****************/


    }
//------------------------------------------------------------------------------

    HICON TextPlugInfo::GetIcon()
    {
        return IconHandle;
    }

//------------------------------------------------------------------------------
    TScanOptions TextPlugInfo::GetScanOptions(wchar_t* folder)
    {
        wxLogDebug( L"%hs( %s )", __FUNCTION__, folder);

        TScanOptions so;


        wchar_t* slash = wcschr(folder,L'/');

        while (slash)
        {
            *slash=0;
            ++slash;
            switch (*slash)
            {
            case L'S':  //build 185
            case L's':
                wxLogDebug( L"so.Recursive = true" );
                ++slash;
                switch (*slash) // > build 186
                {
                case L'o':
                case L'O':
                    wxLogDebug( L"so.SubFolderOnly = true" );
                    so.SubFolderOnly = true;
                    break;

                case L'I':   // > build 186
                case L'i':
                    wxLogDebug( L"so.DelBaseFolder = true" );
                    so.DelBaseFolder = true;
                    break;
                }

                so.Recursive = true;
                break;


                /*
                case 'h':
                case 'H':  so.Hidden = true;
                           break;

                case 'r':
                case 'R':  so.ReadOnly = true;
                           break; */
            }
            slash = wcschr(slash,'/'); //find next
        }

        return so;
    }
//---------------------------------------------------------------------------

    void TextPlugInfo::DoScan( bool Preview )
    {
        wchar_t buff[32*1024-1]; //32k is limit of win95
        wchar_t* pstring = buff;
        wchar_t* subkey,*value;
        HKEY rootkey;


        FileList.clear();

        /* Do registry first */
        //addlog("Reading registry section");
        GetPrivateProfileSection( L"registry" , buff, sizeof( buff ), FileName.c_str() );

        ItemsFound = 0;
        BytesFound = 0;

        while ( *pstring )
        {

            int len = lstrlen(pstring);

            ::wxLogDebug( L"%hs: cracking %s", __FUNCTION__, pstring );
            if (CrackRegKey(pstring,rootkey,subkey,value))
            {
                ::wxLogDebug( L"%hs: EnumRegKey(%s, %s, %s)" , __FUNCTION__, pstring, subkey, value );
                EnumRegKey( rootkey, subkey, value, ItemsFound, BytesFound, FileList, pstring);
            }

            pstring+=len+1;

        }
        /*********************/


        /* Do files */
        //addlog("Reading files section");
        GetPrivateProfileSectionW( L"files", buff, sizeof( buff ), FileName.c_str() );
        pstring = buff;

        FolderList.clear();


        while ( *pstring )
        {
            wxLogDebug( L"%hs: found in [files]: %s", __FUNCTION__, pstring );
            std::wstring tmp(pstring);

            ExpandString(tmp, FolderList);

            pstring += lstrlen(pstring) + 1;
        }

        if ( FolderList.size() )
            for (unsigned int i=0;i<FolderList.size();++i)
            {
                //wxLogDebug(L"TextPlugInfo->DoScan(%d): Scanning %s" , Preview, FolderList[i].c_str() );

                wchar_t folder[ MAX_PATH + 1 ] = { 0 };
                wchar_t file_or_mask[ MAX_PATH + 1 ] = { 0 };

                lstrcpy( folder, FolderList[ i ].c_str() );
                TScanOptions scanopt = GetScanOptions( folder );

                wchar_t* lastbkslash = wcsrchr( folder, '\\' );
                lastbkslash++;

                lstrcpy( file_or_mask, lastbkslash );

                *lastbkslash = 0; //Truncate folder after last backslash

                DSdata data = GetFilesInFolder(folder, file_or_mask, &scanopt, FileList );

                BytesFound += data.bytes;
                ItemsFound += data.files;

            }

        /*****************/
    }


    void TextPlugInfo::GetFilesAsStrings(std::vector<std::wstring>& Messages)
    {
        unsigned int count = FileList.size();
        for (unsigned int k = 0;k < count;++k)
        {
            Messages.push_back(FileList[k]);
        }

    }



} //namespace


