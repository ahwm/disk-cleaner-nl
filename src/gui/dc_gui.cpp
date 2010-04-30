///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxCheckedListCtrl.h"
#include "wxlistctrllog.h"

#include "dc_gui.h"

///////////////////////////////////////////////////////////////////////////

dc_base_frame::dc_base_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 390,426 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 5, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer10->AddGrowableCol( 0 );
	fgSizer10->AddGrowableRow( 1 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText17 = new wxStaticText( this, wxID_ANY, _("Check the items to remove"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	m_staticText17->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText17->SetForegroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer10->Add( m_staticText17, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	plugin_listctrl = new wxCheckedListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	fgSizer10->Add( plugin_listctrl, 1, wxEXPAND|wxALL, 5 );
	
	fgSizer1->Add( fgSizer10, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer9->AddGrowableCol( 0 );
	fgSizer9->AddGrowableRow( 1 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, _("Presets"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	m_staticText16->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText16->SetForegroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer9->Add( m_staticText16, 0, wxALL, 5 );
	
	m_staticText20 = new wxStaticText( this, wxID_ANY, _("Program"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	m_staticText20->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText20->SetForegroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer9->Add( m_staticText20, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 1, 4, 0, 0 );
	fgSizer2->AddGrowableCol( 0 );
	fgSizer2->AddGrowableCol( 3 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString preset_boxChoices;
	preset_box = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, preset_boxChoices, 0 );
	preset_box->SetSelection( 0 );
	fgSizer2->Add( preset_box, 1, wxALL|wxEXPAND, 5 );
	
	preset_save_btn = new wxButton( this, wxID_ANY, _("&Save..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	fgSizer2->Add( preset_save_btn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2 );
	
	preset_delete_btn = new wxButton( this, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	fgSizer2->Add( preset_delete_btn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	fgSizer9->Add( fgSizer2, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	config_btn = new wxButton( this, wxID_ANY, _("Options..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer5->Add( config_btn, 1, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 2 );
	
	about_btn = new wxButton( this, wxID_ANY, _("About..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer5->Add( about_btn, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	fgSizer9->Add( bSizer5, 0, wxEXPAND|wxLEFT, 10 );
	
	fgSizer1->Add( fgSizer9, 1, wxEXPAND|wxBOTTOM|wxRIGHT, 5 );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, _("Actions"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	m_staticText18->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText18->SetForegroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer1->Add( m_staticText18, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	clean_btn = new wxButton( this, wxID_OK, _("&Clean"), wxDefaultPosition, wxDefaultSize, 0 );
	clean_btn->SetDefault(); 
	bSizer3->Add( clean_btn, 0, wxEXPAND|wxTOP, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	runasadmin_btn = new wxButton( this, wxID_ANY, _("&Run as Administrator"), wxDefaultPosition, wxDefaultSize, 0 );
	runasadmin_btn->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer3->Add( runasadmin_btn, 0, wxEXPAND|wxTOP, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	exit_btn = new wxButton( this, wxID_CANCEL, _("&Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( exit_btn, 0, wxEXPAND|wxTOP, 5 );
	
	fgSizer1->Add( bSizer3, 0, wxEXPAND|wxALL, 10 );
	
	bSizer17->Add( fgSizer1, 1, wxEXPAND|wxRIGHT|wxLEFT, 10 );
	
	this->SetSizer( bSizer17 );
	this->Layout();
	context_menu = new wxMenu();
	wxMenuItem* m_select_all;
	m_select_all = new wxMenuItem( context_menu, wxID_ANY, wxString( _("Select All") ) , wxEmptyString, wxITEM_NORMAL );
	context_menu->Append( m_select_all );
	
	wxMenuItem* m_select_none;
	m_select_none = new wxMenuItem( context_menu, wxID_ANY, wxString( _("Select None") ) , wxEmptyString, wxITEM_NORMAL );
	context_menu->Append( m_select_none );
	
	wxMenuItem* m_invert_selection;
	m_invert_selection = new wxMenuItem( context_menu, wxID_ANY, wxString( _("Invert Selection") ) , wxEmptyString, wxITEM_NORMAL );
	context_menu->Append( m_invert_selection );
	
	this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( dc_base_frame::dc_base_frameOnContextMenu ), NULL, this ); 
	
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( dc_base_frame::dc_base_frame_onclose ) );
	plugin_listctrl->Connect( wxEVT_KEY_UP, wxKeyEventHandler( dc_base_frame::plugin_listctrl_keyup ), NULL, this );
	plugin_listctrl->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( dc_base_frame::plugin_listctrl_column_clicked ), NULL, this );
	preset_box->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( dc_base_frame::preset_box_onchoice ), NULL, this );
	preset_save_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::preset_save_btn_click ), NULL, this );
	preset_delete_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::preset_delete_btn_click ), NULL, this );
	config_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::config_btn_click ), NULL, this );
	about_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::about_btn_click ), NULL, this );
	clean_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::clean_btn_click ), NULL, this );
	runasadmin_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::runasadmin_btn_click ), NULL, this );
	exit_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::exit_btn_click ), NULL, this );
	this->Connect( m_select_all->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dc_base_frame::select_all_click ) );
	this->Connect( m_select_none->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dc_base_frame::select_none_click ) );
	this->Connect( m_invert_selection->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dc_base_frame::invert_selection_click ) );
}

dc_base_frame::~dc_base_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( dc_base_frame::dc_base_frame_onclose ) );
	plugin_listctrl->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( dc_base_frame::plugin_listctrl_keyup ), NULL, this );
	plugin_listctrl->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( dc_base_frame::plugin_listctrl_column_clicked ), NULL, this );
	preset_box->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( dc_base_frame::preset_box_onchoice ), NULL, this );
	preset_save_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::preset_save_btn_click ), NULL, this );
	preset_delete_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::preset_delete_btn_click ), NULL, this );
	config_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::config_btn_click ), NULL, this );
	about_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::about_btn_click ), NULL, this );
	clean_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::clean_btn_click ), NULL, this );
	runasadmin_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::runasadmin_btn_click ), NULL, this );
	exit_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dc_base_frame::exit_btn_click ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dc_base_frame::select_all_click ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dc_base_frame::select_none_click ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( dc_base_frame::invert_selection_click ) );
	
	delete context_menu; 
}

wait_base_dlg::wait_base_dlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, _("Please wait, Disk Cleaner is scanning the system."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer6->Add( m_staticText7, 1, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 1, 3, 0, 0 );
	fgSizer7->AddGrowableCol( 1 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, _("0 %"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer7->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	progress_bar = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,20 ), wxGA_HORIZONTAL );
	progress_bar->SetMaxSize( wxSize( -1,20 ) );
	
	fgSizer7->Add( progress_bar, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 10 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("100 %"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer7->Add( m_staticText9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	fgSizer6->Add( fgSizer7, 0, wxEXPAND, 5 );
	
	this->SetSizer( fgSizer6 );
	this->Layout();
}

wait_base_dlg::~wait_base_dlg()
{
}

about_base_dlg::about_base_dlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bitmap3 = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("bmplogo"), wxBITMAP_TYPE_RESOURCE ), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( m_bitmap3, 0, 0, 10 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, _("A fast open source disk cleanup tool"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	bSizer7->Add( m_staticText16, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	version_text = new wxStaticText( this, wxID_ANY, _("Version"), wxDefaultPosition, wxDefaultSize, 0 );
	version_text->Wrap( -1 );
	bSizer7->Add( version_text, 0, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Copyright 1999-2010 R. J. Moerland"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer7->Add( m_staticText3, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_hyperlink1 = new wxHyperlinkCtrl( this, wxID_ANY, _("http://www.diskcleaner.nl"), wxT("http://www.diskcleaner.nl"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer7->Add( m_hyperlink1, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, _("Released under the GPL v2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer7->Add( m_staticText6, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	ok_sizer = new wxStdDialogButtonSizer();
	ok_sizerOK = new wxButton( this, wxID_OK );
	ok_sizer->AddButton( ok_sizerOK );
	ok_sizer->Realize();
	bSizer7->Add( ok_sizer, 1, wxTOP|wxRIGHT|wxALIGN_RIGHT, 10 );
	
	fgSizer8->Add( bSizer7, 1, wxEXPAND|wxBOTTOM|wxLEFT, 10 );
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
	// Connect Events
	ok_sizerOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( about_base_dlg::ok_btn_clicked ), NULL, this );
}

about_base_dlg::~about_base_dlg()
{
	// Disconnect Events
	ok_sizerOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( about_base_dlg::ok_btn_clicked ), NULL, this );
	
}

result_base_frame::result_base_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 330,300 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText19 = new wxStaticText( this, wxID_ANY, _("Cleaning results"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	m_staticText19->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText19->SetForegroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer1->Add( m_staticText19, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	result_lc = new wxListCtrlLog( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER|wxLC_REPORT );
	fgSizer1->Add( result_lc, 1, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	back_btn = new wxButton( this, wxID_ANY, _("&Back"), wxDefaultPosition, wxDefaultSize, 0 );
	back_btn->SetDefault(); 
	bSizer3->Add( back_btn, 0, wxALL, 5 );
	
	save_btn = new wxButton( this, wxID_ANY, _("&Save results..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( save_btn, 0, wxALL, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	exit_btn = new wxButton( this, wxID_OK, _("&Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	exit_btn->SetDefault(); 
	bSizer3->Add( exit_btn, 0, wxALL, 5 );
	
	fgSizer1->Add( bSizer3, 0, wxEXPAND, 5 );
	
	bSizer18->Add( fgSizer1, 1, wxEXPAND|wxRIGHT|wxLEFT, 10 );
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( result_base_frame::init_result_dlg ) );
	back_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( result_base_frame::back_btn_click ), NULL, this );
	save_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( result_base_frame::save_btn_click ), NULL, this );
	exit_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( result_base_frame::exit_btn_click ), NULL, this );
}

result_base_frame::~result_base_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( result_base_frame::init_result_dlg ) );
	back_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( result_base_frame::back_btn_click ), NULL, this );
	save_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( result_base_frame::save_btn_click ), NULL, this );
	exit_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( result_base_frame::exit_btn_click ), NULL, this );
	
}

prefs_dlg_base::prefs_dlg_base( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	prefsbook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel1 = new wxPanel( prefsbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	delete_locked_cb = new wxCheckBox( m_panel1, wxID_ANY, _("&Delete locked files on reboot (needs administrator privileges)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( delete_locked_cb, 0, wxALL, 10 );
	
	hide_admin_items_cb = new wxCheckBox( m_panel1, wxID_ANY, _("For normal users, &hide items that need administrator priviliges"), wxDefaultPosition, wxDefaultSize, 0 );
	hide_admin_items_cb->Hide();
	
	bSizer6->Add( hide_admin_items_cb, 0, wxBOTTOM|wxRIGHT|wxLEFT, 10 );
	
	hide_empty_cb = new wxCheckBox( m_panel1, wxID_ANY, _("&Hide entries when empty (requires restart of Disk Cleaner)"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( hide_empty_cb, 0, wxBOTTOM|wxRIGHT|wxLEFT, 10 );
	
	m_panel1->SetSizer( bSizer6 );
	m_panel1->Layout();
	bSizer6->Fit( m_panel1 );
	prefsbook->AddPage( m_panel1, _("Global"), true );
	m_panel3 = new wxPanel( prefsbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	delete_readonly_cb = new wxCheckBox( m_panel3, wxID_ANY, _("&Delete read-only files"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( delete_readonly_cb, 0, wxALL, 10 );
	
	delete_emptyfolder_cb = new wxCheckBox( m_panel3, wxID_ANY, _("Delete &empty subfolders"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( delete_emptyfolder_cb, 0, wxBOTTOM|wxRIGHT|wxLEFT, 10 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText12 = new wxStaticText( m_panel3, wxID_ANY, _("Minimum age before removal (days)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	bSizer10->Add( m_staticText12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 10 );
	
	wxString minage_comboChoices[] = { _("0"), _("1"), _("2"), _("5"), _("10"), _("20"), _("50") };
	int minage_comboNChoices = sizeof( minage_comboChoices ) / sizeof( wxString );
	minage_combo = new wxChoice( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, minage_comboNChoices, minage_comboChoices, 0 );
	minage_combo->SetSelection( 0 );
	bSizer10->Add( minage_combo, 0, wxALL, 10 );
	
	bSizer12->Add( bSizer10, 0, wxEXPAND, 5 );
	
	m_panel3->SetSizer( bSizer12 );
	m_panel3->Layout();
	bSizer12->Fit( m_panel3 );
	prefsbook->AddPage( m_panel3, _("System temporary files"), false );
	m_panel4 = new wxPanel( prefsbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel4->Hide();
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	tempinet_offline_cb = new wxCheckBox( m_panel4, wxID_ANY, _("&Delete off-line items"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( tempinet_offline_cb, 0, wxALL|wxEXPAND, 10 );
	
	m_panel4->SetSizer( bSizer14 );
	m_panel4->Layout();
	bSizer14->Fit( m_panel4 );
	prefsbook->AddPage( m_panel4, _("Internet Explorer Cache"), false );
	
	bSizer4->Add( prefsbook, 1, wxALL|wxEXPAND, 5 );
	
	ok_cancel = new wxStdDialogButtonSizer();
	ok_cancelOK = new wxButton( this, wxID_OK );
	ok_cancel->AddButton( ok_cancelOK );
	ok_cancelCancel = new wxButton( this, wxID_CANCEL );
	ok_cancel->AddButton( ok_cancelCancel );
	ok_cancel->Realize();
	bSizer4->Add( ok_cancel, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	
	// Connect Events
	ok_cancelCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( prefs_dlg_base::cancel_btn_clicked ), NULL, this );
	ok_cancelOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( prefs_dlg_base::ok_btn_clicked ), NULL, this );
}

prefs_dlg_base::~prefs_dlg_base()
{
	// Disconnect Events
	ok_cancelCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( prefs_dlg_base::cancel_btn_clicked ), NULL, this );
	ok_cancelOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( prefs_dlg_base::ok_btn_clicked ), NULL, this );
	
}

MyDialog6::MyDialog6( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel2 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel2->Hide();
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText11 = new wxStaticText( m_panel2, wxID_ANY, _("Select a preset and click 'Install' to auto-run Disk Cleaner in quiet mode on start-up with the selected preset. Click 'Remove' to remove the shortcut."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer7->Add( m_staticText11, 1, wxALL|wxEXPAND, 10 );
	
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString preset_comboChoices;
	preset_combo = new wxChoice( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, preset_comboChoices, 0 );
	preset_combo->SetSelection( 0 );
	bSizer161->Add( preset_combo, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 10 );
	
	autostart_install_btn = new wxButton( m_panel2, wxID_ANY, _("&Install"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( autostart_install_btn, 0, wxTOP|wxBOTTOM, 10 );
	
	autostart_remove_btn = new wxButton( m_panel2, wxID_ANY, _("&Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( autostart_remove_btn, 0, wxALL, 10 );
	
	bSizer7->Add( bSizer161, 0, wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer7 );
	m_panel2->Layout();
	bSizer7->Fit( m_panel2 );
	m_notebook2->AddPage( m_panel2, _("Autostart"), false );
	m_panel5 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel5->Hide();
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 4, 1, 0, 0 );
	fgSizer8->AddGrowableCol( 0 );
	fgSizer8->AddGrowableRow( 2 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	cookie_filter_cb = new wxCheckBox( m_panel5, wxID_ANY, _("Use cookie filtering"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( cookie_filter_cb, 0, wxALL, 15 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( m_panel5, wxID_ANY, _("Keep cookies for "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer15->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString cookie_age_comboChoices[] = { _("0"), _("1"), _("2"), _("5"), _("10"), _("20"), _("50") };
	int cookie_age_comboNChoices = sizeof( cookie_age_comboChoices ) / sizeof( wxString );
	cookie_age_combo = new wxChoice( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, cookie_age_comboNChoices, cookie_age_comboChoices, 0 );
	cookie_age_combo->SetSelection( 0 );
	bSizer15->Add( cookie_age_combo, 0, wxALL, 5 );
	
	m_staticText14 = new wxStaticText( m_panel5, wxID_ANY, _("days"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizer15->Add( m_staticText14, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	fgSizer8->Add( bSizer15, 1, wxEXPAND|wxLEFT, 10 );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 1, 3, 0, 0 );
	fgSizer9->AddGrowableCol( 0 );
	fgSizer9->AddGrowableCol( 2 );
	fgSizer9->AddGrowableRow( 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText15 = new wxStaticText( m_panel5, wxID_ANY, _("Protected cookies:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	bSizer17->Add( m_staticText15, 0, wxALL, 5 );
	
	m_listBox2 = new wxListBox( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer17->Add( m_listBox2, 1, wxALL|wxEXPAND, 5 );
	
	fgSizer9->Add( bSizer17, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer18->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer18->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_button12 = new wxButton( m_panel5, wxID_ANY, _("&<"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer18->Add( m_button12, 0, wxALL, 5 );
	
	m_button13 = new wxButton( m_panel5, wxID_ANY, _("&>"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	bSizer18->Add( m_button13, 0, wxALL, 5 );
	
	
	bSizer18->Add( 0, 0, 1, wxEXPAND, 5 );
	
	fgSizer9->Add( bSizer18, 1, wxEXPAND, 10 );
	
	wxBoxSizer* bSizer171;
	bSizer171 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText151 = new wxStaticText( m_panel5, wxID_ANY, _("Unprotected cookies:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText151->Wrap( -1 );
	bSizer171->Add( m_staticText151, 0, wxALL, 5 );
	
	m_listBox21 = new wxListBox( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer171->Add( m_listBox21, 1, wxALL|wxEXPAND, 5 );
	
	fgSizer9->Add( bSizer171, 1, wxEXPAND, 5 );
	
	fgSizer8->Add( fgSizer9, 1, wxLEFT|wxEXPAND, 10 );
	
	m_staticText18 = new wxStaticText( m_panel5, wxID_ANY, _("Protected cookies are never deleted"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer8->Add( m_staticText18, 0, wxALL, 15 );
	
	m_panel5->SetSizer( fgSizer8 );
	m_panel5->Layout();
	fgSizer8->Fit( m_panel5 );
	m_notebook2->AddPage( m_panel5, _("Cookies"), false );
	
	bSizer16->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer16 );
	this->Layout();
}

MyDialog6::~MyDialog6()
{
}
