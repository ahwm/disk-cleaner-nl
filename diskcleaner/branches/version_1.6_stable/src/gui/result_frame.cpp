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

#include "result_frame.h"
#include "gui/wxlistctrllog.h"

#include "gui/dc_frame.h"

result_frame::result_frame( wxWindow* parent )
:
result_base_frame( parent )
{
  // Set focus on exit button
  exit_btn->SetFocus();

  // Set parent frame
  parent_frame = dynamic_cast<dc_frame*>( parent );
}

void result_frame::back_btn_click( wxCommandEvent& event )
{
	// Signal our parent that the user wants to go back (rescan)
	parent_frame->result_frame_finished_signal( true );

	Close();
}

void result_frame::exit_btn_click( wxCommandEvent& event )
{
	// Signal our parent that the user wants to exit
	parent_frame->result_frame_finished_signal( false );

	Close();
}

result_frame::~result_frame()
{
    wxLogDebug( L"%hs called", __FUNCTION__ );
}

void result_frame::EnableControls()
{
  back_btn->Enable( true );
  exit_btn->Enable( true );
}

void result_frame::DisableControls()
{
  back_btn->Enable( false );
  exit_btn->Enable( false );
}
