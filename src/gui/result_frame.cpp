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


result_frame::result_frame( wxWindow* parent )
:
result_base_frame( parent )
{
  rerun = false;

  CtrlLog = new wxListCtrlLog( result_lc );
}

void result_frame::back_btn_click( wxCommandEvent& event )
{
	rerun = true;
	Close();
}

void result_frame::exit_btn_click( wxCommandEvent& event )
{
	Close();
}

void result_frame::init_result_dlg( wxInitDialogEvent& event )
{
    
}
