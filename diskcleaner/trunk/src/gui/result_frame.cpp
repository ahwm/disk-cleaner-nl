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

#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>

#include "result_frame.h"
#include "gui/wxlistctrllog.h"
#include "gui/dc_frame.h"

result_frame::result_frame( wxWindow* parent )
:
result_base_frame( parent )
{
  result_lc_hwnd = static_cast<HWND>( result_lc->GetHandle() );

#ifdef __WXDEBUG__
  result_lc->SetDebugFile( wxStandardPaths::Get().GetUserConfigDir()  + L"\\Disk Cleaner" );
#endif

  // Set focus on exit button
  exit_btn->SetFocus();

  // Set parent frame
  parent_frame = dynamic_cast<dc_frame*>( parent );


}

void result_frame::save_btn_click( wxCommandEvent& event )
{
    //Save cleaning results to a file
    wxFileDialog SaveDialog( this,
                            _("Choose a location to save the results"), wxEmptyString, wxEmptyString,
                            _("Log files (*.log)|*.log"), wxFD_SAVE, wxDefaultPosition);

	if (SaveDialog.ShowModal() == wxID_OK) // if the user click "Open" instead of "Cancel"
	{
		wxString filename = SaveDialog.GetPath();
		wxTextFile resultfile( filename );
        ::wxLogDebug( L"%hs: GetFileName() returned %s", __FUNCTION__, filename.c_str() );

        if( !resultfile.Create() && !resultfile.Open() )
        {
            wxMessageBox( _("Sorry, I couldn't write to that file. Please try another name or location"),
                        L"", wxOK, this );
            return;
        }

        resultfile.Clear();
        int max = result_lc->GetItemCount();
        ::wxLogDebug( L"result_lc->GetItemCount() max = %d", max );

        for (int k = 0; k < max; ++k )
        {
            resultfile.AddLine( result_lc->GetItemText( k ) );
        }

        wxLogDebug( L"Writing to log file..." );
        resultfile.Write();
	}
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

void result_frame::enable_controls()
{
  back_btn->Enable( true );
  exit_btn->Enable( true );
  save_btn->Enable( true );

  progress_bar->Hide();
}

void result_frame::disable_controls()
{
  back_btn->Enable( false );
  exit_btn->Enable( false );
  save_btn->Enable( false );
}

void result_frame::handle_mousewheel(wxMouseEvent &event)
{
    SendMessage( result_lc_hwnd, WM_MOUSEWHEEL, MAKEWPARAM( 0, event.m_wheelRotation), MAKELPARAM( event.GetX(), event.GetY() ) );
}

void result_frame::set_progress_range( int max_range )
{
    progress_bar->SetRange( max_range );
}

void result_frame::Increment()
{
    progress_bar->SetValue( progress_bar->GetValue() + 1 );
}
