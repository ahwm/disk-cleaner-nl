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

struct DSdata
{
  __int64 files;
  __int64 bytes;
};

struct TScanOptions{
  unsigned int minimum_age;
  bool ReadOnly;
  bool Hidden;
  bool Recursive;       // /s    (subfolders too)
  bool SubFolderOnly;   // /so   (just the subfolders)
  bool DelBaseFolder;   // /si   (include base folder)
  bool FilesOnly;       // /sf   (only remove files, keep folder structure)
  bool Secure;          //Not an option yet, but uses global HSWCTXT

  TScanOptions() : minimum_age( 0 ), ReadOnly( false ), Hidden( false ), Recursive( false ),
                    SubFolderOnly( false ), DelBaseFolder( false ), FilesOnly( false ) ,
                    Secure( false ) {};
} ;

//DSdata*  GetFilesInFolder(const wchar_t* folder, const wchar_t* masks, const TScanOptions* so);
DSdata  GetFilesInFolder(const wchar_t* folder, const wchar_t* masks,
                          TScanOptions* so, std::vector<std::wstring>& FileList);

//DSdata*  CleanFilesInFolder(const wchar_t* folder, const wchar_t* masks, const TScanOptions* so);
DSdata  CleanFilesInFolder(const wchar_t* folder, const wchar_t* masks,
                            TScanOptions* so, std::vector<std::wstring>& FileList);

void  SetRemoveOnReboot(bool Remove);
__int64  GetFilesScheduledRemoveOnReboot();
void ResetFilesScheduledRemoveOnReboot();
void SetAllFilesRemoved( bool _AllFilesRemoved);
bool GetAllFilesRemoved();


//} //extern "C"

#endif
