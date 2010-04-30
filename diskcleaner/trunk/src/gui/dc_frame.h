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

#ifndef __dc_frame__
#define __dc_frame__

/**
@file
Subclass of dc_base_frame, which is generated by wxFormBuilder.
*/

#include "gui/dc_gui.h"
#include "gui/dcsettings.h"
#include "gui/dcpresets.h"
#include "core/plugin_info_base.h"

#include <vector>
#include <memory>

//Forward declaration
class dcApp;

class result_frame;

/** Implementing dc_base_frame */
class dc_frame : public dc_base_frame
{
    std::auto_ptr<result_frame> rsframe;
protected:

    //Manages all user settings that are persistent
    diskcleaner::dcsettings settings;

    //Class that manages (un)checking the items in the main
    //window's list control, depending on the chosen preset
    std::auto_ptr<diskcleaner::dcpreset_handler> ppreset_handler;

	// Handlers for dc_base_frame events.
	void preset_save_btn_click( wxCommandEvent& event );
	void preset_delete_btn_click( wxCommandEvent& event );
	void config_btn_click( wxCommandEvent& event );
	void clean_btn_click( wxCommandEvent& event );
	void runasadmin_btn_click( wxCommandEvent& event );
	void about_btn_click( wxCommandEvent& event );
	void exit_btn_click( wxCommandEvent& event );
	void plugin_checkbox_toggled( wxCommandEvent& event );
	void dc_base_frame_onclose( wxCloseEvent& event  );
    void preset_box_onchoice( wxCommandEvent& event );
    void plugin_listctrl_column_clicked( wxListEvent& event );
    void select_all_click( wxCommandEvent& event );
    void select_none_click( wxCommandEvent& event );
    void invert_selection_click( wxCommandEvent& event );

	//Restarts Disk Cleaner, possibly as administrator
	//Used for 'Run as Administrator' button in the main window
	//and for the 'Back' button in the result window
	void run_diskcleaner( bool as_admin );

	//Adds the plugin to the list control, with all the information
	//it contains.
	void add_plugin_to_listctrl( diskcleaner::PlugInfo * pi);

	wxString bytes_to_string( __int64 bytes );


public:
	/** Constructor */
	dc_frame( wxWindow* parent );

	// Start actual cleaning
	void clean(__int64& total_files, __int64& total_bytes);
	void init_dialog();

	// For result_frame instances only
	void result_frame_finished_signal( bool restart );
};

#endif // __dc_frame__
