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

#ifndef __SYSTEMPH
#define __SYSTEMPH

#include "core/plugin_info_base.h"

namespace diskcleaner
{
    struct systemp_;

    class system_temp : public PlugInfo
    {

    protected:
        std::vector<std::wstring> ItemList;
        diskcleaner::systemp_& settings;
        std::wstring tempfolder;

        static bool SysTempAvailable;

    public:
        system_temp( diskcleaner::systemp_& rsettings );
        void Clean();
        void Scan();
        void Preview() { Scan(); };
        void GetFilesAsStrings(std::vector<std::wstring>& Messages);
    };

}
#endif //__ASTDH
