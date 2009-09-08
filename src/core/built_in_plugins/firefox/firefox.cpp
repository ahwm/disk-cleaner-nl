#include <windows.h>
#include <string>
#include <shlobj.h>

#include "core\disk scan\DiskScan.h"
#include "firefox.h"
//#include "ff_icon.h"
#include <wx/log.h>
#include <wx/intl.h>

namespace diskcleaner
{

    bool firefox_base::IsInitialized = false;
    bool firefox_base::FFPresent = false;
    wchar_t firefox_base::cachefolder[MAX_PATH] = L"\0";
    wchar_t firefox_base::localcachefolder[MAX_PATH] = L"\0";

    void firefox_base::Initialize()
    {
        //whatever happens, we don't need to try again
        IsInitialized = true;

        wchar_t theinifile[MAX_PATH];
        wchar_t theFFfolder[MAX_PATH];

        LPITEMIDLIST pidl;


        if ( SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl ) == S_OK )
        {
            ::wxLogDebug( L"%hs: SHGetSpecialFolderLocation(CSIDL_APPDATA) == S_OK", __PRETTY_FUNCTION__ );

            SHGetPathFromIDList( pidl, theFFfolder );
            CoTaskMemFree( pidl );

            ::wxLogDebug( L"%hs: SHGetPathFromIDList(pidl) = %s", __PRETTY_FUNCTION__, theFFfolder );
        }
        else
        {
            ::wxLogDebug( L"%hs: SHGetSpecialFolderLocation(CSIDL_APPDATA) != S_OK", __PRETTY_FUNCTION__ );

        }

        if ( SHGetSpecialFolderLocation( NULL, CSIDL_LOCAL_APPDATA, &pidl ) == S_OK )
        {
            ::wxLogDebug( L"%hs: SHGetSpecialFolderLocation(CSIDL_LOCAL_APPDATA) == S_OK", __PRETTY_FUNCTION__ );

            SHGetPathFromIDList( pidl, localcachefolder );
            CoTaskMemFree( pidl );

            ::wxLogDebug( L"%hs: SHGetPathFromIDList(pidl) = %s", __PRETTY_FUNCTION__, localcachefolder );
        }



        lstrcat( theFFfolder, L"\\Mozilla\\Firefox\\" );
        ::wxLogDebug( L"%hs: theFFfolder = %s", __PRETTY_FUNCTION__, theFFfolder );

        if ( *localcachefolder )
        {
            lstrcat( localcachefolder, L"\\Mozilla\\Firefox\\" );
            ::wxLogDebug( L"%hs: localcachefolder = %s", __PRETTY_FUNCTION__, localcachefolder );
        }

        lstrcpy( theinifile, theFFfolder );
        lstrcat( theinifile, L"profiles.ini" );

        //read profiles.ini
        GetPrivateProfileString( L"Profile0", L"Path", L"-1", cachefolder, MAX_PATH, theinifile );
        ::wxLogDebug( L"%hs: cachefolder = %s", __PRETTY_FUNCTION__, cachefolder );

        if ( lstrcmp( cachefolder, L"-1" ) == 0)
        {
            //Firefox ini file not found, FF not installed(?)
            ::wxLogDebug( L"%hs: FF's profiles.ini not found", __PRETTY_FUNCTION__ );

            return;
        }


        int IsRelative = GetPrivateProfileInt( L"Profile0", L"IsRelative", -1, theinifile );
        ::wxLogDebug( L"%hs: IsRelative = %d", __PRETTY_FUNCTION__, IsRelative );

        if ( IsRelative == -1 )
        {
            ::wxLogDebug( L"%hs: Cannot determine if cache folder is relative path or absolute path. Stopping.",
                          __PRETTY_FUNCTION__ );

            return;
        }


        //Convert forward slashes to backward slashes
        wchar_t * p = cachefolder;

        while ( *p )
        {
            if ( *p == '/' ) *p = '\\';
            ++p;
        }

        if ( IsRelative == 1 ) //Add profile location to theFFfolder
        {
            lstrcat( theFFfolder, cachefolder );
            ::wxLogDebug( L"%hs: IsRelative = %d, theFFfolder = %s", __PRETTY_FUNCTION__, IsRelative, theFFfolder );

            if ( *localcachefolder ) lstrcat( localcachefolder, cachefolder );
            lstrcpy( cachefolder, theFFfolder );
        }

        FFPresent = true;

        return;

    }


    void firefox_base::GetFilesAsStrings(std::vector<std::wstring>& Messages)
    {
        unsigned int count = ItemList.size();
        Messages.reserve(count);

        for (unsigned int k = 0;k < count;++k)
        {
            Messages.push_back(ItemList[k]);
        }

    }

    void firefox_base::ScanFile( const wchar_t* files )
    {
        ItemList.clear();

        if ( FFPresent ) //then also variable cachefolder will be valid
        {
            wchar_t cleanfolder[MAX_PATH];

            lstrcpy( cleanfolder, cachefolder );
            lstrcat( cleanfolder, L"\\" );
            ::wxLogDebug( L"%hs: cleanfolder = %s%s", __PRETTY_FUNCTION__, cleanfolder, files );

            TScanOptions so;
            //so.Recursive = false; ->Now all initialized to false
            so.Hidden = true;

            DSdata ds = GetFilesInFolder( cleanfolder, files, &so, ItemList );

            ItemsFound = ds.files;
            BytesFound = ds.bytes;

            wxLogDebug( L"%hs: ItemsFound = %I64d", __PRETTY_FUNCTION__, ItemsFound );

        }
    }

    void firefox_base::CleanFile( const wchar_t* files, const wchar_t* warning )
    {

        if ( FFPresent ) //then variable cachefolder is valid as well
        {
            wchar_t cleanfile[MAX_PATH] = { 0 };
            wchar_t* ptr(NULL);

            lstrcpy( cleanfile, cachefolder );
            lstrcat( cleanfile, L"\\" );

            // Find last backslash
            ptr = std::wcsrchr( cleanfile, '\\' );

            // Set ptr to first character after last backslash
            ++ptr;

            while ( *files )
            {
                // Make sure 'cleanfile' is NULL terminated
                *ptr = 0;

                // append a file name
                lstrcat( cleanfile, files );
                wxLogDebug( L"%hs: file to remove is: %s", __FUNCTION__, cleanfile );

                DWORD file_attr = GetFileAttributes( cleanfile );

                if ( file_attr != INVALID_FILE_ATTRIBUTES )
                {
                    if ( DeleteFile( cleanfile ) )
                    {
                        ::wxLogDebug( L"%hs: firefox cookies (%s) succesfully deleted", __PRETTY_FUNCTION__, cleanfile );
                        BytesCleaned = BytesFound;
                        ItemsCleaned = ItemsFound;
                    }
                    else
                    {
                        wxLogWarning( L"%s (%s)", warning, cleanfile );
                    }
                }

                // let 'files' point to next file or terminating '\0'
                files += lstrlen( files ) + 1;
            }

        }
    }
//---------------------------------------------------------------------------

    firefox_cache::firefox_cache()
    {
        ShortDesc = _( "Firefox Cache" );
        LongDesc = _( "Clear the Firefox cache" );
    }

    void firefox_cache::Clean()
    {
        wchar_t cleanfolder[MAX_PATH];

        if ( FFPresent ) //Should never be called if !FFPresent, but still,
            //better check
        {
            TScanOptions so;
            so.ReadOnly = true;
            so.Hidden = true;
            so.Recursive = true;

            lstrcpy( cleanfolder, cachefolder );
            lstrcat( cleanfolder, L"\\Cache\\" );

            ::wxLogDebug( L"%hs: cachefolder != 0, clean folder %s\\*.*", __PRETTY_FUNCTION__, cleanfolder );

            DSdata ds = CleanFilesInFolder( cleanfolder, L"*.*\0", &so, ItemList );
            DSdata ds2 = { 0 }; //Adding 0 does not change ItemsCleaned or BytesCleaned below.

            if ( *localcachefolder )
            {
                lstrcpy( cleanfolder, localcachefolder );
                lstrcat( cleanfolder, L"\\Cache\\" );
                ::wxLogDebug( L"%hs: localcachefolder != 0, clean folder %s\\*.*", __PRETTY_FUNCTION__, cleanfolder );
                ds2 = CleanFilesInFolder( cleanfolder, L"*.*\0", &so, ItemList );
            }

            ItemsCleaned= ds.files + ds2.files;
            BytesCleaned = ds.bytes + ds2.bytes;
        }
    }

    void firefox_cache::DoScan(bool Preview)
    {
        ItemList.clear();

        if ( FFPresent ) //then variable cachfolder is valid as well
        {
            wchar_t cleanfolder[MAX_PATH];

            lstrcpy( cleanfolder, cachefolder );
            lstrcat( cleanfolder, L"\\Cache\\" );
            ::wxLogDebug( L"%hs: cleanfolder = %s", __PRETTY_FUNCTION__, cleanfolder );

            TScanOptions so;
            so.Recursive = true;

            //extra "\0" is paramount in L"*.*\0"!
            DSdata ds = GetFilesInFolder( cleanfolder, L"*.*\0", &so, ItemList );
            DSdata ds2 = { 0 };

            if ( *localcachefolder )
            {
                ::wxLogDebug( L"%hs: localcachefolder != 0", __PRETTY_FUNCTION__ );

                lstrcpy( cleanfolder, localcachefolder );
                lstrcat( cleanfolder, L"\\Cache\\" );

                ::wxLogDebug( L"%hs: cleanfolder = %s", __PRETTY_FUNCTION__, cleanfolder );
                ds2 = GetFilesInFolder( cleanfolder, L"*.*\0", &so, ItemList );
            }

            ItemsFound = ds.files + ds2.files;
            BytesFound = ds.bytes + ds2.bytes;

        }


    }
//---------------------------------------------------------------------------

    firefox_history::firefox_history()
    {
        ShortDesc = _( "Firefox History" );
        LongDesc = _( "Remove Firefox history records" );
    }

    void firefox_history::DoScan(bool Preview)
    {
        //Checks for FFPresent
        ScanFile( L"History.dat\0places.sqlite\0" );

    }

    void firefox_history::Clean()
    {
        //Checks for FFPresent
        CleanFile( L"History.dat\0places.sqlite\0", _( "Unable to delete firefox history" ) );
    }

//---------------------------------------------------------------------------
    firefox_cookies::firefox_cookies()
    {
        ShortDesc = _( "Firefox Cookies" );
        LongDesc = _( "Remove Firefox cookies" );
    }

    void firefox_cookies::DoScan(bool Preview)
    {
        //Checks for FFPresent
        ScanFile( L"cookies.txt\0cookies.sqlite\0" );
    }


    void firefox_cookies::Clean()
    {
        //Checks for FFPresent
        CleanFile( L"cookies.txt\0cookies.sqlite\0", L"Unable to delete firefox cookies");

    }
//---------------------------------------------------------------------------

}

