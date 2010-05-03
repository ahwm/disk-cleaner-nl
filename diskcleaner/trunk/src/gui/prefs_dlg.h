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

#ifndef __prefs_dlg__
#define __prefs_dlg__

#include "dc_gui.h"
#include "dcsettings.h"

/** Implementing prefs_dlg_base */
class prefs_dlg : public prefs_dlg_base
{
private:
    //Hide this constructor, a dcsettings ref should be provided (see below)
    prefs_dlg( wxWindow* parent );

protected:

    diskcleaner::dcsettings& rsettings;

    void InitializePreferencesDialog(diskcleaner::dcsettings& prefs);

public:
	/** Constructor */

	prefs_dlg( wxWindow* parent, diskcleaner::dcsettings& prefs );

	// Virtual event handlers, overide them in your derived class
    virtual void cancel_btn_clicked( wxCommandEvent& event );
	virtual void ok_btn_clicked( wxCommandEvent& event );



};

#endif // __prefs_dlg__
