
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <wizard.h>
#include "MyWizard.h"
#include <stdio.h>

#define NELEMS(a)  (sizeof(a) / sizeof(a[0]))
#define NAMELEN 80

static TCHAR g_szProjname[9];
static TCHAR g_szPROJNAME[9];
static TCHAR g_szDispname[NAMELEN+1];
static int cchProjname;


static BOOL WINAPI OnStep1(HWND, enum WizAction);
static BOOL WINAPI OnStep2(HWND, enum WizAction);
static void WINAPI OnFileLine(PTSTR, int);

int ReplaceText(PTSTR pszLine,PTSTR pszTextOld,PTSTR pszTextNew);
void Error(char* errmsg);
int MakeFiles(void);

struct fileset
{
	char *pszFile;
	WORD IDR_FILE;
	BOOL bText;
};
struct fileset filesets[]=
{
	"main.c",	IDR_MAIN_C,	TRUE,//TRUE: this will replace text
	"main.rc",	IDR_MAIN_RC,	TRUE,//TRUE: this will replace text
	"main.h",	IDR_MAIN_H,	FALSE,
	"main.ico",	IDR_MAIN_ICO,	FALSE,
	"",0,0
};

BOOL WINAPI WizMain(void)
{
	TCHAR szBuffer[1024];
    TCHAR szTail[16];

	if (!WizGetProjectName(szBuffer, NELEMS(szBuffer))){
		Error("GetProjectName");
		return FALSE;
	}

	cchProjname = lstrlen(lstrcpyn(g_szProjname, szBuffer, NELEMS(g_szProjname)));
	CharUpper(lstrcpy(g_szPROJNAME,g_szProjname));

    // Build a default display name.
    LoadString(WizGetInstanceHandle(), IDS_APPLICATION, szTail, NELEMS(szTail));
    lstrcpyn(g_szDispname, szBuffer, NELEMS(g_szDispname) - lstrlen(szTail));
    lstrcat(g_szDispname, szTail);


	if(!WizAddStep(MAKEINTRESOURCE(DLG_START), OnStep1)){
		Error("AddStep 1");
		return FALSE;
	}
	if(!WizAddStep(MAKEINTRESOURCE(DLG_FINISH), OnStep2)){
		Error("AddStep 2");
		return FALSE;
	}

	if (!WizShowSteps()){
		//Error("ShowSteps");
		return FALSE;
	}
	if (!WizSetProjectType(<WIZTYPE>)){
		Error("SetProjectType");
		return FALSE;
	}


	if(!MakeFiles())return FALSE;

	return TRUE;
}



static BOOL WINAPI OnStep1(HWND hwnd, enum WizAction action)
{
    switch (action)
    {
        case Action_SetActive:
            // Set the display name.
            SetDlgItemText(hwnd, DID_DISPNAME, g_szDispname);
            return TRUE;

        case Action_KillActive:
            // Remember the display name.
            GetDlgItemText(hwnd, DID_DISPNAME, g_szDispname, NELEMS(g_szDispname));
            return g_szDispname[0] != '\0';
    }

    return FALSE;
}

static BOOL WINAPI OnStep2(HWND hwnd, enum WizAction action)
{
	switch (action)
	{
	case Action_SetActive:
		{
			for(int i=0;filesets[i].pszFile[0]!=0;i++){
			ListBox_AddString(GetDlgItem(hwnd,DID_RESULTFILES),filesets[i].pszFile);
			}

     TCHAR szProjPath[260];
     GetCurrentDirectory(NELEMS(szProjPath), szProjPath);
     SetDlgItemText(hwnd, DID_PROJFOLDER, szProjPath);
   
			return TRUE;
		}

	case Action_UpdateUI:
		{

			return TRUE;
		}

	case Action_KillActive:
		{

			return TRUE;
		}

	}
	return FALSE;
}



static void WINAPI OnFileLine(PTSTR pszLine, int cchMaxLine /* normally 4096 chars */)
{

/*<NOTREPLACE>*/	if (ReplaceText(pszLine,"<NOTREPLACE>","<NOTREPLACE>")){
		return;
	}
/*<NOTREPLACE>*/	if (ReplaceText(pszLine,"<PROJNAME>",g_szPROJNAME)){
		return;
	}
/*<NOTREPLACE>*/	if (ReplaceText(pszLine,"<DISPNAME>",g_szDispname)){
		return;
	}

}
int ReplaceText(PTSTR pszLine,PTSTR pszTextOld,PTSTR pszTextNew){
	PTSTR psz;
	if((psz = _tcsstr(pszLine, TEXT(pszTextOld))) != NULL){
		int cchLine = lstrlen(pszLine);
		int cchTextOld = lstrlen(pszTextOld);
		int cchTextNew = lstrlen(pszTextNew);
		_tmemmove(psz + cchTextNew, psz + cchTextOld, pszLine + cchLine - (psz + cchTextOld) + 1);
		_tmemcpy(psz, pszTextNew, cchTextNew);
		return TRUE;
	}
	return FALSE;
}

void Error(char* errmsg){
	MessageBox(0,errmsg,"wizwiz error",   MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST );
}

int MakeFiles(void){
	for(int i=0;filesets[i].pszFile[0]!=0;i++){
		if(filesets[i].bText==TRUE){
			if(!WizWriteTextFileFromResourceA(filesets[i].pszFile, MAKEINTRESOURCE(filesets[i].IDR_FILE),OnFileLine)){
				MessageBox(0,filesets[i].pszFile,"WizWriteTextFileFromResourceA error ",   MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST );
				return FALSE;
			}
		}
		if(filesets[i].bText==FALSE){
			if (!WizWriteFileFromResourceA(filesets[i].pszFile, MAKEINTRESOURCE(filesets[i].IDR_FILE))){
				MessageBox(0,filesets[i].pszFile,"WizWriteFileFromResourceA error ",   MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST );
				return FALSE;
			}
		}
	}
	
	for(int i=0;filesets[i].pszFile[0]!=0;i++){
		char *pszC=filesets[i].pszFile;
		while(*pszC!='.'&&pszC!=0)pszC++;
		if(strcmp(pszC,".c")==0 ||
			strcmp(pszC,".C")==0	||
			strcmp(pszC,".rc")==0	||
			strcmp(pszC,".RC")==0){
				if(!WizAddProjectFile(filesets[i].pszFile)){
				MessageBox(0,filesets[i].pszFile,"WizAddProjectFile error ",   MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST );
				return FALSE;
			}
		}
	}

	return TRUE;
}
