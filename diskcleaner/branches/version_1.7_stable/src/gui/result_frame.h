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

#ifndef __result_frame__
#define __result_frame__

/**
@file
Subclass of result_base_frame, which is generated by wxFormBuilder.
*/

#include <windows.h>
#include "dc_gui.h"
class wxListCtrlLog;
class dc_frame;

/** Implementing result_base_frame */
class result_frame : public result_base_frame
{
    HWND result_lc_hwnd;

protected:
	// Handlers for result_base_frame events.
	void back_btn_click( wxCommandEvent& event );
	void exit_btn_click( wxCommandEvent& event );
	void init_result_dlg( wxInitDialogEvent& event ){};
	void save_btn_click( wxCommandEvent& event );
	void handle_mousewheel (wxMouseEvent &event);

public:
	/** Constructor */
	result_frame( wxWindow* parent );
	~result_frame();

	dc_frame* parent_frame;
	void enable_controls();
	void disable_controls();

};

#endif // __result_frame__