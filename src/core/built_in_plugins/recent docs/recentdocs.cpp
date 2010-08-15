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

#include <windows.h>
#include <shlobj.h>

#include "core/disk scan/DiskScan.h"
#include "core/built_in_plugins/recent docs/recentdocs.h"


namespace diskcleaner
{
    recent_docs::recent_docs()

    {
        ShortDesc =  _( "Recent Documents" );
        LongDesc =   _( "Clears the Recent Documents in the Start menu" );
    }

    void recent_docs::Scan()
    {
        diskscan_data scandata = { 0 };

        LPITEMIDLIST pidl;

        wchar_t* RecentDocPath = new wchar_t[MAX_PATH];           //Recent Document path

        if ( SHGetSpecialFolderLocation(NULL,CSIDL_RECENT,&pidl) == NOERROR )
        {
            if ( RecentDocPath && SHGetPathFromIDList( pidl, RecentDocPath ) )
            {
                lstrcat( RecentDocPath, L"\\");
                TScanOptions so;
                so.ReadOnly = so.Recursive = true;
                scandata = GetFilesInFolder( RecentDocPath,L"*.*\0", &so, ItemList );
            }
        }
        delete[] RecentDocPath;

        BytesFound = scandata.bytes;
        ItemsFound = scandata.files;
    }

    void recent_docs::Clean()
    {
        SHAddToRecentDocs(0,NULL);
        ItemsCleaned = ItemsFound;
        BytesCleaned = BytesFound;
    }

    void recent_docs::GetFilesAsStrings( std::vector<std::wstring>& Messages )
    {

        unsigned int count = ItemList.size();
        for (unsigned int k = 0;k < count;++k)
        {
            Messages.push_back(ItemList[k]);
        }

    }

}
//---------------------------------------------------------------------------
//BUILDTYPE TDLLInfo* GetDLLInfo()
//{
//  SetRDocInfo();                                //Recent documents
//
//  SetInfoValues(FindDoc, NULL, FIND_DOC_KEY,    //Find Documents list
//                   CleanFDoc, NULL, FDocOK,
//                   ID_FINDDOC,"Find Document List",
//                   "Clears the most recently used search patterns "
//                   "in the Find Document window.");
//
//  SetInfoValues(FindComp, NULL, FIND_COMP_KEY,  //Find Computer list
//                   CleanFComp, NULL, FCompOK,
//                   ID_FINDCOMP,"Find Computer List",
//                   "Clears the most recently used search patterns "
//                   "in the Find Computer window.");
//
//  SetInfoValues(Run, NULL, RUN_MRU_KEY,         //Run... list
//                   CleanRun, NULL, RunOK,
//                   ID_RUN,"Run... List",
//                   "Clears the most recently used entries "
//                   "in the Run... window, that are not a URL.");
//
//  SetInfoValues(TypedUrl, "url", TYPED_URL_KEY, //Typed URLs
//                   CleanTypedUrl, NULL, TypedOK,
//                   ID_TYPEDURL,"URLs in IE Address Bar",
//                   "Clears the most recently typed URLs "
//                   "in the Internet Explorer address bar.");
//
//  SetInfoValues(AddressBar, "url", ADDRESS_BAR_KEY, //Address Bar Shell
//                   CleanAddressBar, NULL, ABOK,
//                   ID_ADDRESSBAR,"URLs in Shell Address Bar",
//                   "Clears the most recently typed URLs "
//                   "in the Shell Address Bar in the Task Bar.");
//
//  SetMediaPlayerInfo();
//
//  SetInfoValues(WinzipRecent,"filemenu", WINZIP_RECENT_FILES, //Winzip Recent files
//                   CleanWinzipRecent, NULL, WinzipRecentOK,
//                   ID_WINZIP,"WinZip Recent Files",
//                   "Clears the most recently opened ZIP "
//                   "files list in WinZip.");
//
//  SetInfoValues(WinzipExtract,"extract", WINZIP_EXTRACT_KEY,  //Winzip Extract folders
//                   CleanWinzipExtract, NULL, WinzipExtractOK,
//                   ID_WINZIP,"WinZip Extract To Folders",
//                   "Clears the list of folders most recently used"
//                   " as ZIP extraction destination in WinZip.");
//
//  SetInfoValues(Paint,"File", PAINT_RECENT_FILES,             //Paintbrush
//                   CleanPaint, NULL, PaintOK,
//                   ID_PAINT,"Paint Recent Files",
//                   "Clears the most recently opened bitmap (BMP) files "
//                   "list in Paint");
//
//  SetInfoValues(WordPad,"File", WORDPAD_RECENT_FILES,             //Wordpad
//                   CleanWordPad, NULL, WordPadOK,
//                   ID_WORDPAD,"WordPad Recent Files",
//                   "Clears the most recently opened files "
//                   "list in WordPad");
//
//
//
//}
//
//void SetMediaPlayerInfo()
//{
//  HKEY MPUKey;
//  int files=0,bytes=0;
//  MediaURLOK = 0;
//
//  if(RegOpenKeyEx(HKEY_CURRENT_USER,MEDIAPLAYER_URL_KEY,0,KEY_ALL_ACCESS,&MPUKey)
//      == ERROR_SUCCESS)
//  {
//     EnumStrKey(MPUKey,"URL",files,bytes);
//     RegCloseKey(MPUKey);
//     MediaURLOK = 1;
//  }
//  if(RegOpenKeyEx(HKEY_CURRENT_USER,MEDIAPLAYER_URL_KEY2,0,KEY_ALL_ACCESS,&MPUKey)
//      == ERROR_SUCCESS)
//  {
//     EnumStrKey(MPUKey,"File",files,bytes);
//     RegCloseKey(MPUKey);
//     MediaURLOK = 1;
//  }
//
//  MediaURL.FilesInvolved = files;
//  MediaURL.BytesInvolved = bytes;
//  MediaURL.pCleanItemFunc = CleanMediaPlayer;
//  MediaURL.pConfigFunc = NULL;
//  MediaURL.Icon = LoadIcon(hDLLinst, MAKEINTRESOURCE(ID_MEDIAPLAY));
//  MediaURL.Icon = (HICON) CopyImage(MediaURL.Icon, IMAGE_ICON,16,16,
//                             LR_COPYFROMRESOURCE|LR_COPYDELETEORG);
//
//  strcpy(MediaURL.ShortDesc,"Media Player Recent URLs");
//  strcpy(MediaURL.LongDesc,"Clears the most recently used URLs "
//                         "(including local files) in the Media Player.");
//}
////---------------------------------------------------------------------------

//void CleanMediaPlayer()
//{
//  HKEY MPUKey;
//  int files=0,bytes=0;
//
//  if(MediaURLOK)
//  {
//    if(RegOpenKeyEx(HKEY_CURRENT_USER,MEDIAPLAYER_URL_KEY,0,KEY_ALL_ACCESS,&MPUKey)
//        == ERROR_SUCCESS)
//    {
//       CleanStrKey(MPUKey,"URL",files,bytes);
//       RegCloseKey(MPUKey);
//    }
//    if(RegOpenKeyEx(HKEY_CURRENT_USER,MEDIAPLAYER_URL_KEY2,0,KEY_ALL_ACCESS,&MPUKey)
//      == ERROR_SUCCESS)
//    {
//       CleanStrKey(MPUKey,"File",files,bytes);
//       RegCloseKey(MPUKey);
//    }
//    MediaURL.FilesCleaned = files;
//    MediaURL.BytesCleaned = bytes;
//  }
//}
////---------------------------------------------------------------------------
//
