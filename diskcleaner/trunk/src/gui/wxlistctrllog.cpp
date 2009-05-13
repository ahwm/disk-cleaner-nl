/*
** Copyright (C) 2009 Robert Moerland
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the wxWindows Library Licence, Version 3.1
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** wxWindows Library Licence for more details.
**
*/

#include "wxlistctrllog.h"
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include <wx/artprov.h>
#include <wx/app.h>

#ifdef  __WXMSW__
// for OutputDebugString()
#include  "wx/msw/private.h"
#endif // Windows

// Shamelessly copied from wxLogGui :)
wxListCtrlLog::wxListCtrlLog(wxListCtrl* aListCtrl) : wxLog( ), m_bErrors( false )
{
    m_listctrl = aListCtrl;

    m_listctrl->ClearAll();

    m_listctrl->SetWindowStyle( wxSUNKEN_BORDER | wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL );

    m_listctrl->InsertColumn(0, _T("Message"));
    //m_listctrl->InsertColumn(1, _T("Time"));

    static const int ICON_SIZE = 16;
    pimageList = boost::shared_ptr<wxImageList>( new wxImageList(ICON_SIZE, ICON_SIZE) );

    // order should be the same as in the switch below!
    static const wxChar* icons[] =
    {
        wxART_ERROR,
        wxART_WARNING,
        wxART_INFORMATION
    };

    bool loadedIcons = true;

    for ( size_t icon = 0; icon < WXSIZEOF(icons); icon++ )
    {
        wxBitmap bmp = wxArtProvider::GetBitmap(icons[icon], wxART_MESSAGE_BOX,
                                                wxSize(ICON_SIZE, ICON_SIZE));

        // This may very well fail if there are insufficient colours available.
        // Degrade gracefully.
        if ( !bmp.Ok() )
        {

            loadedIcons = false;

            break;
        }

        pimageList->Add(bmp);
    }

    m_listctrl->SetImageList( pimageList.get() , wxIMAGE_LIST_SMALL );

    SetActiveTarget( this );


}

wxListCtrlLog::~wxListCtrlLog()
{
    //dtor
}

void wxListCtrlLog::DoLog(wxLogLevel level, const wxChar *szString, time_t t)
{
    switch ( level )
    {
    case wxLOG_Info:
        if ( GetVerbose() )
        case wxLOG_Message:
    {
        m_messages.Add(szString);
        m_severity.Add(wxLOG_Message);
        m_times.Add((long)t);
    }
    break;

    case wxLOG_Trace:
    case wxLOG_Debug:
#ifdef __WXDEBUG__
        {
            wxString str;
            TimeStamp(&str);
            str += szString;

#if defined(__WXMSW__) && !defined(__WXMICROWIN__)
            // don't prepend debug/trace here: it goes to the
            // debug window anyhow
            str += wxT("\r\n");
            OutputDebugString(str);
#else
            // send them to stderr
            wxFprintf(stderr, wxT("[%s] %s\n"),
                      level == wxLOG_Trace ? wxT("Trace")
                      : wxT("Debug"),
                      str.c_str());
            fflush(stderr);
#endif
        }
#endif // __WXDEBUG__

        break;

    case wxLOG_FatalError:
        // show this one immediately
        wxMessageBox(szString, _("Fatal error"), wxICON_HAND);
        wxExit();
        break;

    case wxLOG_Error:
        if ( !m_bErrors )
        {
#if !wxUSE_LOG_DIALOG
            // discard earlier informational messages if this is the 1st
            // error because they might not make sense any more and showing
            // them in a message box might be confusing
            m_messages.Empty();
            m_severity.Empty();
            m_times.Empty();
#endif // wxUSE_LOG_DIALOG
            m_bErrors = true;
        }
        // fall through

    case wxLOG_Warning:

        m_messages.Add(szString);
        m_severity.Add((int)level);
        m_times.Add((long)t);

        break;
    }

}

void wxListCtrlLog::Flush()
{
    wxString fmt = wxLog::GetTimestamp();
    if ( !fmt )
    {
        // default format
        fmt = _T("%c");
    }

    size_t count = m_messages.GetCount();
    size_t listctrl_count = m_listctrl->GetItemCount();

    for ( size_t n = 0 ; n < count; n++ )
    {
        int image = -1;

        if ( loadedIcons )
        {
            switch ( m_severity[n] )
            {
            case wxLOG_Error:
                image = 0;
                break;

            case wxLOG_Warning:
                image = 1;
                break;

            default:
                image = -1;
            }
        }
        else // failed to load images
        {
            image = -1;
        }

        m_listctrl->InsertItem( n + listctrl_count, m_messages[n], image );
        //m_listctrl->SetItem(n, 1, wxLog::TimeStamp(fmt, (time_t)m_times[n]));
    }

    m_messages.Clear();
    // let the columns size themselves
    m_listctrl->SetColumnWidth(0, wxLIST_AUTOSIZE);
}
