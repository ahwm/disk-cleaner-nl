/* This is C code to add a shortcut to the current program in the startup folder.
 * This code will NOT compile as C++. Add it as a seperate file with the .c extension.
 * To compile in Dev-C++ add '-lole32 -luuid -lshell32' to the linker box under project options.
 */


#include <windows.h>
#include <objbase.h>
#include <shlobj.h>
#include <objidl.h>
#include <shlwapi.h>
#include <stdio.h>

#if defined(_MSC_VER) || defined(__BORLANDC__)
//#pragma comment(lib,"uuid.lib")
//#pragma comment(lib,"ole32.lib")
//#pragma comment(lib,"shell32.lib")
#elif defined(__LCC__)
#pragma lib <ole32.lib>
#pragma lib <import32.lib>
#pragma lib <uuid.lib>
#endif

/*
 * Create a shortcut.
 * From http://groups.google.com/groups?sel...64ac1%40p87618a
 */
HRESULT xCreateShortcut (
 LPCTSTR CommandLine,
 LPCTSTR Arguments,
 WORD HotKey,
 LPCTSTR IconLocation,
 int IconIndex,
 int Show,
 LPCTSTR WorkingDirectory,
 LPCTSTR Description,
 LPCTSTR LinkName )
    {
    HRESULT hres;
    IShellLink *psl;
    BOOL bUninitCom = FALSE;

    if( (CommandLine==NULL) || (LinkName==NULL))
        {
        return E_INVALIDARG;
        }

    if ( SUCCEEDED(CoInitialize(NULL)))
        {
        bUninitCom = TRUE;
        }

    hres = CoCreateInstance( &CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (void **) &psl);

    if( SUCCEEDED(hres) )
        {
        IPersistFile *ppf;

        psl->lpVtbl->SetPath( psl, CommandLine );

        if( Description != NULL )
            {
            psl->lpVtbl->SetDescription( psl, Description );
            }

        if( Arguments != NULL )
            {
            psl->lpVtbl->SetArguments( psl, Arguments );
            }

        if( HotKey != 0 )
            {
            psl->lpVtbl->SetHotkey( psl, HotKey );
            }

        if( IconLocation != NULL )
            {
            psl->lpVtbl->SetIconLocation( psl, IconLocation, IconIndex);
            }

        if( Show != -1 )
            {
            psl->lpVtbl->SetShowCmd( psl, Show );
            }

        if( WorkingDirectory != NULL )
            {
            psl->lpVtbl->SetWorkingDirectory( psl, WorkingDirectory );
            }

        hres = psl->lpVtbl->QueryInterface( psl, &IID_IPersistFile, (void **) &ppf );

        if( SUCCEEDED(hres))
            {
#ifndef UNICODE
            WCHAR wsz[MAX_PATH];
            MultiByteToWideChar( CP_ACP, 0, LinkName, -1, wsz, MAX_PATH);
#else
            LPCWSTR wsz = LinkName;
#endif
            hres = ppf->lpVtbl->Save( ppf, wsz, TRUE );
            ppf->lpVtbl->Release(ppf);
            }

        psl->lpVtbl->Release(psl);
        }

    if (bUninitCom)
         {
         CoUninitialize();
         }

    return hres;
    }

/*
 * Get a special folder path.
 */
BOOL xGetFolderPath(LPTSTR szFolder, int csidlFolder)
{
	LPITEMIDLIST pidl = NULL;
	BOOL         bRet = FALSE;

	if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, csidlFolder, &pidl)))
	{
		if(SHGetPathFromIDList(pidl, szFolder))
		{
			bRet = TRUE;
		}

		CoTaskMemFree(pidl);
	}

	return bRet;
}

/*
 * Add ourselves to the startup folder as a shortcut.
 * Use -1 for the IconResourceID if you don't have an embedded icon.
 * szDescription and szArguments can be NULL.
 * szLinkTitle must not exceed MAX_PATH characters.
 */
BOOL xAddSelfAsStartupShortcut(LPCTSTR szLinkTitle, LPCTSTR szDescription,
                               LPCTSTR szArguments, int IconResourceID)
{
	TCHAR szLinkName[MAX_PATH + MAX_PATH + 10];
	TCHAR szPathToSelf[MAX_PATH];

	if (!GetModuleFileName(NULL, szPathToSelf, MAX_PATH))
	{
		return FALSE;
	}

	if (FAILED(xGetFolderPath(szLinkName, CSIDL_STARTUP)))
	{
		return FALSE;
	}

	lstrcat(szLinkName, TEXT("\\"));
	lstrcat(szLinkName, szLinkTitle);
	lstrcat(szLinkName, TEXT(".lnk"));

	return SUCCEEDED(xCreateShortcut(szPathToSelf, szArguments, 0,
	                                 IconResourceID != -1 ? szPathToSelf : NULL,
	                                 IconResourceID, -1, NULL, szDescription, szLinkName));
}

BOOL xDeleteSelfAsStartupShortcut(LPCTSTR szLinkTitle)
{
	TCHAR szLinkName[MAX_PATH + MAX_PATH + 10];

	if (FAILED(xGetFolderPath(szLinkName, CSIDL_STARTUP)))
	{
		return FALSE;
	}

	lstrcat(szLinkName, TEXT("\\"));
	lstrcat(szLinkName, szLinkTitle);
	lstrcat(szLinkName, TEXT(".lnk"));

	return DeleteFile(szLinkName);
}