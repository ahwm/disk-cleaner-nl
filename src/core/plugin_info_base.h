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

#ifndef STRUCTS_H
#define STRUCTS_H

#include <windows.h>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <wx/log.h>


//typedef void* HICON;
//struct TCleaningInfo;

namespace diskcleaner
{

//Not thread safe accessing!!
    extern std::map<std::wstring,HANDLE> IconList;
    extern std::map<std::wstring,HANDLE>::iterator icon_iterator;
    HANDLE LoadIconShared(std::wstring& filename);

    class PlugInfo
    {
    protected:
        std::wstring ShortDesc;          //Shown text in main form
        std::wstring LongDesc;           //Shown when pressing ? button

        __int64 ItemsFound;
        __int64 BytesFound;
        __int64 ItemsCleaned;
        __int64 BytesCleaned;


        bool SecureRemove;
    public:

        virtual __int64 GetBytesFound() { return BytesFound; };
        virtual __int64 GetBytesCleaned() { return BytesCleaned; };
        virtual __int64 GetItemsFound() { return ItemsFound; };
        virtual __int64 GetItemsCleaned() { return ItemsCleaned; };
        virtual HICON GetIcon() { return NULL; };
        virtual bool CanConfigure() { return false; };
        virtual const std::wstring& GetShortDesc() { return ShortDesc; };
        virtual const std::wstring& GetLongDesc() { return LongDesc; };
        virtual void Configure() {};
        virtual void Clean() = 0;
        virtual void Preview() = 0;
        virtual void Scan() = 0;
        bool GetSecureRemove()
        {
            return SecureRemove;
        };
        void SetSecureRemove(bool bVal)
        {
            SecureRemove = bVal;
        };
        virtual void GetFilesAsStrings(std::vector<std::wstring>&) = 0;

        virtual ~PlugInfo(){};
        PlugInfo() : ItemsFound( 0 ), BytesFound( 0 ),
                ItemsCleaned( 0 ),BytesCleaned( 0 ), SecureRemove( false )
        {
            ;
        };

    };

    std::wstring SetItemText(PlugInfo* pInfo);

    std::wstring BytesToString(__int64 NrOfBytes);

} //namespace
#endif //STRUCTS_H
