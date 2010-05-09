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

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "dcApp.h"
#include "dc_frame.h"
#include "processes_dlg.h"
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/log.h>
#include <wx/cmdline.h>

#ifdef __cplusplus
extern "C"
{
#endif
    WINUSERAPI BOOL WINAPI CheckTokenMembership(HANDLE, PSID, PBOOL);

#ifdef __cplusplus
}
#endif

IMPLEMENT_APP(dcApp);

/// Available command line switches
static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, L"p", L"portable", _( "run in portable mode: save configuration files in the application "
                                                "folder." ) },
    { wxCMD_LINE_SWITCH, L"q", L"quiet",    _( "clean without showing a GUI. If a preset to be loaded is specified "
                                               "(option -r), the items checked in the specified preset will be cleaned. "
                                               "Otherwise, the items checked the last time Disk Cleaner was run in "
                                               "interactive mode are cleaned." ) },

    { wxCMD_LINE_OPTION, L"r", L"recall-preset", _( "specify preset to be recalled for use. If -q (quiet mode) "
                                                    "is also specified, the preset will be loaded and "
                                                    "used for cleaning the items specified in the preset." ),
                                                 wxCMD_LINE_VAL_STRING},
//    { wxCMD_LINE_SWITCH, L"l", L"log-to-file", L"log the results of the cleaning to file." },
    { wxCMD_LINE_SWITCH, L"nt", L"no-text-plugins", _( "do not use text plugins (.dct files). " ) },
    { wxCMD_LINE_SWITCH, L"nb", L"no-builtin-plugins", _( "do not use the built-in plugins" ) },
#ifdef __WXDEBUG__
    { wxCMD_LINE_SWITCH, L"d", L"debug", _( "Shows messages in a debug window " ) },
#endif
    { wxCMD_LINE_NONE }
};



dcApp::dcApp()
{

    // Extract directory
    strAppDirectory = wxStandardPaths::Get().GetExecutablePath();
    strAppDirectory = strAppDirectory.substr(0, strAppDirectory.rfind(L"\\"));
}

bool dcApp::OnInit()
{
    //Do default processing first (command line etc.)
    //sets config file location based on result.
    //Aborts execution if, e.g., --help was specified
    if (wxApp::OnInit() )
    {

        settings.Load();

        frame = new dc_frame(0L, settings);
        frame->SetIcon(wxICON(aaaa)); // To Set App Icon
        frame->init_dialog();

        if ( !IsQuietMode() )
        {
            //Show main window
            frame->Show( true );
            frame->Raise();
        }
        else
        {
            wxLogDebug( L"%hs: quiet mode, not showing dc_frame", __FUNCTION__ );
            wxLogNull dont_log;
            __int64 total_files, total_bytes; // Unused
            frame->clean(total_files, total_bytes);
        }

        return true;
    }

    return false;
}

    //FROM MSDN
    /**
    Routine Description: This routine returns TRUE if the caller's
    process is a member of the Administrators local group. Caller is NOT
    expected to be impersonating anyone and is expected to be able to
    open its own process and process token.
    \return
       true - Caller has Administrators local group.
       false - Caller does not have Administrators local group.
    */
bool dcApp::IsUserAdmin()
{
    BOOL b;
    SID_IDENTIFIER_AUTHORITY NtAuthority = {SECURITY_NT_AUTHORITY};
    PSID AdministratorsGroup;

    b = AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                                 &AdministratorsGroup);
    if (b)
    {
        if (!::CheckTokenMembership( NULL, AdministratorsGroup, &b))
        {
            b = FALSE;
        }
        FreeSid(AdministratorsGroup);
    }

    return ( b == TRUE );

}

/// Used to determine if a 'Run as administrator' button is displayed.
/// \return true if the OS is Vista or higher, false otherwise
bool dcApp::IsVistaOrHigher()
{
    OSVERSIONINFO osversion;
    osversion.dwOSVersionInfoSize = sizeof(osversion);

    GetVersionEx(&osversion);

    return osversion.dwMajorVersion >= 6;
}


void dcApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    wxApp::OnInitCmdLine( parser );
    parser.SetDesc (cmdLineDesc);
    parser.SetLogo( L"Disk Cleaner, a fast open source utility to recover disk space.\n"
                    L"Copyright 1999-2009 R. J. Moerland\n"
                    L"Released under the GPL v2\n" );
}

/// Parses the command line for options and sets no_text_plugins,
/// no_builtin_plugins, quiet_mode and defines the location where to
/// load and save the user's preference
/// \see NoBuiltInPlugins NoTextPlugins IsQuietMode diskcleaner::dcsettings
bool dcApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if ( !wxApp::OnCmdLineParsed( parser ) )
    {
        //End application
        return false;
    }

    bool portable = parser.Found( L"p" );
    no_text_plugins = parser.Found( L"nt" );
    no_builtin_plugins = parser.Found( L"nb" );

    wxString wxrecall_preset;
    quiet_mode    = parser.Found( L"q" );
    if ( parser.Found( L"r", &wxrecall_preset ) )
    {
        recall_preset = wxrecall_preset;
    }

#ifdef __WXDEBUG__
    if ( parser.Found( L"d" ) )
    {
        pLogWindow = std::auto_ptr< wxLogWindow > (new wxLogWindow( NULL, L"Disk Cleaner debug messages", true, true ) );
    }
#endif

    if ( portable )
    {

        wxFileConfig* cf =
            new wxFileConfig( L"Disk Cleaner", wxEmptyString, GetAppDirectory() + L"\\dcsettings.ini" );
        wxConfigBase::Set( cf );
    }
    else //Not in portable mode
    {
        std::wstring configpath ( wxStandardPaths::Get().GetUserConfigDir()  + L"\\Disk Cleaner" );

        if ( !::wxDirExists( configpath.c_str() ) )
        {
            if ( !::wxMkdir( configpath ) )
            {
                wxLogError( _( "I'm unable to save/load the per user settings in a file; I failed to create folder %s. "
                            L"Saving settings to the registry instead." ) , configpath.c_str() );

                //Let wxWidgets figure it out.
                wxConfigBase::Create();

                return true;

            }

        }

        wxFileConfig* cf = new wxFileConfig( L"", L"", configpath + L"\\dcsettings.ini", L"", wxCONFIG_USE_LOCAL_FILE );
        wxConfigBase::Set( cf );


    }

    return true;
}

/// \param ButtonWindow a handle (cast to HWND) to a button window
/// \param ShowShield true to show, false to hide
void  dcApp::Button_SetShield(const WXWidget ButtonWindow, bool ShowShield )
{
    const int BCM_FIRST = 0x1600; //Normal button
    const int BCM_SETSHIELD = (BCM_FIRST + 0x000C); //Elevated button
    SendMessage(static_cast<HWND>( ButtonWindow ) , BCM_SETSHIELD, 0, TRUE);
}

