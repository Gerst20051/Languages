/***************************************************************************** 
 *
 * File    : FindUsage.c
 *
 * Purpose : Add-In which allow to start, from the contextual menu of the source editor,
 *           a find in files of the word which is under the cursor or of the selected text.
 *
 * Versions history :
 * v1 - 28/11/06 - initial version
 * v2 - 29/11/06 - add the separators TAB and comma
 * v3 - 22/01/07 - take into account the French title of the "Find in files" popup
 * 
 *****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <addin.h>
#include <stdlib.h>
#include <stdio.h>

#include "FindUsage.h"

static HANDLE g_hmod = NULL;		//this module handle
static HWND g_hwndMain = NULL;	//the PellesC main window handle

//list of the separators of the C language
#define SEP_NUM 26
static char Separator[SEP_NUM] = {' ', ';', ',', '&', '.', '"', '\'', 
											 '{', '}','(' ,')' ,'[' ,']', 
											 '-', '+', '/', '*', '=', 
											 '>', '<', '|', '\\', '!', 10, 13, 9};

//identifier of the command to find in files
#define ID_FINDINFILES 1

/****************************************************************************
 Function:     DebugLog
 Purpose:      Log a text formated like sprintf function
               The text can be retrieved by the freeware DebugView of Sysinternals
 Input:        The text format, the datas to format in the text
 ****************************************************************************/
void DebugLog(const char* TextFormat, ...)
{
	char Text[200];
	va_list argList;

	va_start(argList, TextFormat);
	vsnprintf(Text, 200, TextFormat, argList);
	va_end(argList);

	OutputDebugString(Text);
}

/****************************************************************************
 *                                                                          
 * Function: DllMain                                                        
 *                                                                          
 * Purpose : DLL entry and exit procedure.                                  
 *                                                                          
 ****************************************************************************/
BOOL WINAPI DllMain(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
			// Save module handle, in case we need it.
			g_hmod = hDLL;
			return TRUE;

		case DLL_PROCESS_DETACH:
			return TRUE;

		default:
			return TRUE;
	}
}

/****************************************************************************
 *                                                                          
 * Function: GetSourceText                                                      
 *                                                                          
 * Purpose : Get the text of the active source document
 *                                                                          
 ****************************************************************************/
char *GetSourceText(void)
{
	// Get window handle of the active MDI document
	HWND hwndDoc = AddIn_GetActiveDocument(g_hwndMain);
	if (hwndDoc == NULL)
		return NULL;

	// Get type of the active document, it must be a source code document, else we exit
	if (AddIn_GetDocumentType(hwndDoc) != AID_SOURCE)
		return NULL;
	// Get the text of the active document
	int len = AddIn_GetSourceTextLength(hwndDoc);
	// if there is no text, exit
	if (len <= 0)
		return NULL;

	// Allocate a buffer to receive the document text
	char *pText;
	pText = malloc(len + 1);
	// If the malloc has failed, exit
	if (pText == NULL)
		return NULL;
	// Write the end of string character
	pText[len] = '\0';

	// Get the source text
	if (AddIn_GetSourceText(hwndDoc, pText, len) <= 0)
	{	// The get has failed, exit
		free(pText);
		return NULL;
	}

	return pText;
}

/****************************************************************************
 *                                                                          
 * Function: IsSeparator                                                      
 *                                                                          
 * Purpose : Return TRUE if the given character is a C separator
 *                                                                          
 ****************************************************************************/
BOOL IsSeparator(char C)
{
	for (int i = 0; i < SEP_NUM; i++)
		if (C == Separator[i])
			return TRUE;

	return FALSE;
}

/****************************************************************************
 *                                                                          
 * Function: AddInMain                                                      
 *                                                                          
 * Purpose : Add-in entry and exit procedure.                               
 *                                                                          
 ****************************************************************************/
ADDINAPI BOOL WINAPI AddInMain(HWND hwnd, ADDIN_EVENT eEvent)
{
	 // This is the only required export function in an Add-In.

	switch (eEvent)
	{
		// Application events.

		case AIE_APP_CREATE:
		{
			ADDIN_ADD_COMMAND AddCmd;

			// Remember main window handle, in case we need it.
			g_hwndMain = hwnd;

			// Add "Find word usage" command to IDE source menu
			AddCmd.cbSize = sizeof(AddCmd);
			AddCmd.pszText = "Find word usage";
			AddCmd.hIcon = LoadImage(g_hmod, MAKEINTRESOURCE(IDICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR | LR_SHARED);
			AddCmd.id = ID_FINDINFILES;
			AddCmd.idMenu = AIM_MENU_SOURCE;
			AddIn_AddCommand(hwnd, &AddCmd);

			AddIn_WriteOutput(hwnd, "Addin to find word usage is loaded.");

			return TRUE;
		}

		case AIE_APP_DESTROY:
			// Forget main window handle.
			g_hwndMain = NULL;

			// Remove the "Find word usage" from the IDE
			AddIn_RemoveCommand(hwnd, ID_FINDINFILES);

			return TRUE;

		default:
			return TRUE;
	}
}

/****************************************************************************
 *                                                                          
 * Function: FillInThreadFunc                                                      
 *                                                                          
 * Purpose : Function of the thread which fill the "Find what" field of the dialog box "Find in files"
 *                                                                          
 ****************************************************************************/
DWORD WINAPI FillInThreadFunc(void *WordToFind)
{
	char Txt[100];
	HWND hTop; //handle of the "Find in files" dialog box
	
	//wait during the 2 seconds maximum the arrival of the "Find in files" dialog in foreground
	DWORD T0 = GetTickCount(); //memorize the starting time
	int n = 0;
	do
	{
		//read the caption of the foreground window
		hTop = GetForegroundWindow();
		GetWindowText(hTop, Txt, sizeof(Txt));

		if (strcmp(Txt, "Find in files") == 0 || strcmp(Txt, "Chercher dans des fichiers") == 0)
		{	//the expected window is arrived
			//get the handle of the first control of the dialog box
			HWND h = GetWindow(hTop, GW_CHILD|GW_HWNDFIRST);
			while (h != NULL)
			{
				if (n == 1) //combo box containing the string to search
				{
					//add to the combo the searched string (received in the command line)
					SendMessage(h, CB_INSERTSTRING, 0, (LPARAM)WordToFind);
					SendMessage(h, CB_SETCURSEL, 0, 0);
				}
				if (n == 9) //button "Find"
				{	//simulate a click on the button "Find"
					PostMessage(h, BM_CLICK, 0, 0);
					//exit the application
					return 1;
				}

				h = GetWindow(h, GW_HWNDNEXT); //retrieve the handle of the next control
				n++; //increment the control number
			}
		}
		Sleep(10); //wait a little before to look again for the expected dialog box
	} 
	while (GetTickCount() - T0 < 2000); //loop while the time-out is not reached

	/*
	Order of the controls of the "Find in files" dialog box :
	[0] 16256808 : Find &what:
	[1] 8327112 : 
	[2] 11734944 : In &types:
	[3] 7147286 : 
	[4] 1511260 : In f&older:
	[5] 2494180 : 
	[6] 14683910 : ...
	[7] 2559794 : &Match case
	[8] 15863700 : Look in &subfolders
	[9] 8916796 : &Find
	[10] 7868254 : Cancel
	*/

	return(0);
}

/****************************************************************************
 *                                                                          
 * Function: AddInCommandEx                                                 
 *                                                                          
 * Purpose : Add-in command handler.                                        
 *                                                                          
 ****************************************************************************/
ADDINAPI void WINAPI AddInCommandEx(int idCmd, LPCVOID pcvData)
{
	switch (idCmd)
	{
		case ID_FINDINFILES:
		{	// A word has to be searched in filed

			// Get the handle of the active document
			HWND hwndDoc;
			hwndDoc = AddIn_GetActiveDocument(g_hwndMain);

			if (AddIn_GetDocumentType(hwndDoc) == AID_SOURCE)
			{	// the active document contain source code

				ADDIN_RANGE mRange;
				if (! AddIn_GetSourceSel(hwndDoc, &mRange))
					return;

				char *src = NULL;	// The source buffer
				src = GetSourceText();	// Get all file text
				if (src == NULL)
					return;				// nothing to do

				if (mRange.iEndPos > mRange.iStartPos)
				{	//a texte is selected : we will search it as it is
					mRange.iEndPos--;
				}
				else
				{	//no text is selected : search for the boundaries of the text under the cursor
					if (IsSeparator(src[mRange.iStartPos]) && IsSeparator(src[mRange.iEndPos]))
					{
						free(src);
						return;
					}

					while (!IsSeparator(src[mRange.iStartPos - 1]) && mRange.iStartPos > 0)
	 					mRange.iStartPos--;
					while (!IsSeparator(src[mRange.iEndPos + 1]) && mRange.iEndPos < strlen(src))
						mRange.iEndPos++;
				}
				
				if (mRange.iEndPos - mRange.iStartPos + 1 > 50) //the text to search is too long
				{
					free(src);
					MessageBox(g_hwndMain, "Please select less than 50 characters", "Find Word Usage Addin", MB_OK | MB_ICONINFORMATION);
					return;
				}

				char T[50];
				memset(T, 0, 50);
				strncpy(T, &src[mRange.iStartPos], mRange.iEndPos - mRange.iStartPos + 1);
				free(src);

				//start a thread to fill the "Find in files" pop-up
				DWORD id;
				HANDLE hThread = CreateThread(NULL, 0, FillInThreadFunc, (void*)T, 0, &id);
				//open the "Find in files" pop, the thread will detect it and will fill it
				if (hThread != NULL)
				{
					AddIn_SendIDECommand(g_hwndMain, AIC_FILE_FILEFIND);
					//we have exit from the dialog box, close the thread handle
					CloseHandle(hThread);
				}
			}
		}
		break;
	}
}
