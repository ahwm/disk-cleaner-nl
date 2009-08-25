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
#include <boost/shared_ptr.hpp>


//Forward declaration
class wxCmdLineParser;
class wxLogWindow;

class dcApp : public wxApp
{
    std::wstring strAppDirectory;
    std::wstring quiet_mode_preset;
#ifdef __WXDEBUG__
    boost::shared_ptr< wxLogWindow > pLogWindow;
#endif

    bool quiet_mode;
    bool no_text_plugins;
    bool no_buildin_plugins;

    public:
        dcApp();
        virtual bool OnInit();
        virtual void OnInitCmdLine(wxCmdLineParser& parser);
        virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
                bool IsUserAdmin();
                bool IsVistaOrHigher();
                bool IsQuietMode() {return quiet_mode;};
                bool NoTextPlugins() {return no_text_plugins;};
                bool NoBuildInPlugins() {return no_buildin_plugins;};
                std::wstring& GetQuietModePreset() {return quiet_mode_preset; };
        void    Button_SetShield(const WXWidget ButtonWindow, bool ShowShield = true);
        const   std::wstring& GetAppDirectory() { return strAppDirectory; };

};

DECLARE_APP(dcApp);

#endif // DCAPP_H
