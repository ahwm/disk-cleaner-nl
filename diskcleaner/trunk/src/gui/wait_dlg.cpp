#include "wait_dlg.h"
#include "wx/app.h"

wait_dlg::wait_dlg( wxWindow* parent )
:
wait_base_dlg( parent )
{

}

void wait_dlg::SetProgressRange( const int maximum )
{
    progress_bar->SetRange( maximum );
}

void wait_dlg::Increment()
{
    progress_bar->SetValue( progress_bar->GetValue() + 1 );
    wxTheApp->Yield();
}

