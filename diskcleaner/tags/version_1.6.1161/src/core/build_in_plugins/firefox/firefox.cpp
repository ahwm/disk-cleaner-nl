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
    wchar_t firefox_base::cachefolder[MAX_PATH] = L"\0";
    wchar_t firefox_base::localcachefolder[MAX_PATH] = L"\0";

    bool firefox_base::Initialize()
    {
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
            return false;

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
            return false;


        int IsRelative = GetPrivateProfileInt( L"Profile0", L"IsRelative", -1, theinifile );
        ::wxLogDebug( L"%hs: IsRelative = %d", __PRETTY_FUNCTION__, IsRelative );

        if ( IsRelative == -1 )
            return false;


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

        return true;

    }


    void firefox_base::SetIcon(int ICON_ID)
    {

//  IconHandle = (HICON) CopyImage(
//                LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(ICON_ID)),
//                IMAGE_ICON,16,16, LR_COPYFROMRESOURCE|LR_COPYDELETEORG);
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
        wchar_t cleanfolder[MAX_PATH];

        if ( *cachefolder ) //dereference item 0
            //checks for valid string
        {
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
        wchar_t cleanfile[MAX_PATH];

        if ( *cachefolder ) //dereference item 0
                            //checks for valid string
        {
            while ( *files )
            {
                lstrcpy( cleanfile, cachefolder );
                lstrcat( cleanfile, files );
                files += lstrlen( files ); //files points to next file or terminating '\0'
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
                        wxLogWarning( _( "%s (%s)" ), warning, cleanfile );
                    }
                }
            }

        }
    }
//---------------------------------------------------------------------------

    firefox_cache::firefox_cache()
    {
        ShortDesc = _( "Firefox Cache" );
        LongDesc = _( "Clear the Firefox cache" );
//  SetTCIIcon( this, ID_FFCACHE );
    }

    void firefox_cache::Clean()
    {
        wchar_t cleanfolder[MAX_PATH];

        if ( *cachefolder ) //dereference item 0
            //checks for valid string
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

        wchar_t cleanfolder[MAX_PATH];

        if ( *cachefolder ) //dereference item 0
            //checks for valid string
        {
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
//  SetTCIIcon(this,ID_FFHIST);
    }

    void firefox_history::DoScan(bool Preview)
    {
        ScanFile( L"History.dat\0places.sqlite\0" );
    }

    void firefox_history::Clean()
    {
        CleanFile( L"History.dat\0places.sqlite\0", L"Unable to delete firefox history");
    }

//---------------------------------------------------------------------------
    firefox_cookies::firefox_cookies()
    {
        ShortDesc = _( "Firefox Cookies" );
        LongDesc = _( "Remove Firefox cookies" );
//  SetIcon(ID_FFCOOKIES);
    }

    void firefox_cookies::DoScan(bool Preview)
    {
        ScanFile( L"cookies.txt\0cookies.sqlite\0" );
    }


    void firefox_cookies::Clean()
    {
        CleanFile( L"cookies.txt\0cookies.sqlite\0", L"Unable to delete firefox cookies");
    }
//---------------------------------------------------------------------------

}

