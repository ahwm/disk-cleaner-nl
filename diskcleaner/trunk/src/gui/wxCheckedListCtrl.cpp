#include <windows.h>
#include <commctrl.h>

#include "wxCheckedListCtrl.h"
#include <wx/dcmemory.h>
#include <wx/renderer.h>

#define LVS_EX_DOUBLEBUFFER 0x10000
#define LVS_EX_BORDERSELECT 0x8000

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
    "                "
};

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
    "                "
};



IMPLEMENT_CLASS(wxCheckedListCtrl, wxListCtrl)

BEGIN_EVENT_TABLE(wxCheckedListCtrl, wxListCtrl)
    EVT_LEFT_DOWN(wxCheckedListCtrl::OnMouseEvent)
    EVT_CHAR(wxCheckedListCtrl::OnChar)
END_EVENT_TABLE()


wxCheckedListCtrl::wxCheckedListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
                                     const wxSize& sz, long style):
        wxListCtrl(parent, id, pt, sz, style), m_imageList(16, 16, TRUE)
{
    SetImageList( &m_imageList, wxIMAGE_LIST_SMALL );

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

    SetBackgroundImage( 0 );

//#else //Let windows handle the check boxes, set double buffer style as well
//   DWORD styles = ListView_GetExtendedListViewStyle( (HWND) this->GetHandle() );
//   ListView_SetExtendedListViewStyle( (HWND) this->GetHandle(), styles | LVS_EX_CHECKBOXES );
#ifdef __WXMSW__
    DWORD styles = ListView_GetExtendedListViewStyle( (HWND) this->GetHandle() );
    ListView_SetExtendedListViewStyle( (HWND) this->GetHandle(), styles |
                                       LVS_EX_DOUBLEBUFFER | LVS_EX_BORDERSELECT );
#endif //__WXMSW__

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

void wxCheckedListCtrl::OnChar(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_SPACE )
    {
        long itemIndex = -1;

        for (;;) {
            itemIndex = GetNextItem(itemIndex,
                                                 wxLIST_NEXT_ALL,
                                                 wxLIST_STATE_SELECTED);

            if (itemIndex == -1) break;

            // Got the selected item index
            Check(itemIndex, !IsChecked(itemIndex));
        }
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

bool wxCheckedListCtrl::SetBackgroundImage(const wchar_t* resource)

{
    // This is almost literally from Stefan Kueng's post
    // about watermarks in list controls on Windows
    // http://tortoisesvn.net/listcontrol_watermark

// add a guard here, should we ever port this to non-windows OSs
#ifdef __WXMSW__

    ListView_SetTextBkColor( (HWND) this->GetHandle(), CLR_NONE) ;
    COLORREF bkColor = ListView_GetBkColor( (HWND) this->GetHandle() );

    // create a bitmap from the icon
    HICON hIcon = (HICON)LoadImage(GetModuleHandle( NULL ),
                                   resource, IMAGE_ICON,
                                   256, 256, LR_DEFAULTCOLOR);

    if (!hIcon)
        return false;

    RECT rect = {0};

    rect.right = 256;
    rect.bottom = 256;

    HBITMAP bmp = NULL;

    HWND desktop = ::GetDesktopWindow();

    if (desktop)
    {
        HDC screen_dev = ::GetDC(desktop);

        if (screen_dev)
        {
            // Create a compatible DC
            HDC dst_hdc = ::CreateCompatibleDC(screen_dev);

            if (dst_hdc)
            {
                // Create a new bitmap of icon size
                bmp = ::CreateCompatibleBitmap(screen_dev,
                                               rect.right, rect.bottom);

                if (bmp)
                {
                    // Select it into the compatible DC
                    HBITMAP old_bmp = (HBITMAP)::SelectObject(dst_hdc, bmp);

                    // Fill the background of the compatible DC
                    // with the given colour
                    ::SetBkColor(dst_hdc,bkColor );

                    ::ExtTextOut(dst_hdc, 0, 0, ETO_OPAQUE, &rect,
                                 NULL, 0, NULL);

                    // Draw the icon into the compatible DC
                    ::DrawIconEx(dst_hdc, 0, 0, hIcon,
                                 rect.right, rect.bottom, 0,
                                 NULL, DI_NORMAL);

                }
                ::DeleteDC(dst_hdc);

            }
        }
        ::ReleaseDC(desktop, screen_dev);
    }

    // Restore settings
    DestroyIcon(hIcon);

    if (bmp == NULL)

        return false;

    LVBKIMAGE lv = { 0 };
    lv.ulFlags = LVBKIF_TYPE_WATERMARK;

    lv.hbm = bmp;
    lv.xOffsetPercent = 100;

    lv.yOffsetPercent = 100;
    ListView_SetBkImage( (HWND) this->GetHandle(), &lv);


    return true;
#else
    return false;
#endif //__WXMSW__

}

