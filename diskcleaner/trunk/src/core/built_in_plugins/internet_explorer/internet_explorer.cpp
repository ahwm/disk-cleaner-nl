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

#include <string>
#include <shlobj.h>

#include "internet_explorer.h"
#include <wx/log.h>
#include <wx/intl.h>

#define ALL_CACHE_ENTRY   0xFFFFFFFF


#ifdef __GNUC__
#include "mingw_urlhist.h"
#else
#include "urlhist.h"
#endif

namespace diskcleaner
{

    bool ie_base::EnumerateCache( __int64& Group_ID )
    {
        DWORD dwEntrySize;
        LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
        HANDLE hCacheDir;
        dwEntrySize = 4096; //INIT_CACHE_ENTRY_SIZE;
        lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
        lpCacheEntry->dwStructSize = dwEntrySize;

again:
        hCacheDir = FindFirstUrlCacheEntryEx( NULL,0,ALL_CACHE_ENTRY,
                                              Group_ID,lpCacheEntry,&dwEntrySize,NULL,NULL,NULL );
        if (!hCacheDir)
        {
            delete[] lpCacheEntry;
            switch ( GetLastError() )
            {
            case ERROR_NO_MORE_ITEMS:
                FindCloseUrlCache( hCacheDir);
                return true;

            case ERROR_INSUFFICIENT_BUFFER:
                lpCacheEntry = ( LPINTERNET_CACHE_ENTRY_INFO ) new char[ dwEntrySize ];
                lpCacheEntry->dwStructSize = dwEntrySize;
                goto again;

            default:
                FindCloseUrlCache( hCacheDir );
                return false;
            }
        }
        //Found an item..
        if ( lpCacheEntry->CacheEntryType & cache_type )
        {
            //URLHISTORY_CACHE_ENTRY and  COOKIE_CACHE_ENTRY entries are also
            //NORMAL_CACHE_ENTRY entries. We need to separate these.
            if ( ( cache_type & ( URLHISTORY_CACHE_ENTRY | COOKIE_CACHE_ENTRY ) ) ||
                   ( cache_type == NORMAL_CACHE_ENTRY && !( lpCacheEntry->CacheEntryType & ( URLHISTORY_CACHE_ENTRY | COOKIE_CACHE_ENTRY ) ) ) )
            {
                ItemsFound++;
                BytesFound += lpCacheEntry->dwSizeHigh * MAXDWORD +
                              lpCacheEntry->dwSizeLow;
                ItemList.push_back( lpCacheEntry->lpszSourceUrlName );
                ::wxLogDebug( L"%hs: Found %s", __FUNCTION__ , lpCacheEntry->lpszSourceUrlName );
            }
        }
        delete (lpCacheEntry);

        do
        {
            dwEntrySize = 4096;
            lpCacheEntry=( LPINTERNET_CACHE_ENTRY_INFO ) new char[ dwEntrySize ];
            lpCacheEntry->dwStructSize = dwEntrySize;

retry:
            if (!FindNextUrlCacheEntryEx(hCacheDir,lpCacheEntry,
                                         &dwEntrySize,NULL,NULL,NULL))
            {
                delete[] lpCacheEntry;
                switch ( GetLastError() )
                {
                case ERROR_NO_MORE_ITEMS:
                    FindCloseUrlCache( hCacheDir );
                    return true;
                case ERROR_INSUFFICIENT_BUFFER:
                    lpCacheEntry = ( LPINTERNET_CACHE_ENTRY_INFO )
                                   new char[dwEntrySize];
                    lpCacheEntry->dwStructSize = dwEntrySize;
                    goto retry;
                default:
                    FindCloseUrlCache( hCacheDir );
                    return false;
                }
            }

            //Found an item..
            if ( lpCacheEntry->CacheEntryType & cache_type )
            {
                //URLHISTORY_CACHE_ENTRY and  COOKIE_CACHE_ENTRY entries are also
                //NORMAL_CACHE_ENTRY entries. We need to separate these.
                if ( ( cache_type & ( URLHISTORY_CACHE_ENTRY | COOKIE_CACHE_ENTRY ) ) ||
                       ( cache_type == NORMAL_CACHE_ENTRY && !( lpCacheEntry->CacheEntryType & ( URLHISTORY_CACHE_ENTRY | COOKIE_CACHE_ENTRY ) ) ) )
                {
                    ItemsFound++;
                    BytesFound += lpCacheEntry->dwSizeHigh * MAXDWORD +
                                  lpCacheEntry->dwSizeLow;
                    ItemList.push_back( lpCacheEntry->lpszSourceUrlName );
                    ::wxLogDebug( L"%hs: Found %s", __FUNCTION__ , lpCacheEntry->lpszSourceUrlName );
                }
            }

            delete[] lpCacheEntry;
        }
        while (true);

    }

    void ie_base::DeleteCache()
    {
        DWORD dwEntrySize;
        LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry;
        HANDLE hCacheDir;

        dwEntrySize = 4096;

        lpCacheEntry = ( LPINTERNET_CACHE_ENTRY_INFO ) new char[dwEntrySize];
        lpCacheEntry->dwStructSize = dwEntrySize;


again:
        hCacheDir = FindFirstUrlCacheEntryEx( NULL, 0, ALL_CACHE_ENTRY,
                                              0, lpCacheEntry, &dwEntrySize, NULL, NULL, NULL);

        if ( !hCacheDir )
        {
            delete[] lpCacheEntry;
            switch ( GetLastError() )
            {
            case ERROR_NO_MORE_ITEMS:
                FindCloseUrlCache( hCacheDir );
                return ;
            case ERROR_INSUFFICIENT_BUFFER:
                lpCacheEntry = ( LPINTERNET_CACHE_ENTRY_INFO )
                               new char[dwEntrySize];
                lpCacheEntry->dwStructSize = dwEntrySize;
                goto again;
            default:
                FindCloseUrlCache( hCacheDir );
                return ;
            }
        }
        //Found an entry..

        CleanCacheItem( lpCacheEntry );

        delete ( lpCacheEntry );


        do
        {
            dwEntrySize = 4096;
            lpCacheEntry = ( LPINTERNET_CACHE_ENTRY_INFO ) new char[dwEntrySize];
            lpCacheEntry->dwStructSize = dwEntrySize;

retry:
            if ( !FindNextUrlCacheEntryEx( hCacheDir,lpCacheEntry,
                                           &dwEntrySize,NULL,NULL,NULL ) )
            {
                delete[] lpCacheEntry;
                switch ( GetLastError() )
                {
                case ERROR_NO_MORE_ITEMS:
                    FindCloseUrlCache( hCacheDir );
                    return ;
                case ERROR_INSUFFICIENT_BUFFER:
                    lpCacheEntry = ( LPINTERNET_CACHE_ENTRY_INFO )
                                   new char[dwEntrySize];
                    lpCacheEntry->dwStructSize = dwEntrySize;
                    goto retry;
                default:
                    FindCloseUrlCache( hCacheDir );
                    return ;
                }
            }
            //Found an entry..

            CleanCacheItem( lpCacheEntry );
            delete[] lpCacheEntry;
        }
        while (true);


    }

    void ie_base::GetFilesAsStrings(std::vector<std::wstring>& Messages)
    {
        unsigned int count = ItemList.size();
        Messages.reserve(count);

        for (unsigned int k = 0;k < count;++k)
        {
            Messages.push_back(ItemList[k]);
        }

    }


//---------------------------------------------------------------------------

    ie_cache::ie_cache( bool delete_offline_items ) :  ie_base( NORMAL_CACHE_ENTRY )
    {
        process = L"iexplore.exe";
        process_pretty_name = _( "Internet Explorer" );
        DeleteOfflineItems = delete_offline_items;
        ShortDesc = _( "Internet Explorer Cache" );
        LongDesc = _( "Clears the Internet Explorer browser cache" );
    }

    void ie_cache::CleanCacheItem( LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry )
    {
        if ( cache_type == NORMAL_CACHE_ENTRY &&
                !(lpCacheEntry->CacheEntryType & ( URLHISTORY_CACHE_ENTRY | COOKIE_CACHE_ENTRY ) ) )
        {
            if (lpCacheEntry->CacheEntryType & STICKY_CACHE_ENTRY && DeleteOfflineItems )
            {
                if (DeleteUrlCacheEntry( lpCacheEntry->lpszSourceUrlName ) )
                {
                    ::wxLogDebug( L"%hs: deleted sticky %s", __FUNCTION__ , lpCacheEntry->lpszSourceUrlName );
                    ItemsCleaned++;
                    BytesCleaned += lpCacheEntry->dwSizeHigh * MAXDWORD +
                                    lpCacheEntry->dwSizeLow;
                }

                return;
            } //if STICKY_CACHE_ENTRY


            if (DeleteUrlCacheEntry( lpCacheEntry->lpszSourceUrlName ) )
            {
                ::wxLogDebug( L"%hs: deleted %s", __FUNCTION__ , lpCacheEntry->lpszSourceUrlName );

                ItemsCleaned++;
                BytesCleaned += lpCacheEntry->dwSizeHigh * MAXDWORD +
                                lpCacheEntry->dwSizeLow;
            }

            return;
        } //if NORMAL_CACHE_ENTRY

    }

    void ie_cache::Clean()
    {
        DeleteCache();
    }

    void ie_cache::DoScan(bool Preview)
    {
        __int64 GID = 0;
        EnumerateCache( GID );
    }
//---------------------------------------------------------------------------

    ie_history::ie_history() : ie_base( URLHISTORY_CACHE_ENTRY )
    {
        CoInitialize( NULL );

        ShortDesc = _( "Internet Explorer History" );
        LongDesc = _( "Removes Internet Explorer history records" );

        pUrlHistoryStg2 = NULL;

        HRESULT hr = CoCreateInstance( CLSID_CUrlHistory, NULL, CLSCTX_INPROC,
                                       IID_IUrlHistoryStg2, (void**)&pUrlHistoryStg2 );

        if ( SUCCEEDED( hr ) ) //COM can be used to clear the history
        {
            ::wxLogDebug( L"%hs: CoCreateInstance - got IUrlHistoryStg2 instance", __FUNCTION__ );
            ComClearHistory = true;
        }
        else
        {
            ::wxLogDebug( L"%hs: CoCreateInstance - NO IUrlHistoryStg2 instance", __FUNCTION__ );
            ComClearHistory = false;
        }


    }

    void ie_history::CleanCacheItem( LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry )
    {

        if ( ( cache_type == URLHISTORY_CACHE_ENTRY ) && ( lpCacheEntry->CacheEntryType & URLHISTORY_CACHE_ENTRY ) )
            if ( DeleteUrlCacheEntry( lpCacheEntry->lpszSourceUrlName ) )
            {
                ::wxLogDebug( L"%hs: deleted %s", __FUNCTION__ , lpCacheEntry->lpszSourceUrlName );

                ItemsCleaned++;

                return;
            }
    }


    void ie_history::DoScan(bool Preview)
    {
        __int64 GID( 0 );
        EnumerateCache( GID );
    }

    void ie_history::Clean()
    {
        DeleteCache();

        if ( ComClearHistory )
        {
            HRESULT hr = pUrlHistoryStg2->ClearHistory();
            wxLogDebug( L"%hs: IUrlHistoryStg2.ClearHistory == %s ", __FUNCTION__,
                        ( hr == S_OK ) ? L"S_OK" : L"S_ERROR" );
        }

    }

    ie_history::~ie_history()
    {
        if (pUrlHistoryStg2)
        {
            pUrlHistoryStg2->Release();  //unload etc.
        }
        CoUninitialize();
    }

//---------------------------------------------------------------------------
    ie_cookies::ie_cookies() : ie_base( COOKIE_CACHE_ENTRY )
    {
        ShortDesc = _( "Internet Explorer Cookies" );
        LongDesc = _( "Removes Internet Explorer cookie files" );
//  SetIcon(ID_FFCOOKIES);
    }

    void ie_cookies::CleanCacheItem( LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry )
    {
        if ( ( cache_type == COOKIE_CACHE_ENTRY ) && ( lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY ) )
        {
            if ( !false )
                if (DeleteUrlCacheEntry( lpCacheEntry->lpszSourceUrlName ) )
                {
                    ::wxLogDebug( L"%hs: deleted %s", __FUNCTION__ , lpCacheEntry->lpszSourceUrlName );
                    ItemsCleaned++;
                    BytesCleaned += lpCacheEntry->dwSizeHigh * MAXDWORD +
                                    lpCacheEntry->dwSizeLow;
                    return;
                }

//            if (CookieFilterOn && TestEntryFilter(lpCacheEntry))
//                if (DelUrl(lpCacheEntry->lpszSourceUrlName))
//                {
//                    Info.FilesCleaned++;
//                    Info.BytesCleaned += lpCacheEntry->dwSizeHigh * MAXDWORD +
//                                         lpCacheEntry->dwSizeLow;
//                    return;
//                }
//            return; //Exit anyway, done with cookie handling
        }
    }

    void ie_cookies::DoScan(bool Preview)
    {
        __int64 GID( 0 );
        EnumerateCache( GID );
    }


    void ie_cookies::Clean()
    {
        DeleteCache();
    }
//---------------------------------------------------------------------------

}

