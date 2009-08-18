#include "wxCheckedListCtrl.h"
#include <wx/dcmemory.h>
#include <wx/renderer.h>

/* XPM */
static char * down_xpm[] = {
"16 16 3 1",
" 	c None",
".	c Black",
"X	c Gray100",
"  ..........    ",
"   ........     ",
"    ......      ",
"     ....       ",
"      ..        ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                "};

/* XPM */
static char * up_xpm[] = {
"16 16 3 1",
" 	c None",
".	c Black",
"X	c Gray100",
"                ",
"      ..        ",
"     ....       ",
"    ......      ",
"   ........     ",
"  ..........    ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                ",
"                "};



IMPLEMENT_CLASS(wxCheckedListCtrl, wxListCtrl)

BEGIN_EVENT_TABLE(wxCheckedListCtrl, wxListCtrl)
    EVT_LEFT_DOWN(wxCheckedListCtrl::OnMouseEvent)
END_EVENT_TABLE()


wxCheckedListCtrl::wxCheckedListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
                                     const wxSize& sz, long style):
        wxListCtrl(parent, id, pt, sz, style), m_imageList(16, 16, TRUE)
{
    AssignImageList( &m_imageList, wxIMAGE_LIST_SMALL );

//    // Checked
//        wxRendererNative::Get().DrawCheckBox(this, mdc, wxRect(0, 0, 16, 16), wxCONTROL_CHECKED);
//        // Undetermined
//        wxRendererNative::Get().DrawCheckBox(this, dc, wxRect(40, 170, 16, 16), wxCONTROL_CHECKABLE);
//        // Unchecked
//        wxRendererNative::Get().DrawCheckBox(this, dc, wxRect(60, 170, 16, 16), 0);

    wxBitmap bm_unchecked( 16, 16 );
    wxMemoryDC mdc( bm_unchecked );
    wxRendererNative::Get().DrawCheckBox(this, mdc, wxRect( 0, 0, 16, 16 ), 0);
    mdc.SelectObject( ::wxNullBitmap );
    m_imageList.Add( bm_unchecked );

    wxBitmap bm_checked( 16, 16 );
    mdc.SelectObject( bm_checked );
    wxRendererNative::Get().DrawCheckBox(this, mdc, wxRect( 0, 0, 16, 16 ), wxCONTROL_CHECKED );
    mdc.SelectObject( wxNullBitmap );
    m_imageList.Add( bm_checked );

    wxIcon ic_up( up_xpm );
    m_imageList.Add( ic_up );

    wxIcon ic_down( down_xpm );
    m_imageList.Add( ic_down );

}

void wxCheckedListCtrl::OnMouseEvent(wxMouseEvent& event)
{
    if (event.LeftDown())
    {
        int flags;
        long item = HitTest(event.GetPosition(), flags);
        if (item > -1 && (flags & wxLIST_HITTEST_ONITEMICON))
        {
            Check(item, !IsChecked(item));
        }
        else
            event.Skip();
    }
    else
    {
        event.Skip();
    }
}

bool wxCheckedListCtrl::IsChecked(long item) const
{
    wxListItem info;
    info.m_mask = wxLIST_MASK_IMAGE ;
    info.m_itemId = item;

    if (GetItem(info))
    {
        return (info.m_image == 1 );
    }
    else
        return FALSE;
}

void wxCheckedListCtrl::Check(long item, bool checked)
{
    SetItemImage(item, (checked ? 1 : 0), -1);
}
