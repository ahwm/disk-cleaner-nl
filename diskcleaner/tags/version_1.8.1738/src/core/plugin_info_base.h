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


namespace diskcleaner
{

    /// This class represents the interface for all of Disk Cleaner's plug-ins
    /// This class is an abstract base class, though not pure virtual.
    /// To create  a new plug-in, derive a class from this class.
    class PlugInfo
    {
    protected:
        std::wstring ShortDesc;          ///< Title of the plug-in
        std::wstring LongDesc;           ///< Longer descriptive text

        __int64 ItemsFound;              ///< The number of items found after a Scan()
        __int64 BytesFound;              ///< The size of all items together in bytes
        __int64 ItemsCleaned;            ///< After cleaning, the number of items that were removed
        __int64 BytesCleaned;            ///< Disk space recovered by removing the items

        bool AdminRequired;              ///< Does the plug-in needsadministrator priviliges?
        std::wstring process;            ///< Process/exe name to check if running. \see processes_dlg RunningProcessCheck
        std::wstring process_pretty_name;///< Name of application to show in message dialogs

        bool SecureRemove;               ///< Not used
                                         /**<  indicates whether secure removal
                                              such as overwriting with random numbers should be used */
    public:

        /// returns BytesFound
        virtual __int64 GetBytesFound() const { return BytesFound; };
        /// returns BytesCleaned
        virtual __int64 GetBytesCleaned() const { return BytesCleaned; } ;
        /// returns ItemsFound
        virtual __int64 GetItemsFound() const { return ItemsFound; };
        /// returns ItemsCleaned
        virtual __int64 GetItemsCleaned() const { return ItemsCleaned; };
        /// returns ShortDesc
        virtual const std::wstring& GetShortDesc() const { return ShortDesc; };
        /// returns LongDesc
        virtual const std::wstring& GetLongDesc() const { return LongDesc; };
        /// Clean all items associated with plug-in
        virtual void Clean() = 0;
        /// Scan and store names of found items
        virtual void Preview() = 0;
        /// Scan, don't store names
        virtual void Scan() = 0;

//        /// Not used
//        bool GetSecureRemove()
//        {
//            return SecureRemove;
//        };
//
//        ///Not used
//        void SetSecureRemove(bool bVal)
//        {
//            SecureRemove = bVal;
//        };
//
        /// Copy a list of found items into the given std::vector<std::wstring> reference
        virtual void GetFilesAsStrings(std::vector<std::wstring>&) = 0;

        /// returns  AdminRequired
        bool AdminPriviligesRequired() { return AdminRequired; };

        /// Warn the user about a certain application if it's still running

        /// \param Process name of the executable file to check (example: dclean.exe)
        /// \param ProcessPrettyName of the application to show to the user (example: Disk Cleaner)
        /// \return true if Disk Cleaner should check for a running process, false otherwise.
        /// Nothing needs to be copied into the strings then.
        /// \see processes_dlg process process_pretty_name
        bool RunningProcessCheck( std::wstring& Process, std::wstring& ProcessPrettyName )
        {
            if( !process.empty() )
            {
                Process = process;
                ProcessPrettyName = process_pretty_name;
                return true;
            }
            return false;
        }

        virtual ~PlugInfo(){};
        PlugInfo() : ItemsFound( 0 ), BytesFound( 0 ),
                ItemsCleaned( 0 ),BytesCleaned( 0 ), AdminRequired( false ), SecureRemove( false )
        {
            ;
        };

    };

} //namespace
#endif //STRUCTS_H
