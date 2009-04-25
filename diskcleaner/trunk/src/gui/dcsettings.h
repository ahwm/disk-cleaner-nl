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
#ifndef _DCSETTINGS_H_
#define _DCSETTINGS_H_

class wxConfigBase;

namespace diskcleaner
{

    struct global_
    {
        bool delete_locked;
        bool hide_empty;
        bool show_description;
    };

    struct systemp_
    {
        bool delete_ro;
        bool delete_subfolders;
        long int min_age;
    };

    struct tempinternetfiles_
    {
        bool delete_offline;
    };

    struct cookies_
    {
        bool use_cookie_filter;
        long min_cookie_age;
    };

    class dcsettings
    {
        public:

        global_ global;
        systemp_ systemp;
        tempinternetfiles_ tempinternetfiles;
        cookies_ cookies;

        bool Persist();
        bool Persist( wxConfigBase* const config_object ) ;
        bool Load();
        bool Load( wxConfigBase* const config_object );
    };

} //namespace diskcleaner


#endif // _DCSETTINGS_H_
