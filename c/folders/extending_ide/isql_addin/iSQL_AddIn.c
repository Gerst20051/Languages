/****************************************************************************
 *                                                                          *
 * File    : iSQL_AddIn.c                                                   *
 *                                                                          *
 * Purpose : Add-In main module.                                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <addin.h>
#include <tchar.h>
#include "iSQL_AddIn.h"
#include <commctrl.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <stdlib.h>








//-----------------------------------------------------------------------------
// Macro to handle errors in ODBC calls.
//-----------------------------------------------------------------------------
#define TRYODBC(h,ht,x) \
{ \
    RETCODE rc = x; \
    if (rc != SQL_SUCCESS) \
    { \
        handle_error(h,ht,rc); \
    } \
    if (rc == SQL_ERROR) \
    { \
        MessageBox(NULL, #x,"Errror",0); \
        goto Exit;      \
    }  \
}
//
// Misc. constants.
//
#define DISPLAY_MAX             50              // maximum column width
#define DISPLAY_FORMAT_EXTRA    3               // extra characters per column ("| <data> ")
#define DISPLAY_FORMAT          "%*.*s "     // column format
#define DISPLAY_FORMAT_C        "%-*.*s "    // text column format
#define NULL_SIZE               6               // length of "<NULL>"
#define PIPE                    '|'

//-----------------------------------------------------------------------------
// Structure holding column information.
//-----------------------------------------------------------------------------
typedef struct STR_BINDING {
    SQLSMALLINT colwidth;           // column width
    char *buffer;                   // data buffer; from SQLFetch()
    SQLINTEGER indicator;           // indicator; from SQLFetch()
    BOOL ischar;                    // is this column containing text?
    struct STR_BINDING *next;       // pointer to next node
} BINDING;

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------

static LRESULT CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);
HWND StatusBar_Create(HWND hwndParent, int nStatusID, HINSTANCE  hinst, int nParts);
void StatusBar_SetText(HWND hwndStatus, int nPart,LPTSTR szText);
static void handle_error(SQLHANDLE, SQLSMALLINT, RETCODE);

static void display_results(HWND ,HSTMT, SQLSMALLINT);
static void allocate_bindings(HSTMT, SQLSMALLINT, BINDING **, SQLSMALLINT *);
static void display_titles(HWND, HSTMT, DWORD, BINDING *);
void ExecuteSQLStatement(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

HINSTANCE hInst;
HENV env_handle = NULL;
HDBC dbc_handle = NULL;
HSTMT stmt_handle = NULL;
char *connection;







// Locals.
static HANDLE g_hmod = NULL;
static HWND g_hwndMain = NULL;
static HWND g_hwndProj = NULL;
static HWND g_hwndiSQLPage = NULL;
static BOOL g_Connected= FALSE;

//Private prototypes
static void OutputInfo(PCTSTR);

/****************************************************************************
 *                                                                          *
 * Function: DllMain                                                        *
 *                                                                          *
 * Purpose : DLL entry and exit procedure.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
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
 *                                                                          *
 * Function: AddInMain                                                      *
 *                                                                          *
 * Purpose : Add-in entry and exit procedure.                               *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

ADDINAPI BOOL WINAPI AddInMain(HWND hwnd, ADDIN_EVENT eEvent)
{


	MSG Msg;
	HACCEL hAccel;
    /*
     * This is the only required export function in an Add-In.
     */
    switch (eEvent)
    {
        /*
         * Application events.
         */
        case AIE_APP_CREATE:
            // Remember main window handle, in case we need it.
            g_hwndMain = hwnd;
						
		
						// Initialize common controls. Also needed for MANIFEST's.
						InitCommonControls();

						
						aja:
						//Create New Tab
						g_hwndiSQLPage=CreateDialog(g_hmod , MAKEINTRESOURCE(DLG_MAIN), hwnd, (DLGPROC)MainDlgProc);
						
						//g_hwndiSQLPage=CreateWindowEx(0,"STATIC","AJA",WS_CHILD|WS_VISIBLE,0,0,0,0,hwnd,0,NULL,NULL);
						AddIn_AddTabPage(hwnd,"iSQL",g_hwndiSQLPage);
						//Make sure the user knows about it
						Exit:
						OutputInfo("Registering iSQL Add-In ... Made by sanotto@yahoo.com");
						
            return TRUE;

        case AIE_APP_DESTROY:
            // Forget main window handle.
            g_hwndMain = NULL;
						OutputInfo("De-Registering iSQL Add-In ... Bye Bye");
						DestroyWindow(g_hwndiSQLPage);

						  // Release ODBC handles and quit.
  if (dbc_handle)
  {
  	SQLDisconnect(dbc_handle);
    SQLFreeConnect(dbc_handle);
  }
  if (env_handle) SQLFreeEnv(env_handle);
            return TRUE;

        /*
         * Project events.
         */
        case AIE_PRJ_CREATE:
            // Remember project window handle, in case we need it.
            g_hwndProj = hwnd;
            return TRUE;

        case AIE_PRJ_DESTROY:
            // Forget project window handle.
            g_hwndProj = NULL;
            return TRUE;

        case AIE_PRJ_SAVE:
            return TRUE;

        case AIE_PRJ_STARTBUILD:
            return TRUE;

        case AIE_PRJ_ENDBUILD:
            return TRUE;

        /*
         * MDI Document events.
         */
        case AIE_DOC_CREATE:
            return TRUE;

        case AIE_DOC_DESTROY:
            return TRUE;

        case AIE_DOC_SAVE:
            return TRUE;

        default:
            return TRUE;
    }
}

/****************************************************************************
 *                                                                          *
 * Function: AddInCommandEx                                                 *
 *                                                                          *
 * Purpose : Add-in command handler.                                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

ADDINAPI void WINAPI AddInCommandEx(int idCmd, LPCVOID pcvData)
{
    /*
     * This export is optional.
     */
}

/****************************************************************************
 *                                                                          *
 * Function: AddInSetup                                                     *
 *                                                                          *
 * Purpose : Add-in setup handler.                                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

ADDINAPI void WINAPI AddInSetup(HWND hwnd, LPCVOID pcvData)
{
    /*
     * This export is optional.
     */
}


static void OutputInfo(PCTSTR pszEvent)
{
	
		AddIn_WriteOutput(g_hwndMain, _T(pszEvent));
	
}


//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------


/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Initialize the application.  Register a window class,          *
 *           create and display the main window and enter the               *
 *           message loop.                                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/


	
	




/****************************************************************************
 *                                                                          *
 * Function: MainDlgProc                                                    *
 *                                                                          *
 * Purpose : Process messages for the Main dialog.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hwndStsBar;
	int nWidth;
	int nHeight;
	char szUserName[MAX_COMPUTERNAME_LENGTH + 1];
	char szComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	ULONG nSizeUserName=sizeof(szUserName);
	ULONG nSizeComputerName=sizeof(szComputerName);

  switch (uMsg)
    {
        case WM_INITDIALOG:
						//hwndStsBar=StatusBar_Create(hwndDlg, 9001, hInst, 4);
						//GetUserName(szUserName, &nSizeUserName);
						//StatusBar_SetText(hwndStsBar,0,szUserName);
						//GetComputerName(szComputerName,&nSizeComputerName);
						//StatusBar_SetText(hwndStsBar,1,szComputerName);
    				//nWidth=GetDeviceCaps(GetDC(hwndDlg), HORZRES	);						 
    				//nHeight=GetDeviceCaps(GetDC(hwndDlg), VERTRES	);						 
						//MoveWindow(hwndDlg, (nWidth/2)-(nWidth/4), 
						//                    (nHeight/2)-(nHeight/4),	
						//									  (nWidth/2),
						//										(nHeight/2), 
						//										TRUE);
						RegisterHotKey(hwndDlg, 0,MOD_ALT,VK_RETURN);
						EnableWindow(GetDlgItem(hwndDlg,	IDB_EXECUTE),FALSE);	
						EnableWindow(GetDlgItem(hwndDlg,	SQL_SENTENCE),FALSE);	
						EnableWindow(GetDlgItem(hwndDlg,	DLG_TABLE),FALSE);	
            return TRUE;
						break;
				case WM_PAINT:
						SetFocus(GetDlgItem(hwndDlg,	SQL_SENTENCE ));
						break;
        case WM_SIZE:

						nWidth = LOWORD(lParam); 
						nHeight = HIWORD(lParam);
						  
						MoveWindow(GetDlgItem(hwndDlg,	SQL_SENTENCE ),	2,4,nWidth-100, 50, TRUE);	
						MoveWindow(GetDlgItem(hwndDlg,	DLG_TABLE),2,60,nWidth-4,nHeight -76, TRUE);	
						MoveWindow(GetDlgItem(hwndDlg,	IDC_CONNECT),nWidth-90,6,84,20, TRUE);	
						MoveWindow(GetDlgItem(hwndDlg,	IDB_EXECUTE),nWidth-90,35,84,20, TRUE);	
						MoveWindow(GetDlgItem(hwndDlg,	IDL_WARNING),2,nHeight-16,250,15, TRUE);	
            return TRUE;

					case WM_DESTROY:
						EndDialog(hwndDlg, TRUE);
						PostQuitMessage(0);
            return TRUE;
					break;

        case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam))
            {  
							case MENU_SALIR:
										// MessageBoxA(hwndDlg,"Leaving from menu", "Wow the use knows how to use a menu!!!",0);
										// EndDialog(hwndDlg, TRUE);
										// PostQuitMessage(0);
									  return TRUE;
							case IDC_CONNECT:
								g_Connected=~g_Connected;
								if (g_Connected) {

								// Allocate an environment handle.
  							if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_handle) == SQL_ERROR)
  							{
  								MessageBox(NULL,"Unable to allocate SQL environment handle", "Error",0);
    							goto Exit;
  							}
								// Register this program as a ODBC 2.x application.
  							TRYODBC(env_handle, SQL_HANDLE_ENV, SQLSetEnvAttr(env_handle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC2, 0));

  							// Allocate a connection handle.
  							TRYODBC(env_handle, SQL_HANDLE_ENV, SQLAllocHandle(SQL_HANDLE_DBC, env_handle, &dbc_handle));

  							// Get any connection string (DSN) from the command line.
  							connection =  NULL;

  							// Connect to the driver. Use the connection string, if possible, or let the
  							// driver manager prompt for connection information.
  							TRYODBC(dbc_handle, SQL_HANDLE_DBC, SQLDriverConnect(dbc_handle, GetDesktopWindow(),
      				   (SQLCHAR *)connection, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE));
  							TRYODBC(dbc_handle, SQL_HANDLE_DBC, SQLAllocHandle(SQL_HANDLE_STMT, dbc_handle, &stmt_handle));
								// Enable buttons
	
								EnableWindow(GetDlgItem(hwndDlg,	IDB_EXECUTE),TRUE);	
								EnableWindow(GetDlgItem(hwndDlg,	SQL_SENTENCE),TRUE);
								EnableWindow(GetDlgItem(hwndDlg,	DLG_TABLE),TRUE);	
								SetWindowText(GetDlgItem(hwndDlg,	IDC_CONNECT),"&Disconnect");
								Exit:


								}
								else {
									EnableWindow(GetDlgItem(hwndDlg,	IDB_EXECUTE),FALSE);	
									EnableWindow(GetDlgItem(hwndDlg,	SQL_SENTENCE),FALSE);
									EnableWindow(GetDlgItem(hwndDlg,	DLG_TABLE),FALSE);	
									SetWindowText(GetDlgItem(hwndDlg,	IDC_CONNECT),"&Connect");
							  }
								return TRUE;
							case IDB_EXECUTE:
								ExecuteSQLStatement(hwndDlg, uMsg, wParam, lParam);
								return TRUE;

            }
            break;

				case WM_HOTKEY:	
					ExecuteSQLStatement(hwndDlg, uMsg, wParam, lParam);
					break;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
						PostQuitMessage(0);
            return TRUE;

    }

    return FALSE;
}
/****************************************************************************
 *                                                                          *
 * Function: allocate_bindings                                              *
 *                                                                          *
 * Purpose : Executes query if not empty.                                   *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/


void ExecuteSQLStatement(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	int len;
	char* input;
	char msg[2048];
	RETCODE retcode;
	
	if (~g_Connected) {
		MessageBox(hwndDlg, "You are not connected to a server...","Warning",MB_OK|MB_ICONWARNING);
	}
	len = GetWindowTextLength(GetDlgItem(hwndDlg,SQL_SENTENCE));
	if(len > 0) {		
		input = (char*)GlobalAlloc(GPTR, len + 1);
		GetDlgItemText(hwndDlg, SQL_SENTENCE, input, len + 1);
		
		// Execute the SQL query.
		OemToCharA(input, input);
    retcode = SQLExecDirect(stmt_handle, (SQLCHAR *)input, SQL_NTS);
    switch (retcode)
    {
			case SQL_SUCCESS_WITH_INFO:
            {
                handle_error(stmt_handle, SQL_HANDLE_STMT, retcode);
                // Fall through.
            }

            case SQL_SUCCESS:
            {
                SQLSMALLINT ncols;

                TRYODBC(stmt_handle, SQL_HANDLE_STMT, SQLNumResultCols(stmt_handle, &ncols));
                if (ncols > 0)
                {
                    // The SQL command produced a result. Display it!
                    display_results(hwndDlg,stmt_handle, ncols);
										
                }
                else
                {
                    SQLINTEGER rowcount;

                    TRYODBC(stmt_handle, SQL_HANDLE_STMT, SQLRowCount(stmt_handle, &rowcount));
                    if (rowcount >= 0)
                    {
                        // The SQL command made updates in the database.
                        sprintf(msg,"%d %s affected", rowcount, (rowcount == 1) ? "row" : "rows");
												MessageBox(NULL, input, msg,0);
                    }
                }
                break;
            }

            case SQL_ERROR:
            {
                // No great success...
                handle_error(stmt_handle, SQL_HANDLE_STMT, retcode);
                break;
            }

            default:
                fprintf(stderr, "Unexpected return code: %d!\n", retcode);
                break;
        }

        TRYODBC(stmt_handle, SQL_HANDLE_STMT, SQLFreeStmt(stmt_handle, SQL_CLOSE));
		}
			Exit:
									GlobalFree((HANDLE)input);
}


//-----------------------------------------------------------------------
// Function		: StatusBar_Create
// Task				: Creates a status bar and divides it into
//							the specified number of parts.
// Returns		: The handle to the status bar.
// Input:
// hwndParent	: Parent window for the status bar.
// nStatusID	: Child window identifier.
// hinst 			: Handle to the application instance.
// nParts 		: Number of parts into which to divide the status bar.
//-----------------------------------------------------------------------
HWND StatusBar_Create(HWND hwndParent, 
											int nStatusID, 
											HINSTANCE hinst, 
											int nParts)
{
    HWND hwndStatus;
    RECT rcClient;
    HLOCAL hloc;
    LPINT lpParts;
    int i, nWidth;

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the status bar.
    hwndStatus = CreateWindowEx(
        0,                       // no extended styles
        STATUSCLASSNAME,         // name of status bar class
        (LPCTSTR) NULL,          // no text when first created
        SBARS_SIZEGRIP |         // includes a sizing grip
        WS_CHILD,                // creates a child window
        0, 0, 0, 0,              // ignores size and position
        hwndParent,              // handle to parent window
        (HMENU) nStatusID,       // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    // Get the coordinates of the parent window's client area.
    GetClientRect(hwndParent, &rcClient);

    // Allocate an array for holding the right edge coordinates.
    hloc = LocalAlloc(LHND, sizeof(int) * nParts);
    lpParts = LocalLock(hloc);

    // Calculate the right edge coordinate for each part, and
    // copy the coordinates to the array.
    nWidth = rcClient.right / nParts;
    for (i = 0; i < nParts; i++) { 
       lpParts[i] = nWidth;
       nWidth += nWidth;
    }

    // Tell the status bar to create the window parts.
    SendMessage(hwndStatus, SB_SETPARTS, (WPARAM) nParts, (LPARAM)
               lpParts);
		//Show the StatusBar
		ShowWindow(hwndStatus, SW_SHOWNORMAL);
		UpdateWindow(hwndStatus);
    // Free the array, and return.
    LocalUnlock(hloc);
    LocalFree(hloc);
    return hwndStatus;
}  


//-----------------------------------------------------------------------
// Function		: StatusBar_SetText
// Task				: Set Status bar text
// Returns		: void
// Input:
// hwndStatus	: Handle to Status Bar
// nPart			: Part number for text 
// szText			: Text to set
//-----------------------------------------------------------------------
void StatusBar_SetText(HWND hwndStatus, 
												int nPart,
												LPTSTR szText)
{
	SendMessage(hwndStatus, SB_SETTEXT, (WPARAM) nPart, (LPARAM) szText);
}



/****************************************************************************
 *                                                                          *
 * Function: handle_error                                                   *
 *                                                                          *
 * Purpose : Display error messages.                                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void handle_error(SQLHANDLE handle, SQLSMALLINT handle_type, RETCODE retcode)
{
		
    SQLSMALLINT recno = 0;
    SQLINTEGER native_error;
    char message[1000];
    char state[SQL_SQLSTATE_SIZE];

    if (retcode == SQL_INVALID_HANDLE)
    {
        MessageBox(NULL,"Invalid handle!","Error",0);
        return;
    }

    // Sometimes you get multiple errors; loop through all of them.
    while (SQLGetDiagRec(handle_type, handle, ++recno, (SQLCHAR *)state, &native_error,
        (SQLCHAR *)message, (SQLSMALLINT)sizeof(message), NULL) == SQL_SUCCESS)
    {
        // Ignore some messages...
        if (strncmp(state, "01004", 5) != 0)
        {
            // Map foreign characters to OEM text and display.
            CharToOemA(message, message);

            MessageBox(NULL,message,"Error", 0);
        }
    }
}
/****************************************************************************
 *                                                                          *
 * Function: display_results                                                *
 *                                                                          *
 * Purpose : Display results from a database query.                         *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void display_results(HWND hwndDlg, HSTMT stmt_handle, SQLSMALLINT ncols)
{
    BINDING *first_binding, *this_binding;
    SQLSMALLINT totwidth;
    RETCODE retcode;
		char buffer[2048];
    int count = 0;
		LV_ITEM lvi;
		int i=0;

		HWND hwndLV=GetDlgItem(hwndDlg, DLG_TABLE);

    // Allocate memory for each column.
    allocate_bindings(stmt_handle, ncols, &first_binding, &totwidth);

    // Clear ListView and set column labels
    display_titles(hwndDlg,stmt_handle, totwidth, first_binding);


    // Fetch and display data.
    for (;;)
    {
        // Fetch a row.

        

        TRYODBC(stmt_handle, SQL_HANDLE_STMT, retcode = SQLFetch(stmt_handle));
        if (retcode == SQL_NO_DATA_FOUND) break;
				if (count > 6000) {
					MessageBox(NULL,"Max records reached", "Warning",0);
					break;
				}
        // Display data.
				i=0;
        for (this_binding = first_binding; this_binding != NULL; this_binding = this_binding->next)
        {
            if (this_binding->indicator != SQL_NULL_DATA)
            {
                // Map foreign characters to OEM text and display.
                //CharToOemA(this_binding->buffer, this_binding->buffer);
                sprintf(buffer, this_binding->ischar ? DISPLAY_FORMAT_C : DISPLAY_FORMAT, 
                this_binding->colwidth, this_binding->colwidth, this_binding->buffer);
            }
            else
            {
                sprintf(buffer, DISPLAY_FORMAT_C, this_binding->colwidth, this_binding->colwidth, "<NULL>");            
						}

						lvi.mask = LVIF_TEXT; 
						lvi.iItem =count;
						lvi.iSubItem =i++;

						lvi.pszText = buffer; 							  
						if (i==1)
						{	
						  ListView_InsertItem(hwndLV, &lvi);
						}
						else {
						  ListView_SetItem(hwndLV, &lvi);
						}
        }
				count++;	
    }
		

    
Exit:
    // Release allocated memory.
    while (first_binding)
    {
        this_binding = first_binding->next;
        free(first_binding->buffer);
        free(first_binding);
        first_binding = this_binding;
    }
}

/****************************************************************************
 *                                                                          *
 * Function: allocate_bindings                                              *
 *                                                                          *
 * Purpose : Get column information and create bindings.                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void allocate_bindings(HSTMT stmt_handle, SQLSMALLINT ncols, BINDING **first_binding, SQLSMALLINT *totwidth)
{
    SQLSMALLINT col;
    BINDING *this_binding, *last_binding;
    SQLINTEGER display_size, concise_type;
    SQLSMALLINT colname_size;

    *totwidth = 0;

    for (col = 1; col <= ncols; col++)
    {
        this_binding = malloc(sizeof(*this_binding));
        if (!this_binding)
        {
             MessageBox(NULL, "Out of memory","Error",0); 
        PostQuitMessage(0);
				return ; 
        }

        if (col == 1)
            *first_binding = this_binding;
        else
            last_binding->next = this_binding;
        last_binding = this_binding;

        // Figure out the presentation width of a column (we only bind to the
        // data type char because we just *display* data. Normally, you bind
        // to the most appropriate C type for performance reasons.)
        //
        TRYODBC(stmt_handle, SQL_HANDLE_STMT, SQLColAttribute(stmt_handle,
            col, SQL_DESC_DISPLAY_SIZE, NULL, 0, NULL, &display_size));

        // Figure out if the column is numeric or alpha-numeric.
        //
        TRYODBC(stmt_handle, SQL_HANDLE_STMT, SQLColAttribute(stmt_handle,
            col, SQL_DESC_CONCISE_TYPE, NULL, 0, NULL, &concise_type));

        this_binding->ischar = (concise_type == SQL_CHAR || concise_type == SQL_VARCHAR || concise_type == SQL_LONGVARCHAR);
        this_binding->next = NULL;

        if (display_size > DISPLAY_MAX)
            display_size = DISPLAY_MAX;

        // Allocate a buffer big enough to hold a text representation of the data.
        // Add room for the terminating null character.
        //
        this_binding->buffer = (char *)malloc((display_size + 1) * sizeof(char));
        if (!this_binding->buffer)
        {
                 MessageBox(NULL, "Out of memory","Error",0); 
        PostQuitMessage(0);
				return ; 
        }

        // Map the buffer we just allocated to the drivers buffer. When the data is
        // later fetched with SQLFetch, the driver will update this buffer.
        // Note! The size is given in bytes, not characters.
        //
        TRYODBC(stmt_handle, SQL_HANDLE_STMT, SQLBindCol(stmt_handle, col, SQL_C_TCHAR,
            (SQLPOINTER)this_binding->buffer, (display_size + 1) * sizeof(char), &this_binding->indicator));

        // Figure of the size of the column caption and decide the final
        // column width for presentation.
        //
        TRYODBC(stmt_handle, SQL_HANDLE_STMT, SQLColAttribute(stmt_handle,
            col, SQL_DESC_NAME, NULL, 0, &colname_size, NULL));

        this_binding->colwidth = max(display_size, colname_size);
        if (this_binding->colwidth < NULL_SIZE)
            this_binding->colwidth = NULL_SIZE;

        *totwidth += this_binding->colwidth + DISPLAY_FORMAT_EXTRA;
    }

Exit: ;
}

/****************************************************************************
 *                                                                          *
 * Function: display_titles                                                 *
 *                                                                          *
 * Purpose : Display all column captions.                                   *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void display_titles(HWND hwndDlg, HSTMT stmt_handle, DWORD totwidth, BINDING *binding)
{
    char title[DISPLAY_MAX];
    SQLSMALLINT col;
    LVCOLUMN lvc; 
		HWND hwndLV=GetDlgItem(hwndDlg, DLG_TABLE);
   
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM; 

		//Delete all data in ListView
		ListView_DeleteAllItems(hwndLV);
		//Delete all columns
		for(;ListView_DeleteColumn(hwndLV,0););
    // Loop through all bindings (=columns).
		
    for (col = 1; binding != NULL; binding = binding->next, col++)
    {
        // Get the caption for column <col>.
        TRYODBC(stmt_handle, SQL_HANDLE_STMT, SQLColAttribute(stmt_handle,
								col, SQL_DESC_NAME, title, sizeof(title), NULL, NULL));

        // Map foreign characters to OEM text and display.
        //CharToOemA(title, title);
    		lvc.iSubItem = col;
        lvc.pszText = title ;	
        lvc.cx = 100;     // width of column in pixels
        ListView_InsertColumn(hwndLV,col, &lvc);
    }

Exit:
    
}

