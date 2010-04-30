
#ifndef AUTO_STARTUP_H
#define AUTO_STARTUP_H

#include <windows.h>
extern "C"
{
HRESULT xCreateShortcut (LPCTSTR CommandLine, LPCTSTR Arguments, WORD HotKey,
                         LPCTSTR IconLocation, int IconIndex, int Show,
                         LPCTSTR WorkingDirectory, LPCTSTR Description,
                         LPCTSTR LinkName);

BOOL xGetFolderPath(LPTSTR szFolder, int csidlFolder);

BOOL xAddSelfAsStartupShortcut(LPCTSTR szLinkTitle, LPCTSTR szDescription,
                               LPCTSTR szArguments, int IconResourceID);

BOOL xDeleteSelfAsStartupShortcut(LPCTSTR szLinkTitle) ;
}
#endif

