#ifndef WXCHECKEDLISTCTRL_H
#define WXCHECKEDLISTCTRL_H

#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include <wx/icon.h>

class wxCheckedListCtrl : public wxListCtrl
{

    DECLARE_EVENT_TABLE()

    wxImageList m_imageList;

public:
    DECLARE_DYNAMIC_CLASS( wxCheckedListCtrl )
    wxCheckedListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
                                         const wxSize& sz, long style);
    void OnMouseEvent(wxMouseEvent& event);
    void OnChar(wxKeyEvent& event);
    bool SetBackgroundImage( const wchar_t* resource );
    bool IsChecked(long item) const ;
    void Check(long item, bool checked);
};
#endif
