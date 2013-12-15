/*******************************************************************************

 File    : FuncList.c

 Purpose : Add-In which show in a tab at the bottom of the PellesC IDE the list
           of the function of the source file in use
           Note : several functions for functions find comes from the Addin "faddin"

*******************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <addin.h>
#include <stdio.h>
#include <stdlib.h>
#include <commctrl.h>

#include "FuncList.h"

// Macros to get and set the old window procedure for a subclassed window.
#define SETOLDWNDPROC(hwnd, pfn)  SetProp((hwnd), MAKEINTRESOURCE(g_atUUID), (HANDLE)(pfn))
#define GETOLDWNDPROC(hwnd)  ((WNDPROC)GetProp((hwnd), MAKEINTRESOURCE(g_atUUID)))
#define UNSETOLDWNDPROC(hwnd)  RemoveProp((hwnd), MAKEINTRESOURCE(g_atUUID))

#define isalnumEx(c) (isalnum((c)) || ((c)) == '_')

static ATOM g_atUUID = 0;

#define POS_LINE   1
#define POS_CHAR   0
#define POS_SCROLL 2
#define SEARCH_FWD 4

// Locals.
static HANDLE g_hmod = NULL;
static HWND g_hwndMain = NULL;
//static HWND g_hwndDlg = NULL;
static HWND g_hwndFuncListPage = NULL;
static LRESULT CALLBACK FuncListDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK MySubclassProc(HWND, UINT, WPARAM, LPARAM);

/* variáveis */
static char *Texto;
static char *buffer;
static int TextLen = 0;
static int iCounter = 0;
static char prevchar = ' ';
static char curchar = ' ';
static int lines;
static int n;
static int done = 0;

#define BUFSIZE 512

#define NAMELEN 512

char FunctionToSelect[NAMELEN];

static char *src = NULL;	// The source buffer

typedef struct {
	char sFunc[NAMELEN];
	char sShortFunc[NAMELEN];
	int iStart;
	int iEnd;
} FUNC;
FUNC func;

int curLine;

// Prototypes
int FindFunc(char *szText, void (*fnptr) (const int, const char *), int (*fnEnd) (const int));


/*******************************************************************************
                                                                          
 Function: ReadChar                                                        
                                                                          
 Purpose : ?? (function from the AddIn faddin)
                                                                          
*******************************************************************************/
static int ReadChar(void)
{
	if (iCounter < TextLen)
	{
		prevchar = curchar;
		// curchar = c;
		curchar = Texto[iCounter];
		iCounter++;
		if (curchar == 13)
		{
			curchar = prevchar;
			ReadChar();
		}
		return 1;
	}
	else
	{
		return 0;
	}
}
/*******************************************************************************
                                                                          
 Function: adicione                                                        
                                                                          
 Purpose : ?? (function from the AddIn faddin)
                                                                          
*******************************************************************************/
static void adicione(char *buf, char c, int bufsize)
{
	int tam = strlen(buf);
	if (c == ' ' && tam == 0)
		return;
	if (tam + 1 < bufsize)
	{
		buf[tam] = c;
		buf[tam + 1] = '\0';
	}
}
/*******************************************************************************
                                                                          
 Function: processa_barra                                                        
                                                                          
 Purpose : ?? (function from the AddIn faddin)
                                                                          
*******************************************************************************/
static void processa_barra(void)
{
	if (prevchar == '/')
	{	// dentro de comentário //
		while (ReadChar())
		{
			if (curchar == 10)
			{
				lines++;

				if (prevchar != '\\')
				{
					break;
				}
			}
		}
	}
}
/*******************************************************************************
                                                                          
 Function: processa_asterisco                                                        
                                                                          
 Purpose : ?? (function from the AddIn faddin)
                                                                          
*******************************************************************************/
static void processa_asterisco(void)
{
	if (prevchar == '/')
	{	// dentro de comentário multiplas linhas
		while (ReadChar())
		{
			if ((curchar == '/') && (prevchar == '*'))
			{
				curchar = 0;	// para evitar esta situaçao: /**/
				break;	// sai do comentario de multiplas linhas
			}
			if (curchar == 10)
				lines++;
		}
	}
	else
	{
		adicione(buffer, curchar, BUFSIZE);
	}
}
/*******************************************************************************
                                                                          
 Function: processa_aspas                                                        
                                                                          
 Purpose : ?? (function from the AddIn faddin)
                                                                          
*******************************************************************************/
static void processa_aspas(void)
{	// dentro de string
	int inBarra = 0;
	while (ReadChar())
	{
		if (curchar == 10)
			lines++;

		if (curchar == '\\')
		{
			inBarra++;
			if (inBarra == 2)
				inBarra = 0;
		}
		else
		{
			if (inBarra == 1)
				inBarra = 0;
		}

		if ((curchar == '\"') && (inBarra == 1))
		{
			inBarra--;
		}
		else if ((curchar == '\"') && (inBarra == 0))
		{
			break;
		}
	}
}
/*******************************************************************************
                                                                          
 Function: processa_aspas_simples                                                        
                                                                          
 Purpose : ?? (function from the AddIn faddin)
                                                                          
*******************************************************************************/
static void processa_aspas_simples(void)
{	// dentro de caracter
	int inBarra = 0;
	while (ReadChar())
	{
		if (curchar == 10)
			lines++;
		if ((curchar == '\\') && (prevchar == '\\'))
			inBarra = !inBarra;
		if (curchar == '\'')
		{
			if (inBarra)
				break;
			if (prevchar != '\\')
				break;
		}
	}
}
/*******************************************************************************
                                                                          
 Function: GetSourceText                                                        
                                                                          
 Purpose : Get the text of the current source window (function from the AddIn faddin)
           The caller must free() the memory.
 Output  : ret NULL on error
                                                                          
*******************************************************************************/
char *GetSourceText(void)
{
	/* Get window handle of the active MDI document */
	HWND hwndDoc = AddIn_GetActiveDocument(g_hwndMain);
	if (hwndDoc == NULL)
		return "" /*NULL*/;

	if (AddIn_GetDocumentType(hwndDoc) != AID_SOURCE)
		return "" /*NULL*/;

	int len = AddIn_GetSourceTextLength(hwndDoc);
			
	if (len <= 0)
		return "" /*NULL*/;

	char *pText;
	pText = malloc(len + 1);
	
	if (pText == NULL)
		return "" /*NULL*/;

	pText[len] = '\0';
	
	if (AddIn_GetSourceText(hwndDoc, pText, len) <= 0)
	{
		free(pText);
		return "" /*NULL*/;
	}

	return pText;
}
/*******************************************************************************
                                                                          
 Function: OnFindFuncStart                                                        
                                                                          
 Purpose : Gets the function name if cursor is inside the code range

 Input   : line = Function start line
           str  = Function Name
 Output  : None
 Errors  : None
                                                                          
*******************************************************************************/
void OnFindFuncStart(int line, const char *str)
{
	if (line <= curLine)
	{
		// str may be what we want
		func.iStart = line;
		strcpy(func.sFunc, str);
	}
	else
	{
		// it doesn't
		func.iStart = 0;
		ZeroMemory(func.sFunc, sizeof(func.sFunc));
	}
}
/*******************************************************************************
                                                                          
 Function: OnFindFuncEnd                                                        
                                                                          
 Purpose : Completes the function search. If current line is outside the 
           function end forget the previous function.
 
 Input   : line = Function end line
 Output  : None
 Errors  : None
                                                                          
*******************************************************************************/
int OnFindFuncEnd(const int line)
{
	if (line <= curLine)
	{
		// forget the old function name....
		ZeroMemory(func.sFunc, sizeof(func.sFunc));

		// continue
		return 1;
	}
	else
	{
		// we have found it, or end of file
		func.iEnd = line;
		// stop the parser
		return 0;
	}
}
/*******************************************************************************
                                                                          
 Function: FindFunc                                                        
                                                                          
 Purpose : Scans text for 'C' functions

 Input   : fnptr = function pointer, the function is called when a 'C' 
           function start is found.
           fnEnd = as the former, called when end of function is found.
 Output  : Always 0.
 Errors  : None.
                                                                          
*******************************************************************************/
int FindFunc(char *szText, void (*fnptr) (const int, const char *), int (*fnEnd) (const int))
{
	int inFunc = 0;
	int chaves = 0;
	prevchar = curchar = ' ';
	iCounter = TextLen = lines = 0;

	buffer = (char *)calloc(BUFSIZE, sizeof(char));
	Texto = szText;
	TextLen = strlen(Texto);
	n = 0;
	// return 0;

	while (ReadChar())
	{
		// printf("%c", curchar);

		switch (curchar)
		{
			case '/':	// barra /
				processa_barra();
				break;

			case '*':	// asterisco *
				processa_asterisco();
				break;

			case '\"':	// aspas duplas"
				processa_aspas();
				break;

			case '\'':	// aspas simples '
				processa_aspas_simples();
				break;

			case '#':	// #
				done = 0;
				while (ReadChar())
				{
					if (curchar == '/')
						processa_barra();
					else if (curchar == '*')
						processa_asterisco();
					else if (curchar == 10)
					{
						lines++;
						if (prevchar != '\\')
						{
							break;
						}
					}

					if (done)
					{
						break;
					}
				}
				break;

			case '{':	// abre chave {
				chaves++;
				if (inFunc == 0)
				{
					if (strchr(buffer, 40) && strchr(buffer, 41))
					{
						fnptr(lines, buffer);
						inFunc = 1;
					}
					else
					{
						memset(buffer, 0, BUFSIZE);
						n = 0;
					}
				}
				break;

			case '}':	// fecha chave }
				chaves--;
				if ((chaves == 0) && (inFunc == 1))
				{
					inFunc = 0;

					if (fnEnd != NULL)
					{
						if ((*fnEnd) (lines + 1) == 0)
							return 0;
					}
				}
				break;

			case 10:
				lines++;
				break;

			case ';':	// ;
				memset(buffer, 0, BUFSIZE);
				n = 0;
				break;

			default:
				if (inFunc == 0)
				{
					if (curchar == '\t')
						curchar = ' ';

					adicione(buffer, curchar, BUFSIZE);
				}
				break;
		}
	}

	free(buffer);
	return 0;
}
/*******************************************************************************
                                                                          
 Function: GetPureFunctionName                                                        
                                                                          
 Purpose : Extract function name from the full function prototype.

 Input   : None
 Output  : None.
           The simple function name is inserted in the structure.
 Errors  : None.
                                                                          
*******************************************************************************/
char *GetPureFunctionName(void)
{
	char *pos;	// search for "("
	int index;
	int end;

	pos = strchr(func.sFunc, '(');

	if (pos != NULL)
	{
		pos--;	// Points to char before '('
		while (pos > func.sFunc && isspace(*pos))	// Skip whitespaces
			pos--;

		index = end = pos - func.sFunc;	// Points to char before '('

		while (index > 0 && isalnumEx(func.sFunc[index]))
			index--;

		memcpy(func.sShortFunc, func.sFunc + index + 1, end - index);
		func.sShortFunc[end - index] = '\0';
	}
	else
	{
		strcpy(func.sShortFunc, func.sFunc);
	}

	return func.sShortFunc;
}
/*******************************************************************************
                                                                          
 Function: GetCurLine                                                        
                                                                          
 Purpose : current line of current source window

 Input   : 
 Output  :  ret -1 on error
 Errors  : 
                                                                          
*******************************************************************************/
int GetCurLine(void)
{
	/* Get window handle of the active MDI document */
	HWND hwndDoc = AddIn_GetActiveDocument(g_hwndMain);
	if (hwndDoc == NULL)
		return -1;

	//Is it a source doc?
	if (AddIn_GetDocumentType(hwndDoc) != AID_SOURCE)
		return -1;

	ADDIN_RANGE mRange;

	//Get actual caret position
	if (!AddIn_GetSourceSel(hwndDoc, &mRange))
		return -1;

	int iLine;
	//Get the line where the cursor is
	iLine = AddIn_SourceLineFromChar(hwndDoc, mRange.iStartPos);

	return iLine + 1;
}
/*******************************************************************************
                                                                          
 Function: SetCursorPosition                                                        
                                                                          
 Purpose : This function set the caret position in the currently active 
           document to a specified position.
           The input could be specified in line number or character 
           position format depending on switches passed in Uflags. It can 
           also scroll active area to made the selected region visible.

 Input   : pos    = is the required position in line number or character   
                    position format.
           Uflags = qualify position format and scroll selection.
 Output  : Boolean value, TRUE if OK, else FALSE.
 Errors  : No errors handling.
                                                                          
*******************************************************************************/
BOOL SetCursorPosition(int pos, UINT Uflags)
{
	ADDIN_RANGE mRange;
	HWND hwndCur;

	hwndCur = AddIn_GetActiveDocument(g_hwndMain);

	if (Uflags & POS_LINE)	// Get char position
		pos = AddIn_SourceLineIndex(hwndCur, pos);

	mRange.iStartPos = mRange.iEndPos = pos;

	AddIn_SetSourceSel(hwndCur, &mRange);

	if (Uflags & POS_SCROLL)
		AddIn_ScrollSourceCaret(hwndCur);

	return TRUE;
}
/*******************************************************************************
                                                                          
 Function: OnFindFunc                                                        
                                                                          
 Purpose : Add a function fount by FindFunc to the functions list of the tab

 Input   : line = Function start line
           str  = Function Name
 Output  : None
 Errors  : None
                                                                          
*******************************************************************************/
void OnFindFunc(const int line, const char *str)
{
	func.iStart = line;
	strcpy(func.sFunc, str);
	GetPureFunctionName();

	SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS, LB_ADDSTRING, 0, (LPARAM)func.sShortFunc);
	SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS_SORTED, LB_ADDSTRING, 0, (LPARAM)func.sShortFunc);
}
/*******************************************************************************
                                                                          
 Function: OnFindFuncSelect                                                        
                                                                          
 Purpose : Select the station fount by FindFunc if it is the one searched

 Input   : line = Function start line
           str  = Function Name
 Output  : None
 Errors  : None
                                                                          
*******************************************************************************/
void OnFindFuncSelect(const int line, const char *str)
{
	func.iStart = line;
	strcpy(func.sFunc, str);
	GetPureFunctionName();

	if (strcmp(func.sShortFunc, FunctionToSelect) == 0)
	{
		ADDIN_RANGE mRange;

		HWND hwndDoc = AddIn_GetActiveDocument(g_hwndMain);
		mRange.iStartPos = AddIn_SourceLineIndex(hwndDoc, line - 1);
		mRange.iEndPos = AddIn_SourceLineIndex(hwndDoc, line);
		AddIn_SetSourceSel(hwndDoc, &mRange);
		AddIn_ScrollSourceCaret(hwndDoc);
		AddIn_SetSourceSel(hwndDoc, &mRange);
	}
}
/*******************************************************************************

 Function: SetFocusOnCurrFunc

 Purpose : Set the focus in the functions list in the tab on the current function in usage

 Input   : None
 Output  : None
 Errors  : None

*******************************************************************************/
void SetFocusOnCurrFunc(void)
{
	if (GetCurLine() != curLine)
	{
		curLine = GetCurLine();
		if (curLine == -1)
			return;

		src = GetSourceText();	// Get all file text
		func.sFunc[0] = '\0';
		FindFunc(src, OnFindFuncStart, OnFindFuncEnd);
		GetPureFunctionName();

		if (func.sFunc[0] == '\0')
		{
			SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS, LB_SETCURSEL, -1, 0);
			SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS_SORTED, LB_SETCURSEL, -1, 0);
		}
		else
		{
			SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS, LB_SETCURSEL, -1, 0);
			int NbItems = SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS, LB_GETCOUNT, 0, 0);
			int i = 0;
			do
			{
				SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS, LB_GETTEXT, i, (LPARAM)FunctionToSelect);
				if (strcmp(FunctionToSelect, func.sShortFunc) == 0)
				{
					SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS, LB_SETCURSEL, i, 0);
					break;
				}
				i++;
			}
			while (i < NbItems);

			SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS_SORTED, LB_SETCURSEL, -1, 0);
			NbItems = SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS_SORTED, LB_GETCOUNT, 0, 0);
			i = 0;
			do
			{
				SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS_SORTED, LB_GETTEXT, i, (LPARAM)FunctionToSelect);
				if (strcmp(FunctionToSelect, func.sShortFunc) == 0)
				{
					SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS_SORTED, LB_SETCURSEL, i, 0);
					break;
				}
				i++;
			}
			while (i < NbItems);

		}				
		free(src);		
	}
}
/*******************************************************************************

 Function: ShowFunctionsList

 Purpose : Show in the functions list tab the list of all the functions of the current source file

 Input   : None
 Output  : None
 Errors  : None

*******************************************************************************/
void ShowFunctionsList(void)
{
	SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS, LB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(g_hwndFuncListPage, LST_FUNCTIONS_SORTED, LB_RESETCONTENT, 0, 0);

	HWND hwndDoc = AddIn_GetActiveDocument(g_hwndMain);
	if (hwndDoc != NULL)
	{
		if (AddIn_GetDocumentType(hwndDoc) == AID_SOURCE)
		{
			src = GetSourceText();	// Get all file text
			func.sFunc[0] = '\0';
			FindFunc(src, OnFindFunc, NULL);
			free(src);
		}
	}
	curLine = 0;
	SetFocusOnCurrFunc();
}
/*******************************************************************************

 Function: SelectFunction

 Purpose : Select in the source editor the function which has the given name

 Input   : None
 Output  : None
 Errors  : None

*******************************************************************************/
void SelectFunction(char *FuncName)
{
	HWND hwndDoc = AddIn_GetActiveDocument(g_hwndMain);
	if (hwndDoc != NULL)
	{
		if (AddIn_GetDocumentType(hwndDoc) == AID_SOURCE)
		{
			int len = AddIn_GetSourceTextLength(hwndDoc);
			if (len > 0)
			{
				char *pText;
				pText = malloc(len + 1);
				if (pText != NULL)
				{
					pText[len] = '\0';

					if (AddIn_GetSourceText(hwndDoc, pText, len) > 0)
					{
						FindFunc(pText, OnFindFuncSelect, NULL);
					}
					free(pText);
				}
			}
		}
	}
}
/*******************************************************************************

 Function: DllMain

 Purpose : DLL entry and exit procedure

*******************************************************************************/
BOOL WINAPI DllMain(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
			g_hmod = hDLL;
			// Build a unique value - to avoid clashing with other add-ins
			g_atUUID = GlobalAddAtom("{CBE8FF4C-DDA7-4044-916F-8214C9F24E46}");
			return TRUE;

		case DLL_PROCESS_DETACH:
			GlobalDeleteAtom(g_atUUID);
			return TRUE;

		default:
			return TRUE;
	}
}
/*******************************************************************************

 Function: AddInMain

 Purpose : Add-in entry and exit procedure

*******************************************************************************/
ADDINAPI BOOL WINAPI AddInMain(HWND hwnd, ADDIN_EVENT eEvent)
{
	switch (eEvent)
	{
		case AIE_APP_CREATE:
		{
			// Save handle of main IDE window
			g_hwndMain = hwnd;

			//create the window for functions list display
			g_hwndFuncListPage = CreateDialog(g_hmod, MAKEINTRESOURCE(DLG_FUNCLIST), hwnd, (DLGPROC)FuncListDlgProc);
			//Create New Tab
			AddIn_AddTabPage(hwnd, "Functions", g_hwndFuncListPage);

			AddIn_WriteOutput(hwnd, "Addin for functions list display in a Tab is loaded");

			return TRUE;
		}

		case AIE_APP_DESTROY:
			AddIn_RemoveTabPage(hwnd, g_hwndFuncListPage);
			DestroyWindow(g_hwndFuncListPage);
			return TRUE;

		case AIE_DOC_CREATE:
			//
			// Subclass all source document windows. The function MySubclassProc
			// will be called instead of the original source window procedure.
			// Handle any special cases in MySubclassProc, and pass on all messages.
			//
			SETOLDWNDPROC(hwnd, SubclassWindow(hwnd, MySubclassProc));
			ShowFunctionsList();
			return TRUE;

		case AIE_DOC_DESTROY:
			// It's important that we clean up after ourself.
			if (GETOLDWNDPROC(hwnd) != 0)
			{
				SubclassWindow(hwnd, GETOLDWNDPROC(hwnd));
				UNSETOLDWNDPROC(hwnd);
			}
			return TRUE;

		case AIE_DOC_SAVE:
		{
			//on source document save, update the functions list.
			HWND hwndDoc = AddIn_GetActiveDocument(g_hwndMain);
			if (AddIn_GetDocumentType(hwndDoc) == AID_SOURCE)
				ShowFunctionsList();
			return TRUE;
		}

		default:
			return TRUE;
	}
}
/*******************************************************************************

 Function: FuncListDlgProc

 Purpose : Window procedure of the functions list tab

*******************************************************************************/
static LRESULT CALLBACK FuncListDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
			g_hwndFuncListPage = hwndDlg;
			SendDlgItemMessage(hwndDlg, LST_FUNCTIONS, LB_SETCOLUMNWIDTH, 200, 0);
			SendDlgItemMessage(hwndDlg, LST_FUNCTIONS_SORTED, LB_SETCOLUMNWIDTH, 200, 0);

			BOOL Sorted = IsDlgButtonChecked(hwndDlg, CHK_LIST_SORTED);
			ShowWindow(GetDlgItem(hwndDlg, LST_FUNCTIONS), Sorted ? SW_HIDE : SW_SHOW);
			ShowWindow(GetDlgItem(hwndDlg, LST_FUNCTIONS_SORTED), !Sorted ? SW_HIDE : SW_SHOW);
			return TRUE;

		case WM_SIZE:
			{
			int nWidth = LOWORD(lParam);
			int nHeight = HIWORD(lParam);
			MoveWindow(GetDlgItem(hwndDlg, LST_FUNCTIONS),        2, 30, nWidth - 5, nHeight - 30, TRUE);
			MoveWindow(GetDlgItem(hwndDlg, LST_FUNCTIONS_SORTED), 2, 30, nWidth - 5, nHeight - 30, TRUE);
			return TRUE;
			}

		case WM_COMMAND:
			if ((LOWORD(wParam) == CHK_LIST_SORTED) && (HIWORD(wParam) == BN_CLICKED))
			{
				BOOL Sorted = IsDlgButtonChecked(hwndDlg, CHK_LIST_SORTED);
				ShowWindow(GetDlgItem(hwndDlg, LST_FUNCTIONS), Sorted ? SW_HIDE : SW_SHOW);
				ShowWindow(GetDlgItem(hwndDlg, LST_FUNCTIONS_SORTED), !Sorted ? SW_HIDE : SW_SHOW);
				SetFocus(g_hwndMain);
			}
			else if ((LOWORD(wParam) == BTN_FUNCTION_BEGIN) && (HIWORD(wParam) == BN_CLICKED))
			{
				src = GetSourceText();	// Get all file text

				curLine = GetCurLine();
				if (curLine == -1)
					return TRUE;

				func.sFunc[0] = '\0';
				FindFunc(src, OnFindFuncStart, OnFindFuncEnd);
				GetPureFunctionName();

				if (func.sFunc[0] == '\0')
				{
					MessageBox(g_hwndMain, "You're not in a function body.\n", "Function", MB_ICONEXCLAMATION);
					return FALSE;
				}

				if (func.iStart > 0)
					SetCursorPosition(func.iStart - 1, POS_LINE | POS_SCROLL);

				free(src);
				SetFocus(g_hwndMain);
			}
			else if ((LOWORD(wParam) == BTN_FUNCTION_END) && (HIWORD(wParam) == BN_CLICKED))
			{
				src = GetSourceText();	// Get all file text

				curLine = GetCurLine();
				if (curLine == -1)
					return TRUE;

				func.sFunc[0] = '\0';
				FindFunc(src, OnFindFuncStart, OnFindFuncEnd);
				GetPureFunctionName();

				if (func.sFunc[0] == '\0')
				{
					MessageBox(g_hwndMain, "You're not in a function body.\n", "Function", MB_ICONEXCLAMATION);
					return FALSE;
				}

				if (func.iEnd > 0)
					SetCursorPosition(func.iEnd - 2, POS_LINE | POS_SCROLL);

				free(src);
				SetFocus(g_hwndMain);
			}
			else if ((LOWORD(wParam) == LST_FUNCTIONS) && (HIWORD(wParam) == LBN_SELCHANGE /*LBN_DBLCLK*/))
			{
				int i = SendDlgItemMessage(hwndDlg, LST_FUNCTIONS, LB_GETCURSEL, 0, 0);

				SendDlgItemMessage(hwndDlg, LST_FUNCTIONS, LB_GETTEXT, i, (LPARAM)FunctionToSelect);
				SendDlgItemMessage(hwndDlg, LST_FUNCTIONS, LB_SETCURSEL, -1, 0);

				SelectFunction(FunctionToSelect);
				SetFocus(g_hwndMain);
			}
			else if ((LOWORD(wParam) == LST_FUNCTIONS_SORTED) && (HIWORD(wParam) == LBN_DBLCLK))
			{
				int i = SendDlgItemMessage(hwndDlg, LST_FUNCTIONS_SORTED, LB_GETCURSEL, 0, 0);

				SendDlgItemMessage(hwndDlg, LST_FUNCTIONS_SORTED, LB_GETTEXT, i, (LPARAM)FunctionToSelect);
				SendDlgItemMessage(hwndDlg, LST_FUNCTIONS_SORTED, LB_SETCURSEL, -1, 0);

				SelectFunction(FunctionToSelect);
				SetFocus(g_hwndMain);
			}
			break;

		case WM_CLOSE:
			EndDialog(hwndDlg, 0);
			return TRUE;
	}

	return FALSE;
}
/*******************************************************************************

 Function: MySubclassProc

 Purpose : Add-in source document window subclass procedure

*******************************************************************************/
static LRESULT CALLBACK MySubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Process any messages here. Normally, all (most) messages should be passed
	// on to the original window procedure, for the IDE to function properly.
	// In this sample we just convert all typed characters to upper case.
	if (msg == WM_SETFOCUS)
	{
		ShowFunctionsList();
		curLine = 0;
	}
	else if (msg ==  WM_KEYDOWN || msg == WM_LBUTTONUP)
	{
		SetFocusOnCurrFunc();
	}
	// Call the original source document window procedure.
	return CallWindowProc(GETOLDWNDPROC(hwnd), hwnd, msg, wParam, lParam);
}
