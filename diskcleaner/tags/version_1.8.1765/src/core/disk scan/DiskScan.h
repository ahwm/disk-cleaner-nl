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

#ifndef DiskScanH
#define DiskScanH
//---------------------------------------------------------------------------
#include <vector>
#include <string>


//extern "C"
//{

/// GetFilesInFolder/CleanFilesInFolder returns a diskscan_data instance
struct diskscan_data
{
  /// number of files found/cleaned
  __int64 files;
  /// number of items found/cleaned
  __int64 bytes;
};

/// Scan options available for Get/CleanFilesInFolder()
struct TScanOptions{
  unsigned int minimum_age;
  bool ReadOnly;        ///< remove read-only files (used in systemp cleaner only)
  bool Hidden;          ///< remove hidden files (used in systemp cleaner only)
  bool Recursive;       ///< include subfolders (text plug-in file: /s)
  bool SubFolderOnly;   ///< subfolders only (text plug-in file: /so)
  bool DelBaseFolder;   ///< include base folder (text plug-in file: /si)
  bool FilesOnly;       ///< only remove files, keep folder structure (text plug-in file: /sf)
  bool Secure;          ///< not implemented
  bool System;          ///< remove system files, such as desktop.ini

  TScanOptions() : minimum_age( 0 ), ReadOnly( false ), Hidden( false ), Recursive( false ),
                    SubFolderOnly( false ), DelBaseFolder( false ), FilesOnly( false ) ,
                    Secure( false ), System( false ) {};
} ;

/// Scan for files in folder 'folder', with search masks 'masks'
/// Use options defined in TScanOptions* so, store names of found files in FileList
diskscan_data  GetFilesInFolder(const wchar_t* folder, const wchar_t* masks,
                          TScanOptions* so, std::vector<std::wstring>& FileList);

/// Clean files in folder 'folder', wich match search masks 'masks'
/// Use options defined in TScanOptions* so, store names of removed files in FileList
diskscan_data  CleanFilesInFolder(const wchar_t* folder, const wchar_t* masks,
                            TScanOptions* so, std::vector<std::wstring>& FileList);

/// Sets Delete locked files on reboot to true or false
void  SetRemoveOnReboot(bool Remove);

/// Returns the amount of files scheduled for removal on reboot
__int64  GetFilesScheduledRemoveOnReboot();

/// Sets the amount of files to be removed on reboot to zero
void ResetFilesScheduledRemoveOnReboot();

/// Sets the internal boolean AllFilesRemoved to true or false
void SetAllFilesRemoved( bool _AllFilesRemoved);

/// Returns true if all files in a folder were removed, false otherwise (skipped or locked files)
bool GetAllFilesRemoved();


//} //extern "C"

#endif
