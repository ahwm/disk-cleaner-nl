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

#ifndef RegEnumH
#define RegEnumH

#include <windows.h>
#include <vector>
#include <string>

void  EnumRegKey(HKEY rootkey,wchar_t* subkey,wchar_t* value,__int64& nitems,
                __int64& nbytes, std::vector<std::wstring>&,const wchar_t* rootname);

//bool DelRegKey(HKEY rootkey,wchar_t* subkey,wchar_t* value,__int64& nitems,__int64& nbytes);

bool  DelRegKey(HKEY rootkey,wchar_t* subkey,wchar_t* value,__int64& nitems,
                __int64& nbytes, std::vector<std::wstring>& FileList, const wchar_t* rootname);

bool  CrackRegKey(wchar_t* regstring,HKEY& root, wchar_t*& subkey,wchar_t*& value);

#endif //RegEnumH
