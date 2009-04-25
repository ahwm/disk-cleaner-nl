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
#include <boost/shared_ptr.hpp>

/** Implementing dc_base_frame */
class dc_frame : public dc_base_frame
{
protected:

    std::vector<diskcleaner::PlugInfo*> plugin_list;
    diskcleaner::dcsettings settings;
    boost::shared_ptr<diskcleaner::dcpreset_handler> ppreset_handler;

	// Handlers for dc_base_frame events.
	void preset_save_btn_click( wxCommandEvent& event );
	void preset_delete_btn_click( wxCommandEvent& event );
	void config_btn_click( wxCommandEvent& event );
	void clean_btn_click( wxCommandEvent& event );
	void runasadmin_btn_click( wxCommandEvent& event );
	void about_btn_click( wxCommandEvent& event );
	void exit_btn_click( wxCommandEvent& event );
	void plugin_checkbox_itemselected( wxCommandEvent& event );
	void plugin_checkbox_toggled( wxCommandEvent& event );
	void dc_base_frame_onclose( wxCloseEvent& event  );

	void set_items_selected_text();

	void run_diskcleaner( bool as_admin );

	void add_plugin_to_lists(std::auto_ptr<diskcleaner::PlugInfo>& pi);


public:
	/** Constructor */
	dc_frame( wxWindow* parent );
	void init_dialog();
};

#endif // __dc_frame__
