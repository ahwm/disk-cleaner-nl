#include "processes_dlg.h"
#include <windows.h>
#include <tlhelp32.h>
#include <wx/log.h>

namespace diskcleaner
{

processes_dlg::processes_dlg( wxWindow* parent )
    :
    processes_dlg_base( parent )
{
    // Create a list of running processes
    check_open_processes();
}

void processes_dlg::ok_btn_click( wxCommandEvent& event )
{
    Close();
}

void processes_dlg::add_process_to_check( const std::wstring& process, const std::wstring& pretty_name)
{
    processlist.insert( make_pair ( process, pretty_name ) );
}

// Create a list of running processes on the system
void processes_dlg::check_open_processes()
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if ( hProcessSnap == INVALID_HANDLE_VALUE )
    {
        wxLogError( L"CreateToolhelp32Snapshot (of processes)" );
        return ;
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof( PROCESSENTRY32 );

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if ( !Process32First( hProcessSnap, &pe32 ) )
    {
        wxLogError( L"%hs: Process32First failed.", __FUNCTION__ ); // show cause of failure
        CloseHandle( hProcessSnap );     // clean the snapshot object
        return ;
    }

    // Now walk the snapshot of processes
    do
    {
        CharLower( pe32.szExeFile );
        running_processes.insert( pe32.szExeFile );
    }
    while ( Process32Next( hProcessSnap, &pe32 ) );

    CloseHandle( hProcessSnap );
}

// Check for running processes and display them in a dialog box
//
// Return value is assignable to dcsettings::global.show_running_processes (true/false)
bool processes_dlg::do_process_check()
{
    wxArrayString as;

    std::map<std::wstring, std::wstring>::iterator it = processlist.begin();

    // Iterate over all processes the plug-ins have specified
    while ( it != processlist.end() )
    {
        // If the particular exe is found running, add the pretty name to the dialog box
        if( running_processes.count( it->first ) > 0 )
        {
            as.Add( processlist[it->first] );
        }

        ++it;
    }

    // Only show the dialog if any specified processes are running
    if ( as.GetCount() > 0 )
    {
        process_list_lb->InsertItems( as, 0 );
        ShowModal();

        // return a value that is assignable to settings.global.show_running_processes
        return !dont_show_cb->IsChecked();
    }

    // We only would show the message box if dcsettings::global.show_running_processes
    // is true. If we do not show a message box, this return value will make sure
    // that one can be shown the next time.
    return true;
}

}
