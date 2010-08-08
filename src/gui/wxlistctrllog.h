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

#ifndef WXLISTCTRLLOG_H
#define WXLISTCTRLLOG_H

#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include <wx/log.h>
#include <wx/file.h>
#include <wx/filename.h>


class wxImageList;

class wxListCtrlLog : public virtual wxLog, public virtual wxListCtrl
{
public:
    wxListCtrlLog(wxWindow* parent, wxWindowID id, const wxPoint& pt,
                                         const wxSize& sz, long style);

    ~wxListCtrlLog();

    virtual void DoLog(wxLogLevel level, const wxChar *szString, time_t t);

    virtual void Flush();

    bool SetDebugFile( wxString PathAndFilename, int Flags = wxFile::write );

protected:
private:
    wxLog*          m_previoustarget;
    wxArrayString   m_messages;
    wxArrayInt      m_severity;
    wxArrayLong     m_times;
    bool            loadedIcons;
    bool            m_bErrors;

    wxImageList     m_imageList;
    wxFile          m_logfile;

};

#endif // WXLISTBOXLOG_H
