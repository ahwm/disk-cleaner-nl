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
    /// A struct to store the sizes of windows on screen
    struct framesize_
    {
        int width;
        int height;
        int topx;
        int topy;
    };

    /// A struct to keep all user interface layout related stuff together
    struct UI_
    {
        framesize_ dc_frame_size;
        framesize_ result_frame_size;
        int col_sdesc_width,    ///< Short description column width
            col_ldesc_width,    ///< Long description column width
            col_item_width,     ///< Number of items column width
            col_size_width;     ///< Number of bytes column width
        int sort_column;        ///< Column that user wants items sorted on
        int sortorder;          ///< Sortorder, 0 is ascending, 1 is descending

    };

    /// Struct for 'global' preferences, that is, not related to a specific plug-in
    struct global_
    {
        bool delete_locked;             ///< Delete locked files on reboot, *if* the user has admin rights.
        bool hide_empty;                ///< Hide plug-ins with 0 items to clean.
        bool hide_admin;                ///< Hide plug-ins that need admin rights, and the user is standard one.
        bool show_running_processes;    ///< Show processes that need to be closed for optimal cleaning.
    };

    /// Struct to store preferences related to system temp folder cleaning
    struct systemp_
    {
        bool delete_ro;                 ///< delete read-only temp files
        bool delete_subfolders;         ///< delete subfolders in the temp folder
        long int min_age;               ///< delete files only if older than ...
    };

    /// Struct to store preferences related to internet explorer cache files
    struct tempinternetfiles_
    {
        bool delete_offline;            ///< delete offline files
    };

    struct cookies_
    {
        bool use_cookie_filter;         //not implemented
        long min_cookie_age;            //not implemented
    };

    /// Class that combines all user preferences and defines methods to load and store them on disk
    class dcsettings
    {
        public:

        global_             global;
        UI_                 ui;
        systemp_            systemp;
        tempinternetfiles_  tempinternetfiles;
        cookies_            cookies;

        /// Save user's preferences, get the config object via wxConfigBase::Get()
        bool Save();
        /// Save user's preferences, use config_object
        bool Save( wxConfigBase* const config_object ) ;
        /// Load user's preferences, get the config object via wxConfigBase::Get()
        bool Load();
        /// Load user's preferences, use config_object
        bool Load( wxConfigBase* const config_object );
    };

} //namespace diskcleaner


#endif // _DCSETTINGS_H_
