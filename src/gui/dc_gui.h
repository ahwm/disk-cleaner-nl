///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dc_gui__
#define __dc_gui__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/checklst.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/gauge.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/hyperlink.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/notebook.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class dc_base_frame
///////////////////////////////////////////////////////////////////////////////
class dc_base_frame : public wxDialog 
{
	private:
	
	protected:
		wxCheckListBox* plugin_checkbox;
		wxStaticText* description_label;
		wxTextCtrl* description_box;
		wxStaticText* items_selected_text;
		wxChoice* preset_box;
		wxButton* preset_save_btn;
		wxButton* preset_delete_btn;
		wxButton* clean_btn;
		
		wxButton* runasadmin_btn;
		
		wxButton* exit_btn;
		wxButton* about_btn;
		wxButton* config_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void dc_base_frame_onclose( wxCloseEvent& event ){ event.Skip(); }
		virtual void plugin_checkbox_itemselected( wxCommandEvent& event ){ event.Skip(); }
		virtual void plugin_checkbox_dblclick( wxCommandEvent& event ){ event.Skip(); }
		virtual void plugin_checkbox_toggled( wxCommandEvent& event ){ event.Skip(); }
		virtual void preset_save_btn_click( wxCommandEvent& event ){ event.Skip(); }
		virtual void preset_delete_btn_click( wxCommandEvent& event ){ event.Skip(); }
		virtual void clean_btn_click( wxCommandEvent& event ){ event.Skip(); }
		virtual void runasadmin_btn_click( wxCommandEvent& event ){ event.Skip(); }
		virtual void exit_btn_click( wxCommandEvent& event ){ event.Skip(); }
		virtual void about_btn_click( wxCommandEvent& event ){ event.Skip(); }
		virtual void config_btn_click( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		dc_base_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Disk Cleaner"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 390,426 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~dc_base_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class wait_base_dlg
///////////////////////////////////////////////////////////////////////////////
class wait_base_dlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxGauge* progress_bar;
		wxStaticText* m_staticText9;
	
	public:
		wait_base_dlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Please wait..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 251,102 ), long style = wxDEFAULT_DIALOG_STYLE );
		~wait_base_dlg();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class about_base_dlg
///////////////////////////////////////////////////////////////////////////////
class about_base_dlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticBitmap* m_bitmap3;
		wxStaticText* m_staticText16;
		wxStaticText* version_text;
		wxStaticText* m_staticText3;
		wxHyperlinkCtrl* m_hyperlink1;
		wxStaticText* m_staticText6;
		
		wxStdDialogButtonSizer* ok_sizer;
		wxButton* ok_sizerOK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void ok_btn_clicked( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		about_base_dlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~about_base_dlg();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class result_base_frame
///////////////////////////////////////////////////////////////////////////////
class result_base_frame : public wxDialog 
{
	private:
	
	protected:
		wxButton* back_btn;
		
		wxButton* exit_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void back_btn_click( wxCommandEvent& event ){ event.Skip(); }
		virtual void exit_btn_click( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxListBox* result_lb;
		result_base_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 336,300 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~result_base_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class prefs_dlg_base
///////////////////////////////////////////////////////////////////////////////
class prefs_dlg_base : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* prefsbook;
		wxPanel* m_panel1;
		wxCheckBox* delete_locked_cb;
		wxCheckBox* hide_empty_cb;
		wxCheckBox* show_description_cb;
		wxPanel* m_panel3;
		wxCheckBox* delete_readonly_cb;
		wxCheckBox* delete_emptyfolder_cb;
		wxStaticText* m_staticText12;
		wxChoice* minage_combo;
		wxPanel* m_panel4;
		wxCheckBox* tempinet_offline_cb;
		wxPanel* m_panel5;
		wxCheckBox* cookie_filter_cb;
		wxStaticText* m_staticText13;
		wxChoice* cookie_age_combo;
		wxStaticText* m_staticText14;
		wxStaticText* m_staticText15;
		wxListBox* m_listBox2;
		
		
		wxButton* m_button12;
		wxButton* m_button13;
		
		wxStaticText* m_staticText151;
		wxListBox* m_listBox21;
		wxStaticText* m_staticText18;
		wxPanel* m_panel2;
		wxStaticText* m_staticText11;
		wxChoice* preset_combo;
		wxButton* autostart_install_btn;
		wxButton* autostart_remove_btn;
		wxStdDialogButtonSizer* ok_cancel;
		wxButton* ok_cancelOK;
		wxButton* ok_cancelCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void cancel_btn_clicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void ok_btn_clicked( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		prefs_dlg_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 467,368 ), long style = wxDEFAULT_DIALOG_STYLE|wxCLIP_CHILDREN );
		~prefs_dlg_base();
	
};

#endif //__dc_gui__
