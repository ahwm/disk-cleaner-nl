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
    struct framesize_
    {
        int width;
        int height;
        int topx;
        int topy;
    };


    struct UI_
    {
        framesize_ dc_frame_size;
        framesize_ result_frame_size;
        int col_sdesc_width, col_ldesc_width,
            col_item_width, col_size_width;
        int sort_column;
        int sortorder; //0 ascending, 1 descending

    };


    struct global_
    {
        bool delete_locked;             // delete locked files on reboot, *if* admin rights
        bool hide_empty;                // hide plug-ins with 0 items to clean
        bool hide_admin;                // hide plug-ins that need admin rights, and the user is standard one
        bool show_running_processes;    // show processes that need to be closed for optimal cleaning
    };

    struct systemp_
    {
        bool delete_ro;                 // delete read-only temp files
        bool delete_subfolders;         // delete subfolders in the temp folder
        long int min_age;               // delete files only if older than ...
    };

    struct tempinternetfiles_
    {
        bool delete_offline;            // delete offline files
    };

    struct cookies_
    {
        bool use_cookie_filter;         //not implemented
        long min_cookie_age;            //not implemented
    };

    class dcsettings
    {
        public:

        global_ global;
        UI_ ui;
        systemp_ systemp;
        tempinternetfiles_ tempinternetfiles;
        cookies_ cookies;

        bool Save();
        bool Save( wxConfigBase* const config_object ) ;
        bool Load();
        bool Load( wxConfigBase* const config_object );
    };

} //namespace diskcleaner


#endif // _DCSETTINGS_H_
