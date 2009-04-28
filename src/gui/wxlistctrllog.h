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
