/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 4909 $
 * $Id: printing_types.cpp 4909 2008-02-27 13:15:26Z mortenmacfly $
 * $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/sdk/printing_types.cpp $
 */

#include "sdk_precomp.h"
#include "printing_types.h"

#ifndef CB_PRECOMP
    #include "manager.h"
    #include "configmanager.h"
#endif

// NOTE (Tiwag#1#): 061012 global wxPrinter, used in cbeditorprintout
//                  to get correct settings if changed in printer dialog
wxPrinter* g_printer = 0;

// TODO (Tiwag#1#): 061012 Page Setup not implemented
// wxPageSetupData* g_pageSetupData = 0;

void InitPrinting()
{
    if (!g_printer)
    {
        g_printer = new wxPrinter;
        int paperid = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/printerdialog/paperid"), wxPAPER_A4 );
        int paperorientation  = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/printerdialog/paperorientation"), wxPORTRAIT );
        wxPrintData* ppd = &(g_printer->GetPrintDialogData().GetPrintData());
        ppd->SetPaperId((wxPaperSize)paperid);
        ppd->SetOrientation(paperorientation);
    }

//    if (!g_pageSetupData)
//        g_pageSetupData = new wxPageSetupDialogData;
}

void DeInitPrinting()
{
    delete g_printer;
    g_printer = 0;
//    delete g_pageSetupData;
//    g_pageSetupData = 0;
}
