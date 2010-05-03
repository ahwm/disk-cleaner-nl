///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dc_gui__
#define __dc_gui__

#include <wx/intl.h>

class wxCheckedListCtrl;
class wxListCtrlLog;

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/dialog.h>
#include <wx/gauge.h>
#include <wx/statbmp.h>
#include <wx/hyperlink.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/listbox.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class dc_base_frame
///////////////////////////////////////////////////////////////////////////////
class dc_base_frame : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText17;
		wxCheckedListCtrl* plugin_listctrl;
		wxStaticText* m_staticText16;
		wxStaticText* m_staticText20;
		wxChoice* preset_box;
		wxButton* preset_save_btn;
		wxButton* preset_delete_btn;
		
		wxButton* config_btn;
		wxButton* about_btn;
		wxStaticText* m_staticText18;
		wxButton* clean_btn;
		
		wxButton* runasadmin_btn;
		
		wxButton* exit_btn;
		wxMenu* context_menu;
		
		// Virtual event handlers, overide them in your derived class
		virtual void dc_base_frame_onclose( wxCloseEvent& event ) { event.Skip(); }
		virtual void plugin_listctrl_keyup( wxKeyEvent& event ) { event.Skip(); }
		virtual void plugin_listctrl_column_clicked( wxListEvent& event ) { event.Skip(); }
		virtual void preset_box_onchoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void preset_save_btn_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void preset_delete_btn_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void config_btn_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void about_btn_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void clean_btn_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void runasadmin_btn_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void exit_btn_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void select_all_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void select_none_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void invert_selection_click( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		dc_base_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Disk Cleaner"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,426 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~dc_base_frame();
		
		void dc_base_frameOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( context_menu, event.GetPosition() );
		}
	
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
		
		wait_base_dlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Please wait..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 260,103 ), long style = wxDEFAULT_DIALOG_STYLE );
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
		virtual void ok_btn_clicked( wxCommandEvent& event ) { event.Skip(); }
		
	
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
		wxStaticText* m_staticText19;
		wxListCtrlLog* result_lc;
		wxButton* back_btn;
		wxButton* save_btn;
		
		wxButton* exit_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void init_result_dlg( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void back_btn_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void save_btn_click( wxCommandEvent& event ) { event.Skip(); }
		virtual void exit_btn_click( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		result_base_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 440,318 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
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
		wxCheckBox* hide_admin_items_cb;
		wxCheckBox* warn_open_processes_cb;
		wxPanel* m_panel3;
		wxCheckBox* delete_readonly_cb;
		wxCheckBox* delete_emptyfolder_cb;
		wxStaticText* m_staticText12;
		wxChoice* minage_combo;
		wxPanel* m_panel4;
		wxCheckBox* tempinet_offline_cb;
		wxStdDialogButtonSizer* ok_cancel;
		wxButton* ok_cancelOK;
		wxButton* ok_cancelCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void cancel_btn_clicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ok_btn_clicked( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		prefs_dlg_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 467,368 ), long style = wxDEFAULT_DIALOG_STYLE|wxCLIP_CHILDREN );
		~prefs_dlg_base();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyDialog6
///////////////////////////////////////////////////////////////////////////////
class MyDialog6 : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook2;
		wxPanel* m_panel2;
		wxStaticText* m_staticText11;
		wxChoice* preset_combo;
		wxButton* autostart_install_btn;
		wxButton* autostart_remove_btn;
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
	
	public:
		
		MyDialog6( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 377,302 ), long style = wxDEFAULT_DIALOG_STYLE );
		~MyDialog6();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class processes_dlg_base
///////////////////////////////////////////////////////////////////////////////
class processes_dlg_base : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText20;
		wxStaticText* m_staticText21;
		wxListBox* process_list_lb;
		wxCheckBox* dont_show_cb;
		wxButton* m_button15;
		
		// Virtual event handlers, overide them in your derived class
		virtual void ok_btn_click( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		processes_dlg_base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Disk Cleaner Information"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 272,219 ), long style = wxDEFAULT_DIALOG_STYLE );
		~processes_dlg_base();
	
};

#endif //__dc_gui__
