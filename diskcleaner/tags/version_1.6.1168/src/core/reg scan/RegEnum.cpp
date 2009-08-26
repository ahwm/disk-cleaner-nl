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

#include "RegEnum.h"
#include "wx/log.h"


void EnumRegKey(HKEY rootkey,wchar_t* subkey,wchar_t* value,__int64& nitems,
                __int64& nbytes, std::vector<std::wstring>& FileList, const wchar_t* rootname)
{
    static wchar_t __thread keyname[4*1024];
    wchar_t* keynamepos;
    HKEY aKey;
    DWORD cValMaxLength=0,cSubMaxLength=0,dwIndex=0;


    if ( rootname )
    {
        lstrcpy( keyname, rootname );
    }

    lstrcat( keyname, L"\\" );
    lstrcat( keyname, subkey );
    keynamepos = keyname + lstrlen( keyname );

    ::wxLogDebug( L"%hs: Opening key: %s", __FUNCTION__, keyname );

    if ( RegOpenKeyEx( rootkey, subkey, 0, KEY_READ, &aKey ) == ERROR_SUCCESS )
    {
        ::wxLogDebug( L"%hs: RegOpenKeyEx(%s) successful", __FUNCTION__, subkey );

        if ( value ) //Value info is needed
        {
            DWORD cbdata;

            ::wxLogDebug( L"%hs: Getting size of value %s\\%s" , __FUNCTION__, keyname, value );
            if (RegQueryValueEx(aKey,value,NULL,NULL,NULL,&cbdata)==ERROR_SUCCESS)
            {
                ::wxLogDebug( L"%hs: RegQueryValueEx successful, size of %s = %d", __FUNCTION__, value, cbdata );

                lstrcat( keyname, L"\\" );
                lstrcat( keyname, value );
                FileList.push_back( std::wstring( keyname ) );
                nitems++;
                nbytes+=cbdata;
            }
        }
        else  //Enum subkeys
        {
            RegQueryInfoKey(aKey,NULL,0,NULL,NULL,&cSubMaxLength,NULL,NULL,&cValMaxLength,
                            NULL,NULL,NULL);
            wchar_t* subsubkey = new wchar_t[++cSubMaxLength];
            wchar_t* keyvals = new wchar_t[++cValMaxLength];

            DWORD bufflen = cSubMaxLength;
            FILETIME ft;
            while (RegEnumKeyEx(aKey,dwIndex,subsubkey,&bufflen,NULL,NULL,NULL,&ft)
                    == ERROR_SUCCESS)
            {
                ++dwIndex;
                bufflen = cSubMaxLength;

                //enumerate subkeys
                ::wxLogDebug( L"%hs: enumerating subkey of %s: %s" , __FUNCTION__, keyname, subkey );
                EnumRegKey(aKey,subsubkey,NULL,nitems,nbytes,FileList,NULL);
                *keynamepos = 0;
            }
            //done enumerating this key's subkeys
            //now enum values
            DWORD dwvalIndex=0;

            DWORD valbufflen = cValMaxLength;

            while (RegEnumValue(aKey,dwvalIndex,keyvals,&valbufflen,NULL,NULL,NULL,NULL)
                    == ERROR_SUCCESS)
            {
                DWORD cbdata;
                ++dwvalIndex;

                ::wxLogDebug( L"%hs: getting size of value %s\\%s" , __FUNCTION__, keyname, value );

                if (RegQueryValueEx(aKey,keyvals,NULL,NULL,NULL,&cbdata)==ERROR_SUCCESS)
                {
                    ::wxLogDebug( L"%hs: RegQueryValueEx successful, size of %s = %d", __FUNCTION__, subkey, value, cbdata );
                    lstrcat(keyname,L"\\");
                    lstrcat(keyname,keyvals);
                    FileList.push_back(std::wstring(keyname));

                    nbytes+=cbdata;
                    ++nitems;
                    *keynamepos = 0;
                }
                valbufflen = cValMaxLength;

            }

            delete subsubkey;
            delete keyvals;

        }

        RegCloseKey(aKey);
    }
    else
    {
    wxLogDebug( L"%hs: failed to open key: %s", __FUNCTION__, keyname );
    }
}

bool DelRegKey(HKEY rootkey,wchar_t* subkey,wchar_t* value,__int64& nitems,
               __int64& nbytes, std::vector<std::wstring>& FileList, const wchar_t* rootname)
{
    static wchar_t __thread keyname[4*1024];
    HKEY aKey;
    DWORD cValMaxLength=0,cSubMaxLength=0,dwIndex=0;
    bool result;
    wchar_t* keynamepos;

    if (rootname)
    {
        lstrcpy(keyname,L"Deleted: ");
        lstrcat(keyname,rootname);
    }
    lstrcat(keyname,L"\\");
    lstrcat(keyname,subkey);
    keynamepos = keyname + lstrlen(keyname);

    if (RegOpenKeyEx(rootkey,subkey,0,KEY_ALL_ACCESS,&aKey)==ERROR_SUCCESS)
    {
        if (value) //Value info is needed
        {
            DWORD cbdata;

            if (RegQueryValueEx(aKey,value,NULL,NULL,NULL,&cbdata)==ERROR_SUCCESS)
            {
                if (RegDeleteValue(aKey,value)==ERROR_SUCCESS)
                {
                    lstrcat(keyname,L"\\");
                    lstrcat(keyname,value);
                    FileList.push_back(std::wstring(keyname));
                    nitems++;
                    nbytes+=cbdata;
                }
            }
            RegCloseKey(aKey);
        }
        else  //Delete subkeys
        {
            RegQueryInfoKey(aKey,NULL,0,NULL,NULL,&cSubMaxLength,NULL,NULL,&cValMaxLength,
                            NULL,NULL,NULL);
            wchar_t* subsubkey = new wchar_t[++cSubMaxLength];
            wchar_t* keyvals = new wchar_t[++cValMaxLength];

            DWORD bufflen = cSubMaxLength;
            FILETIME ft;
            while (RegEnumKeyEx(aKey,dwIndex,subsubkey,&bufflen,NULL,NULL,NULL,&ft)
                    == ERROR_SUCCESS)
            {

                bufflen = cSubMaxLength;

                //Delete subkeys
                if (!DelRegKey(aKey,subsubkey,NULL,nitems,nbytes,FileList,NULL))
                    ++dwIndex; //Only increment when delete is succesful
            }
            //done deleting this key's subkeys
            //now delete values

            DWORD valbufflen = cValMaxLength;

            while (RegEnumValue(aKey,0,keyvals,&valbufflen,NULL,NULL,NULL,NULL)
                    == ERROR_SUCCESS)
            {
                DWORD cbdata;

                if (RegQueryValueEx(aKey,keyvals,NULL,NULL,NULL,&cbdata)==ERROR_SUCCESS)
                {
                    if (RegDeleteValue(aKey,keyvals)==ERROR_SUCCESS)
                    {
                        lstrcat(keyname,L"\\");
                        lstrcat(keyname,keyvals);
                        FileList.push_back(std::wstring(keyname));

                        nbytes+=cbdata;
                        ++nitems;
                        *keynamepos = 0;

                    }
                }
                valbufflen = cValMaxLength;

            }

            //Close this key and delete it
            RegCloseKey(aKey);
            result = (RegDeleteKey(rootkey,subkey)==ERROR_SUCCESS);

            delete subsubkey;
            delete keyvals;
        }
    }
    else
    {
//     addlog("Failed to open key: " + std::wstring(subkey));
        result = false;
    }
    return result;
}


bool CrackRegKey(wchar_t* regstring,HKEY& root, wchar_t*& subkey,wchar_t*& value)
{

    if (!(subkey = wcsstr(regstring,L"|")))
        return false;

    *subkey = 0;
    ++subkey;

    if (!lstrcmpi(regstring,L"HKEY_CURRENT_USER"))root = HKEY_CURRENT_USER;
    else
        if (!lstrcmpi(regstring,L"HKEY_LOCAL_MACHINE"))root = HKEY_LOCAL_MACHINE;
        else
            if (!lstrcmpi(regstring,L"HKEY_USERS")) root = HKEY_USERS;
            else
                if (!lstrcmpi(regstring,L"HKEY_CLASSES_ROOT")) root = HKEY_CLASSES_ROOT;
                else return false; //No valid root key

    value = wcsstr(subkey,L"|");
    if (value)
    {
        //value can point to real value or to '\0', meaning empty std::wstring
        *value = 0;
        ++value;
    }
    //value points to NULL

    return true;
}


