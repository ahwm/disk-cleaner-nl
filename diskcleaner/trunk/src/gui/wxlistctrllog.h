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

#ifndef WXLISTBOXLOG_H
#define WXLISTBOXLOG_H

#include <wx/log.h>
#include <boost/shared_ptr.hpp>


class wxListCtrl;
class wxImageList;

class wxListCtrlLog : public wxLog
{
public:
    wxListCtrlLog(wxListCtrl* ListCtrl);
    ~wxListCtrlLog();

    virtual void DoLog(wxLogLevel level, const wxChar *szString, time_t t);

    void ShowLog();

protected:
private:
    wxArrayString   m_messages;
    wxArrayInt      m_severity;
    wxArrayLong     m_times;
    bool loadedIcons;
    bool m_bErrors;

    boost::shared_ptr<wxImageList> pimageList;
    wxListCtrl* m_listctrl;
};

#endif // WXLISTBOXLOG_H
