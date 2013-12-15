/*
* This file is part of wxSmith plugin for Code::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 5796 $
* $Id: wxsflexgridsizer.cpp 5796 2009-09-16 12:09:22Z mortenmacfly $
* $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsflexgridsizer.cpp $
*/

#include "wxsflexgridsizer.h"

namespace
{
    wxArrayInt GetArray(const wxString& String,bool* Valid = 0)
    {
        wxStringTokenizer Tokens(String,_T(","));
        wxArrayInt Array;
        if ( Valid )
        {
            *Valid = true;
        }

        while ( Tokens.HasMoreTokens() )
        {
            long Value;
            wxString Token = Tokens.GetNextToken();
            Token.Trim(true);
            Token.Trim(false);
            if ( !Token.ToLong(&Value) && Valid )
            {
                *Valid = false;
            }
            Array.Add((int)Value);
        }

        return Array;
    }

    bool FixupList(wxString& List)
    {
        bool Ret;
        wxArrayInt Array = GetArray(List,&Ret);
        List.Clear();
        for ( size_t i=0; i<Array.Count(); i++ )
        {
            List.Append(wxString::Format(_T("%d"),Array[i]));
            if ( i < Array.Count() - 1 )
            {
                List.Append(_T(','));
            }
        }
        return Ret;
    }

    wxsRegisterItem<wxsFlexGridSizer> Reg(_T("FlexGridSizer"),wxsTSizer,_T("Layout"),80);
}

wxsFlexGridSizer::wxsFlexGridSizer(wxsItemResData* Data):
    wxsSizer(Data,&Reg.Info),
    Cols(3),
    Rows(0)
{
}

wxSizer* wxsFlexGridSizer::OnBuildSizerPreview(wxWindow* Parent)
{
	wxFlexGridSizer* Sizer = new wxFlexGridSizer(Rows,Cols,
        VGap.GetPixels(Parent),HGap.GetPixels(Parent));

    wxArrayInt Cols = GetArray(GrowableCols);
    for ( size_t i=0; i<Cols.Count(); i++ )
    {
    	Sizer->AddGrowableCol(Cols[i]);
    }

    wxArrayInt Rows = GetArray(GrowableRows);
    for ( size_t i=0; i<Rows.Count(); i++ )
    {
    	Sizer->AddGrowableRow(Rows[i]);
    }
    return Sizer;
}

void wxsFlexGridSizer::OnBuildSizerCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/sizer.h>"),GetInfo().ClassName,hfInPCH);
            Codef(_T("%C(%d, %d, %s, %s);\n"),Rows,Cols,
                 #if wxCHECK_VERSION(2, 9, 0)
                 VGap.GetPixelsCode(GetCoderContext()).wx_str(),
                 HGap.GetPixelsCode(GetCoderContext()).wx_str());
                 #else
                 VGap.GetPixelsCode(GetCoderContext()).c_str(),
                 HGap.GetPixelsCode(GetCoderContext()).c_str());
                 #endif

            wxArrayInt Cols = GetArray(GrowableCols);
            for ( size_t i=0; i<Cols.Count(); i++ )
            {
                Codef(_T("%AAddGrowableCol(%d);\n"),Cols[i]);
            }

            wxArrayInt Rows = GetArray(GrowableRows);
            for ( size_t i=0; i<Rows.Count(); i++ )
            {
                Codef(_T("%AAddGrowableRow(%d);\n"),Rows[i]);
            }

            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsFlexGridSizer::OnBuildSizerCreatingCode"),GetLanguage());
        }
    }
}

void wxsFlexGridSizer::OnEnumSizerProperties(long Flags)
{
    FixupList(GrowableCols);
    FixupList(GrowableRows);
    WXS_LONG(wxsFlexGridSizer,Cols,_("Cols"),_T("cols"),0);
    WXS_LONG(wxsFlexGridSizer,Rows,_("Rows"),_T("rows"),0);
    WXS_DIMENSION(wxsFlexGridSizer,VGap,_("V-Gap"),_("V-Gap in dialog units"),_T("vgap"),0,false);
    WXS_DIMENSION(wxsFlexGridSizer,HGap,_("H-Gap"),_("H,y-Gap in dialog units"),_T("hgap"),0,false);
    WXS_SHORT_STRING(wxsFlexGridSizer,GrowableCols,_("Growable cols"),_T("growablecols"),_T(""),false);
    WXS_SHORT_STRING(wxsFlexGridSizer,GrowableRows,_("Growable rows"),_T("growablerows"),_T(""),false);
    FixupList(GrowableCols);
    FixupList(GrowableRows);
}
