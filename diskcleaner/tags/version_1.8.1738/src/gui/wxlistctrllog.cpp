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
#include <wx/artprov.h>
#include <wx/app.h>

#ifdef  __WXMSW__
// for OutputDebugString()
#include  "wx/msw/private.h"
#endif // Windows


wxListCtrlLog::wxListCtrlLog(wxWindow* parent, wxWindowID id, const wxPoint& pt,
                                         const wxSize& sz, long style) : wxLog( ),
                                         wxListCtrl(parent, id, pt, sz, style), m_bErrors( false ),
                                         m_imageList( 16, 16, TRUE )
{
    this->ClearAll();
    this->InsertColumn(0, _T("Message"));
    //m_listctrl->InsertColumn(1, _T("Time"));

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
                                                wxSize( 16, 16 ));

        // This may very well fail if there are insufficient colours available.
        // Degrade gracefully.
        if ( !bmp.Ok() )
        {

            loadedIcons = false;

            break;
        }

        m_imageList.Add(bmp);
    }

    this->SetImageList( &m_imageList, wxIMAGE_LIST_SMALL );

    m_previoustarget = SetActiveTarget( this );


}

wxListCtrlLog::~wxListCtrlLog()
{
    SetActiveTarget( m_previoustarget );
    wxLogDebug( L"%hs: SetActive called", __FUNCTION__ );
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
    // Necessary according to docs
    wxLog::Flush();

    wxString fmt = wxLog::GetTimestamp();
    if ( !fmt )
    {
        // default format
        fmt = _T("%c");
    }

    size_t count = m_messages.GetCount();
    size_t listctrl_count = this->GetItemCount();

    for ( size_t n = 0 ; n < count; n++ )
    {
        int image = -1;

        // If we got the images, show them
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

        int index = this->InsertItem( n + listctrl_count, m_messages[n], image );
        if ( index != -1 ) //error during insertion
        {
            this->EnsureVisible( index );
        }
        else
        {
            wxLogDebug( L"Error during message insertion in ListCtrlLog widget" );
        }
    }

    m_messages.Clear();
    m_severity.Clear();
    m_times.Clear();

    // let the columns size themselves
    this->SetColumnWidth(0, wxLIST_AUTOSIZE);
}
