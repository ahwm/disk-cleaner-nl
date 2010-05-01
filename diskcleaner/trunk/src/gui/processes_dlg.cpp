#include "processes_dlg.h"
#include <wx/log.h>
#include <windows.h>
#include <tlhelp32.h>

// For use with check_open_processes
struct running_processes
{
    bool internet_explorer;
    bool firefox;
    bool google_chrome;
    bool opera;
};

processes_dlg::processes_dlg( wxWindow* parent, diskcleaner::global_& gbsettings )
        :
        processes_dlg_base( parent )
{

    if ( gbsettings.show_running_processes )
    {
        running_processes rp = { 0 };
        check_open_processes( rp );

        if ( rp.firefox || rp.google_chrome || rp.internet_explorer || rp.opera )
        {
            wxArrayString as;

            if ( rp.firefox )
            {
                as.Add( _( "Firefox" ) );
            }
            if ( rp.google_chrome )
            {
                as.Add( _( "Google Chrome" ) );
            }
            if ( rp.internet_explorer )
            {
                as.Add( _( "Internet Explorer" ) );
            }
            if ( rp.opera )
            {
                as.Add( _( "Firefox" ) );
            }
            process_list_lb->InsertItems( as, 0 ) ;
            ShowModal();

            if ( dont_show_cb->IsChecked() )
            {
                gbsettings.show_running_processes = false;
            }
        }
    }
}

void processes_dlg::ok_btn_click( wxCommandEvent& event )
{
    Close();
}

void processes_dlg::check_open_processes(running_processes& rp)
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
        wxLogError( L"Process32First" ); // show cause of failure
        CloseHandle( hProcessSnap );     // clean the snapshot object
        return ;
    }

    // Now walk the snapshot of processes

    do
    {
        wxLogDebug( L"Found process name:  %s", pe32.szExeFile );

        if ( !wcsicmp( L"firefox.exe", pe32.szExeFile ) )
        {
            rp.firefox = true;
        }
        if ( !wcsicmp( L"iexplore.exe", pe32.szExeFile ) )
        {
            rp.internet_explorer = true;
        }

    }
    while ( Process32Next( hProcessSnap, &pe32 ) );

    CloseHandle( hProcessSnap );
}
