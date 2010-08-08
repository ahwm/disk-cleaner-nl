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

#ifndef DCAPP_H
#define DCAPP_H

#include <wx/app.h>
#include <string>
#include <memory>

#include "gui/dcsettings.h"

//Forward declaration
class dc_frame;
class wxLogWindow;


class dcApp : public wxApp
{
    // Main window pointer
    dc_frame* frame;
    std::wstring strAppDirectory;

    // Name of preset to recall
    std::wstring recall_preset;
#ifdef __WXDEBUG__
    std::auto_ptr< wxLogWindow > pLogWindow;
#endif

    //true if -p is specified on command line
    bool portable;

    // true if -q is specified on command line
    bool quiet_mode;

    // true if -nt is specified on command line
    bool no_text_plugins;

    // true if -nb is specified on command line
    bool no_builtin_plugins;

    // Manages all user settings that are persistent
    diskcleaner::dcsettings settings;

public:
    dcApp();
    virtual bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

    /// True if portable mode is specified on command line
    bool IsPortable()
    {
        return portable;
    };

    /// True if the user is an administrator
    bool IsUserAdmin();

    /// True if the OS is Vista or higher
    bool IsVistaOrHigher();

     /// True if quiet mode was specified on the command line
    bool IsQuietMode()
    {
        return quiet_mode;
    };

    /// Returns the name of the preset to be loaded
    const std::wstring& GetPresetToBeRecalled()
    {
        return recall_preset;
    };

    /// True if text plug-ins are to be skipped
    bool NoTextPlugins()
    {
        return no_text_plugins;
    };

    /// True if the built-in plugins are to be skipped
    bool NoBuiltInPlugins()
    {
        return no_builtin_plugins;
    };

    /// Sets a shield icon on any WXWidget button handle
    void    Button_SetShield(const WXWidget ButtonWindow, bool ShowShield = true);

    /// Returns the application's installation folder
    const   std::wstring& GetAppDirectory()
    {
        return strAppDirectory;
    };

};

DECLARE_APP(dcApp);

#endif // DCAPP_H
