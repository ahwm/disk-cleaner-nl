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

#ifndef INTERNETEXPLORER_H
#define INTERNETEXPLORER_H

#include "core\plugin_info_base.h"

#include <string>
#include <windows.h>
#include <wininet.h> //Cache functions

//forward declaration
struct IUrlHistoryStg2;


namespace diskcleaner
{


    class ie_base : public PlugInfo
    {
    protected:

        std::vector<std::wstring> ItemList;
        unsigned long int cache_type;



        void SetIcon(int ICON_ID){};
        virtual void DoScan( bool Preview ) = 0;
        bool EnumerateCache( __int64& Group_ID );
        void DeleteCache();
        virtual void CleanCacheItem( LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry ) = 0;
        virtual void Clean() = 0;


    public:

        ie_base( unsigned long int cache_t ) : cache_type ( cache_t ) {};
        virtual void Preview()
        {
            DoScan( true );
        };
        virtual void Scan()
        {
            DoScan( false );
        };

        virtual void GetFilesAsStrings( std::vector<std::wstring>& list );

    };

    class ie_cache : public ie_base
    {
    private:
        bool DeleteOfflineItems;
        virtual void CleanCacheItem( LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry );

    public:
        ie_cache( bool delete_offline_items );
        virtual void Clean();


    protected:

        virtual void DoScan(bool Preview);
    };

    class ie_history : public ie_base
    {
    private:
        IUrlHistoryStg2* pUrlHistoryStg2;
        bool ComClearHistory;

        virtual void CleanCacheItem( LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry );


    public:

        ie_history();
        ~ie_history();
        virtual void Clean();

    protected:

        virtual void DoScan(bool Preview);

    };

    class ie_cookies : public ie_base
    {
    private:
        virtual void CleanCacheItem( LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry );

    public:

        ie_cookies();
        virtual void Clean();

    protected:

        virtual void DoScan(bool Preview);


    };

} //namespace

#endif

