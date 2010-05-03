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
class wxCmdLineParser;
class wxLogWindow;
class dc_frame;


class dcApp : public wxApp
{
    dc_frame* frame;
    std::wstring strAppDirectory;
    std::wstring recall_preset;
#ifdef __WXDEBUG__
    std::auto_ptr< wxLogWindow > pLogWindow;
#endif

    bool quiet_mode;
    bool no_text_plugins;
    bool no_buildin_plugins;

    //Manages all user settings that are persistent
    diskcleaner::dcsettings settings;

public:
    dcApp();
    virtual bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
    bool IsUserAdmin();
    bool IsVistaOrHigher();
    bool IsQuietMode()
    {
        return quiet_mode;
    };
    const std::wstring& GetPresetToBeRecalled()
    {
        return recall_preset;
    };

    bool NoTextPlugins()
    {
        return no_text_plugins;
    };
    bool NoBuildInPlugins()
    {
        return no_buildin_plugins;
    };

    void    Button_SetShield(const WXWidget ButtonWindow, bool ShowShield = true);
    const   std::wstring& GetAppDirectory()
    {
        return strAppDirectory;
    };

};

DECLARE_APP(dcApp);

#endif // DCAPP_H
