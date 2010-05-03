#ifndef __processes_dlg__
#define __processes_dlg__

#include "dc_gui.h"
#include <map>
#include <set>

namespace diskcleaner
{


class processes_dlg : public processes_dlg_base
{
    std::set< std::wstring > running_processes;
    std::map< std::wstring, std::wstring > processlist;

protected:
    // Handlers for processes_dlg_base events.
    void ok_btn_click( wxCommandEvent& event );

    void check_open_processes();
public:
    processes_dlg( wxWindow* parent );

    void add_process_to_check( const std::wstring& process, const std::wstring& pretty_name );
    bool do_process_check();
};

}
#endif // __processes_dlg__
