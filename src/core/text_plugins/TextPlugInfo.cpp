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
#include <cctype>
#include <algorithm>

#include <new>
#include "../reg scan/regenum.h"

#include "wx/log.h"
#include "wx/intl.h"

namespace
{
typedef GUID KNOWNFOLDERID;
#ifdef __cplusplus
#define REFKNOWNFOLDERID const KNOWNFOLDERID &
#else // !__cplusplus
#define REFKNOWNFOLDERID const KNOWNFOLDERID * __MIDL_CONST
#endif // __cplusplus


    typedef HRESULT (WINAPI* nsGetKnownFolderPath)(REFKNOWNFOLDERID rfid,
            DWORD dwFlags,
            HANDLE hToken,
            PWSTR *path);
    typedef BOOL (WINAPI* nsIsWow64Process)( HANDLE hProcess, bool* Wow64Process );


    static nsGetKnownFolderPath gGetKnownFolderPath = NULL;
    static nsIsWow64Process gIsWow64Process = NULL;
    static HINSTANCE gKernel32DLLInst = NULL;
    static HINSTANCE gShell32DLLInst = NULL;

    static bool is64bit = false;
    std::wstring GetFolderLocation(const std::wstring& folder)
    {
        int nFolder=0;
        wchar_t buff[MAX_PATH];

        ::wxLogDebug( L"%hs: find %s", __FUNCTION__, folder.c_str() );
        std::wstring folder_copy( folder );

        // Ensure the string is in lowercase before comparison
        std::transform( folder_copy.begin(), folder_copy.end(), folder_copy.begin(), tolower );


        if ( folder_copy.find(L"%windir%") != std::string::npos )  //all
        {

            wxLogDebug( L"found %%windir%%" );
            GetWindowsDirectory(buff,MAX_PATH);

            wxLogDebug( L"%%windir%% = %s", buff );
            return std::wstring(buff);
        }
        if ( folder_copy.find(L"%sysdir%") != std::string::npos ) //all
        {

            wxLogDebug( L"found %%sysdir%%" );
            GetSystemDirectory(buff,MAX_PATH);

            wxLogDebug( L"%%sysdir%% = %s", buff );
            return std::wstring(buff);
        }


        if ( folder_copy.find(L"%appdata%") != std::string::npos ) //4.71
        {
            wxLogDebug( L"found %%appdata%%" );
            nFolder = CSIDL_APPDATA;
        }
        else
            if ( folder_copy.find(L"%programfiles%") != std::string::npos ) //5.0, but look in reg. otherwise
            {
                wxLogDebug( L"found %%programfiles%%" );
                nFolder = CSIDL_PROGRAM_FILES;
            }
            else
                if ( folder_copy.find(L"%desktop%") != std::string::npos )
                {
                    wxLogDebug( L"found %%desktop%%" );
                    nFolder = CSIDL_DESKTOPDIRECTORY;
                }
                else
                    if ( folder_copy.find(L"%commonappdata%") != std::string::npos ) //5.0
                    {
                        wxLogDebug( L"found %%commonappdata%%" );
                        nFolder = CSIDL_COMMON_APPDATA;
                    }
        if ( folder_copy.find(L"%commondesktop%") != std::string::npos )
        {
            wxLogDebug( L"found %%commondesktop%%" );
            nFolder = CSIDL_COMMON_DESKTOPDIRECTORY;
        }
        if ( folder_copy.find(L"%commondocs%") != std::string::npos )
        {
            wxLogDebug( L"found %%commondocs%%" );
            nFolder = CSIDL_COMMON_DOCUMENTS;  //C:\Documents and Settings\All Users\Documents
        }
        if ( folder_copy.find(L"%commonprogramfiles%") != std::string::npos ) //but look in reg. otherwise
        {
            wxLogDebug( L"found %%commonprogramfiles%%" );
            nFolder = CSIDL_PROGRAM_FILES_COMMON;
        }
        if ( folder_copy.find(L"%localappdata%") != std::string::npos ) //5.0
        {
            wxLogDebug( L"found %%localappdata%%" );
            nFolder = CSIDL_LOCAL_APPDATA;
        }

        if (nFolder)
        {
            if (SHGetFolderPath(NULL, nFolder, NULL, 0, buff) == S_OK)
            {
                ::wxLogDebug( L"SHGetFolderPath returns: %s", buff );
                return std::wstring(buff);
            }
            else
            {
                ::wxLogDebug( L"SHGetFolderPath != S_OK" );
            }
        }

        if ( folder_copy.find(L"%localappdatalow%") != std::string::npos ) //6.0
        {
            wxLogDebug( L"found %%localappdatalow%%" );
            if ( gGetKnownFolderPath )
            {
                const GUID LocalAppDataLow = {0xA520A1A4L, 0x1780, 0x4FF6, {0xBD, 0x18, 0x16, 0x73, 0x43, 0xC5, 0xAF, 0x16 } };
                std::wstring path;
                PWSTR pszPath;
                HRESULT error = gGetKnownFolderPath( LocalAppDataLow, 0, NULL, &pszPath );
                if ( error == S_OK )
                {
                    ::wxLogDebug( L"gGetKnownFolderPath == S_OK" );
                    ::wxLogDebug( L"gGetKnownFolderPath returns: %s", pszPath );
                    path = pszPath;
                    CoTaskMemFree( ( LPVOID ) pszPath );
                    return path;
                }
            }
        }

        // return empty string
        return std::wstring();

    }

    /// Test for writing to or deleting locations that are protected in Vista.
    /// A standard user is denied access to these locations by default
    /// and admin rights are needed instead.
    // Note that we create a copy of path *on purpose*
    bool TestForAdminLocations( std::wstring path )
    {
        std::transform(path.begin(), path.end(), path.begin(),
               (int(*)(int)) std::tolower);

        if ( path.find( L"%windir%") != std::string::npos ||
             path.find( L"%sysdir%") != std::string::npos ||
             path.find( L"%programfiles%" ) != std::string::npos ||
             path.find( L"%commonappdata%") != std::string::npos ) // needs special permissions in Vista?
        {
            return true;
        }
        return false;
    }

/// ExpandString - Expands strings in the form of %string% to their full size.
/// Currently only capable of 1 expansion per string.
/// adds the expanded strings to dest and puts values that were not in the
/// environment variables there.
//--------------------------------------------------------------------------
    int ExpandString(const std::wstring& source,std::vector<std::wstring>& dest)
    {
        ::wxLogDebug( L"ExpandString(%s)", source.c_str() );

        wchar_t buff[MAX_PATH];
        wchar_t* pstring = buff;
        std::wstring temp;

        // On windows 64 bit, the %programfiles% environment variable points to
        // %programfiles(x86)%, but we want to search the 64 bit binaries
        // folder as well.

        if ( is64bit )
        {
            temp = source;
            std::transform(temp.begin(), temp.end(), temp.begin(), tolower );
            // Check for %programfiles% string
            unsigned int find_result = temp.find( L"%programfiles%");

            if ( find_result != std::string::npos )
            {
               // Replace the %programfiles% string with an explicit 64 bit
               // %programw6432%
               temp.replace( find_result, 14, L"%programw6432%");

               // Let windows take care of the environment variable expansion
                int returnval = ExpandEnvironmentStrings(temp.c_str(),buff,MAX_PATH);
                if ( returnval && *buff != '%' )
                {
                    ::wxLogDebug( L"64 bit: ExpandEnvironmentString(temp) = %s", buff);
                    dest.push_back(buff);
                }
            }

            // Check for %commonprogramfiles% string
            find_result = temp.find( L"%commonprogramfiles%");

            if ( find_result != std::string::npos )
            {
               // Specify the explitcit 64 bit location
               temp.replace( find_result, 18, L"%commonprogramw6432%");

               // Let windows take care of the environment variable expansion
                int returnval = ExpandEnvironmentStrings(temp.c_str(),buff,MAX_PATH);
                if ( returnval && *buff != '%' )
                {
                    ::wxLogDebug( L"64 bit: ExpandEnvironmentString(temp) = %s", buff);
                    dest.push_back(buff);
                }
            }

        }

        // Let windows take care of the environment variable expansion
        int returnval = ExpandEnvironmentStrings(source.c_str(),buff,MAX_PATH);

        if ( returnval && *buff != '%' )
        {
            ::wxLogDebug( L"ExpandEnvironmentString(source) = %s", buff);
            dest.push_back(buff);
            return 1;
        }

        // ExpandEnvironmentStrings didn't succeed in expanding the variable
        // Get the value of the variable ourself

        unsigned int nextperc = source.find( L"%", 1 );

        if ( nextperc == std::string::npos ) return 0;

        // Copy %string% to a new std::wstring instance
        std::wstring substring = source.substr( 0, nextperc + 1 );

        ::wxLogDebug( L"%hs: Expand %s" , __FUNCTION__, substring.c_str());
        if ( substring == L"%drive%" )
        {
            ::wxLogDebug( L"Expanding %%drive%% construct" );

            GetLogicalDriveStrings(MAX_PATH,buff);
            while ( *pstring )
            {
                if ( GetDriveType(pstring) == DRIVE_FIXED )
                {
                    ::wxLogDebug( L"Reported as fixed drive: %s", pstring );
                    dest.push_back( std::wstring( pstring ) + source.substr( nextperc + 1, std::string::npos ) );
                }

                pstring += 4;
            }
            //return the number of fixed drives
            return  dest.size();
        }

        temp = GetFolderLocation( substring );
        ::wxLogDebug( L"%hs: GetFolderLocation returned %s = %s" , __FUNCTION__, substring.c_str(), temp.c_str() );

        if ( temp != L"" )
        {
            // Add the found folder location to the Enviroment Variables
            // Windows will take care of the substitution next time :)
            wxLogDebug( L"Enviroment variable: %s", buff + 1 );

            SetEnvironmentVariable( substring.substr( 1, substring.length() - 2 ).c_str(), temp.c_str() );
            wxLogDebug( L"Adding environment variable: %s", substring.substr( 1, substring.length() - 2 ).c_str() );

            // Try again after having added the variable
            returnval = ExpandEnvironmentStrings(source.c_str(), buff, MAX_PATH);

            if ( returnval && *buff != '%' )
            {
                ::wxLogDebug( L"ExpandEnvironmentString(source) = %s", buff);
                dest.push_back(buff);
            }
        }

        return dest.size();
    }


} //unnamed namespace

namespace diskcleaner
{
    /// Create a new TextPlugInfo instance from a file name (including path)
    TextPlugInfo::TextPlugInfo(std::wstring& aFile) : PlugInfo(), FileName(aFile)
    {
        IconHandle = 0;
        wchar_t strbuff[MAX_PATH+1];
        try
        {

            GetPrivateProfileString( L"Info", L"Title", L"Error: No title given", strbuff,MAX_PATH+1, aFile.c_str() );
            ShortDesc = wxGetTranslation( strbuff );

            GetPrivateProfileString( L"Info", L"Description", L"Error: No description given", strbuff,MAX_PATH+1, aFile.c_str() );
            LongDesc = wxGetTranslation( strbuff );

            //Process to check if running, see processes_dlg
            GetPrivateProfileString( L"Info", L"Process", L"", strbuff,MAX_PATH+1, aFile.c_str() );
            process = strbuff;

            wxLogDebug( L"Process = %s", strbuff );

            if( !process.empty() )
            {
                GetPrivateProfileString( L"Info", L"Application", L"Unknown", strbuff,MAX_PATH+1, aFile.c_str() );
                wxLogDebug( L"Application = %s", strbuff );
                process_pretty_name = strbuff;
            }

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

        HKEY rootkey;
        BOOL fOk;
        WIN32_FILE_ATTRIBUTE_DATA fileInfo;

        ItemsCleaned = 0;
        BytesCleaned = 0;

        fOk = GetFileAttributesEx(FileName.c_str(), GetFileExInfoStandard, (void*)&fileInfo);
        if (!fOk)
        {
            wxLogError( _T("Unable to determine file size. Skipping plugin %s"), FileName.c_str() );

            return;
        }

        if ( fileInfo.nFileSizeHigh != 0)
        {
            wxLogError( _T("Extremely large plug-in file detected: %s. Skipping."), FileName.c_str() );

            return;
        }

        wxLogDebug( L"%s: plugin file size is %d", FileName.c_str(), fileInfo.nFileSizeLow );

        wchar_t* buff = new wchar_t[ fileInfo.nFileSizeLow ];
        wchar_t* pstring = buff;
        wchar_t* subkey,*value;

        FileList.clear();
        GetPrivateProfileSection(L"registry",buff, fileInfo.nFileSizeLow, FileName.c_str());

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

        // Do files
        //
        // No need to read the [files] section anymore from the plug-in
        // since it is stored in the vector<std::wstring> FolderList

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
            diskscan_data data = CleanFilesInFolder(folder,file.c_str(),&scanopt,FileList);

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
                case L'F':   // > build 1.6.1273
                case L'f':
                    wxLogDebug( L"so.FilesOnly = true" );
                    so.FilesOnly = true;
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
        HKEY rootkey;
        BOOL fOk;
        WIN32_FILE_ATTRIBUTE_DATA fileInfo;

        ItemsFound = 0;
        BytesFound = 0;

        fOk = GetFileAttributesEx(FileName.c_str(), GetFileExInfoStandard, (void*)&fileInfo);
        if (!fOk)
        {
            wxLogError( _T("Unable to determine file size. Skipping plugin %s"), FileName.c_str() );

            return;
        }

        if ( fileInfo.nFileSizeHigh != 0)
        {
            wxLogError( _T("Extremely large plug-in file detected: %s. Skipping."), FileName.c_str() );

            return;
        }

        wxLogDebug( L"%s: plugin file size is %d", FileName.c_str(), fileInfo.nFileSizeLow );
        wchar_t* buff = new wchar_t[ fileInfo.nFileSizeLow ];
        wchar_t* pstring = buff;
        wchar_t* subkey,*value;

        FileList.clear();

        /* Do registry first */
        wxLogDebug( L"%s: Processing registry section", FileName.c_str() );

        GetPrivateProfileSection( L"registry" , buff, fileInfo.nFileSizeLow, FileName.c_str() );

        while ( *pstring )
        {

            int len = lstrlen(pstring);

            ::wxLogDebug( L"%hs: cracking %s", __FUNCTION__, pstring );
            if (CrackRegKey(pstring,rootkey,subkey,value))
            {
                ::wxLogDebug( L"%hs: EnumRegKey(%s, %s, %s)" , __FUNCTION__, pstring, subkey, value );

                if (rootkey != HKEY_CURRENT_USER ) // Assume admin priviliges are required when not editing our own
                {                                  // (HKEY_CURRENT_USER) keys in the registry
                    AdminRequired = true;
                }
                EnumRegKey( rootkey, subkey, value, ItemsFound, BytesFound, FileList, pstring);
            }

            pstring+=len+1;

        }
        /*********************/


        /* Do files */
        wxLogDebug( L"%s: Processing files section", FileName.c_str() );

        GetPrivateProfileSectionW( L"files", buff, fileInfo.nFileSizeLow, FileName.c_str() );
        pstring = buff;

        FolderList.clear();


        while ( *pstring )
        {
            wxLogDebug( L"%hs: found in [files]: %s", __FUNCTION__, pstring );
            std::wstring tmp( pstring );
            if( !AdminRequired ) AdminRequired = TestForAdminLocations( tmp );
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

                diskscan_data data = GetFilesInFolder(folder, file_or_mask, &scanopt, FileList );

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


    bool InitializeSHGetKnownFolderPath()
    {
        //We always load Shell32.dll
        gShell32DLLInst = GetModuleHandle( L"Shell32.dll" );
        if (gShell32DLLInst)
        {

            gGetKnownFolderPath = (nsGetKnownFolderPath)GetProcAddress(gShell32DLLInst, "SHGetKnownFolderPath");
            if ( gGetKnownFolderPath )
            {
                ::wxLogDebug( L"InitializeSHGetKnownFolderPath != NULL" );

            }

        }

        return false;
    }

    bool UninitializeSHGetKnownFolderPath()
    {
        ::wxLogDebug( L"%hs: setting variables to NULL", __FUNCTION__ );
        gGetKnownFolderPath = NULL;
        gShell32DLLInst = NULL;

        return true;
    }

    bool InitializeIsWow64Process()
    {
        //We always load Kernel32.dll
        gKernel32DLLInst = GetModuleHandle( L"Kernel32.dll" );
        if ( gKernel32DLLInst )
        {

            gIsWow64Process = (nsIsWow64Process) GetProcAddress(gKernel32DLLInst, "IsWow64Process");
            if ( gIsWow64Process )
            {
                ::wxLogDebug( L"IsWow64Process != NULL" );

                gIsWow64Process( GetCurrentProcess(), &is64bit );

                ::wxLogDebug( L" is64bit = %d", is64bit );

                return true;
            }

        }

        return false;
    }

     bool UninitializeIsWow64Process()
    {
        ::wxLogDebug( L"%hs: setting variables to NULL", __FUNCTION__ );
        gIsWow64Process = NULL;
        gKernel32DLLInst = NULL;

        return true;
    }


} //namespace


