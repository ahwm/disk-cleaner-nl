/*
** Copyright (C) 2009 Robert Moerland
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation version 2.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include "gui/dcsettings.h"
#include <wx/confbase.h>

namespace diskcleaner
{
    bool dcsettings::Save()
    {
        return Save( wxConfigBase::Get( false ) );
    }

    bool dcsettings::Save( wxConfigBase* cf )
    {
        cf->Write( L"UI/DCFrameWidth", ui.dc_frame_size.width );
        cf->Write( L"UI/DCFrameHeight", ui.dc_frame_size.height );
        cf->Write( L"UI/DCFrameTopX", ui.dc_frame_size.topx );
        cf->Write( L"UI/DCFrameTopY", ui.dc_frame_size.topy );

        cf->Write( L"UI/ResultFrameWidth", ui.result_frame_size.width );
        cf->Write( L"UI/ResultFrameHeight", ui.result_frame_size.height );
        cf->Write( L"UI/ResultFrameTopX", ui.result_frame_size.topx );
        cf->Write( L"UI/ResultFrameTopY", ui.result_frame_size.topy );

        cf->Write( L"UI/ColShortDescWidth", ui.col_sdesc_width );
        cf->Write( L"UI/ColLongDescWidth", ui.col_ldesc_width );
        cf->Write( L"UI/ColItemWidth", ui.col_item_width );
        cf->Write( L"UI/ColSizeWidth", ui.col_size_width );
        cf->Write( L"UI/SortColumn", ui.sort_column );
        cf->Write( L"UI/SortOrder", ui.sortorder );


        cf->Write( L"Global/Delete Locked", global.delete_locked );
        cf->Write( L"Global/Hide Empty", global.hide_empty );
        cf->Write( L"Global/Show Description", global.show_description );

        cf->Write( L"System Temp/Delete ReadOnly", systemp.delete_ro );
        cf->Write( L"System Temp/Delete Empty Subfolders", systemp.delete_subfolders );
        cf->Write( L"System Temp/Minimum Age", static_cast<long int>( systemp.min_age ) );

        cf->Write( L"Temporary Internet Files/Delete Offline", tempinternetfiles.delete_offline );
        cf->Write( L"Internet Explorer Cookies/Use Cookie Filter", cookies.use_cookie_filter );
        cf->Write( L"Internet Explorer Cookies/Minimum Age", cookies.min_cookie_age );

        //TODO: return something meaningful
        return true;
    }

    bool dcsettings::Load()
    {
        return Load( wxConfigBase::Get( false ) );
    }

    bool dcsettings::Load( wxConfigBase* cf )
    {
        cf->Read( L"UI/DCFrameWidth", &ui.dc_frame_size.width, 0 );
        cf->Read( L"UI/DCFrameHeight", &ui.dc_frame_size.height, 0 );
        cf->Read( L"UI/DCFrameTopX", &ui.dc_frame_size.topx, 0 );
        cf->Read( L"UI/DCFrameTopY", &ui.dc_frame_size.topy, 0 );

        cf->Read( L"UI/ResultFrameWidth", &ui.result_frame_size.width, 0 );
        cf->Read( L"UI/ResultFrameHeight", &ui.result_frame_size.height, 0 );
        cf->Read( L"UI/ResultFrameTopX", &ui.result_frame_size.topx, 0 );
        cf->Read( L"UI/ResultFrameTopY", &ui.result_frame_size.topy, 0 );

        cf->Read( L"UI/ColShortDescWidth", &ui.col_sdesc_width, 0 );
        cf->Read( L"UI/ColLongDescWidth", &ui.col_ldesc_width, 0 );
        cf->Read( L"UI/ColItemWidth", &ui.col_item_width, 0 );
        cf->Read( L"UI/ColSizeWidth", &ui.col_size_width, 0 );
        cf->Read( L"UI/SortColumn", &ui.sort_column, 0 );
        cf->Read( L"UI/SortOrder", &ui.sortorder, 0 );

        cf->Read( L"Global/Delete Locked" , &global.delete_locked, false );
        cf->Read( L"Global/Hide Empty" , &global.hide_empty, true );
        cf->Read( L"Global/Show Description" , &global.show_description, true );

        cf->Read( L"System Temp/Delete ReadOnly", &systemp.delete_ro, true );
        cf->Read( L"System Temp/Delete Empty Subfolders", &systemp.delete_subfolders, true );
        cf->Read( L"System Temp/Minimum Age", (int*)( &systemp.min_age ), 10 );

        cf->Read( L"Temporary Internet Files/Delete Offline", &tempinternetfiles.delete_offline, true );
        cf->Read( L"Internet Explorer Cookies/Use Cookie Filter", &cookies.use_cookie_filter, false );
        cf->Read( L"Internet Explorer Cookies/Minimum Age", &cookies.min_cookie_age, 0 );


        //TODO: something meaningful here
        return true;
    }
}
