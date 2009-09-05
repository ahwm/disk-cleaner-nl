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

#include "about_frame.h"
#include "version.h"
#include <string>

about_frame::about_frame( wxWindow* parent )
:
about_base_dlg( parent )
{
    wxString text;
    text.sprintf( L"Version %d.%d (build %d)", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::BUILD  );

    version_text->SetLabel( text );

    ok_sizerOK->SetFocus();
}

void about_frame::ok_btn_clicked( wxCommandEvent& event )
{
	Close();
}
