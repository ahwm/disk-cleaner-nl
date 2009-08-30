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

#ifndef FIREFOX_H
#define FIREFOX_H
#include "core\plugin_info_base.h"

#include <string>

namespace diskcleaner
{

    class firefox_base : public PlugInfo
    {
    protected:
        std::vector<std::wstring> ItemList;
        static bool IsInitialized;
        static bool FFPresent;
        static wchar_t cachefolder[MAX_PATH];   //First to check
        static wchar_t localcachefolder[MAX_PATH];  //New in FF 1.5 - local settings\app data\etc

        void SetIcon(int ICON_ID);
        virtual void DoScan(bool Preview) = 0;

        //files is a '\0' separated string of files in the Firefox profile folder
        void ScanFile( const wchar_t* files );
        //Show warning when removal of existing file fails
        void CleanFile( const wchar_t* files, const wchar_t* warning );
        void Initialize();

    public:
        virtual void Preview()
        {
            DoScan( true );
        };
        virtual void Scan()
        {
            DoScan( false );
        };

        virtual void GetFilesAsStrings( std::vector<std::wstring>& list);
        firefox_base()
        {
            if ( !IsInitialized ) Initialize();
        };

    };

    class firefox_cache : public firefox_base
    {
    protected:


    public:
        firefox_cache();
        virtual void Clean();


    protected:
        virtual void DoScan(bool Preview);
    };

    class firefox_history : public firefox_base
    {

    public:

        firefox_history();
        virtual void Clean();

    protected:

        virtual void DoScan(bool Preview);

    };

    class firefox_cookies : public firefox_base
    {

    public:

        firefox_cookies();
        virtual void Clean();

    protected:

        virtual void DoScan(bool Preview);


    };

} //namespace

#endif
