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


namespace diskcleaner
{

    // This class represents the interface for all of DC's plug-ins
    // This class is an abstract base class, though not pure virtual.
    class PlugInfo
    {
    protected:
        std::wstring ShortDesc;          //Title of the plug-in
        std::wstring LongDesc;           //Longer descriptive text

        __int64 ItemsFound;              //The number of items found
        __int64 BytesFound;              //The size of all items together in bytes
        __int64 ItemsCleaned;            //After cleaning, the number of items that were removed
        __int64 BytesCleaned;            //Disk space recovered by removing the items

        bool SecureRemove;               //Not used, but indicates whether secure removal
                                         //such as overwriting with random numbers should be used
    public:

        virtual __int64 GetBytesFound() const { return BytesFound; };
        virtual __int64 GetBytesCleaned() const { return BytesCleaned; } ;
        virtual __int64 GetItemsFound() const { return ItemsFound; };
        virtual __int64 GetItemsCleaned() const { return ItemsCleaned; };
        virtual const std::wstring& GetShortDesc() const { return ShortDesc; };
        virtual const std::wstring& GetLongDesc() const { return LongDesc; };
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

} //namespace
#endif //STRUCTS_H
