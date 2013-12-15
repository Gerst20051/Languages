/****************************************************************************
 *                                                                          *
 * Filename: reversi.c                                                      *
 *                                                                          *
 * Purpose : Pocket PC sample for Pelles C for Windows.                     *
 *                                                                          *
 *           Reversi game. Based on an old Microsoft sample.                *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *           07-06-04  Small revisions to work on Windows Mobile 5.         *
 *                                                                          *
 ****************************************************************************/

#define UNICODE
#include <windows.h>
#include <windowsx.h>
#include <aygshell.h>
#include "reversi.h"

#define BOARDSIZE  100
#define EDGE  0
#define EMPTY  1
#define HUMAN  2
#define COMPUTER  3
#define INFIN  32767
#define WIN  32000
#define LOSS  (-32000)
#define MAXDEPTH  6
#define PASS  20

/* #define TRACE(sz)  MessageBox(NULL, TEXT(sz), L"Trace", MB_SETFOREGROUND|MB_OK); */

#define NELEMS(arr)  (sizeof(arr) / sizeof(arr[0]))

#define TID_FLASH  666
#define MENU_HEIGHT  26

/** Global variables ********************************************************/

static PCTSTR g_pszMess;
static RECT g_rcMess;

static int g_cFlashTimes;
static int g_cMaxFlashTimes;

static HBRUSH g_hbrBlack;
static HBRUSH g_hbrPat;
static HBRUSH g_hbrWhite;
static HBRUSH g_hbrBkgnd;
static HBRUSH g_hbrHint;
static HBRUSH g_hbrHuman;
static HBRUSH g_hbrComputer;

static HINSTANCE g_hInst;
static HWND g_hwndMB;

static int g_cxSquare;
static int g_cySquare;
static int g_cxBorder;
static int g_cyBorder;
static int g_nDimension;
static int g_cyChar;

static BOOL g_fPocketPC2002 = FALSE;
static BOOL g_fThinking = FALSE;
static BOOL g_fFirstMove;
static BOOL g_fMouseDown = FALSE;

static TCHAR g_szAppName[] = TEXT("Reversi");
static TCHAR g_szReversi[20];
static TCHAR g_szReversiPractice[40];
static TCHAR g_szPass[30];
static TCHAR g_szMustPass[30];
static TCHAR g_szTie[30];
static TCHAR g_szLoss[30];
static TCHAR g_szWon[30];
static TCHAR g_szIllegal[70];
static TCHAR g_szNoPass[70];

static BYTE g_board[MAXDEPTH+2][BOARDSIZE];
static int g_bestmove[MAXDEPTH+2];
static int g_depth = 1;
static BOOL g_fCanPass = FALSE;

static const int g_aiDirection[9] = { 9, 10, 11, 1, -1, -9, -10, -11, 0 };

static const int g_moves[61] =
{
    11,18,81,88, 13,31,16,61,
    38,83,68,86, 14,41,15,51,
    48,84,58,85, 33,36,63,66,
    34,35,43,46, 53,56,64,65,
    24,25,42,47, 52,57,74,75,
    23,26,32,37, 62,67,73,76,
    12,17,21,28, 71,78,82,87,
    22,27,72,77, 0
};

/** Prototypes **************************************************************/

static LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
static BOOL Main_OnCreate(HWND, CREATESTRUCT *);
static void Main_OnDestroy(HWND);
static void Main_OnSize(HWND, UINT, int, int);
static void Main_OnPaint(HWND);
static void DrawBoard(HDC, BYTE [MAXDEPTH+2][BOARDSIZE]);
static void DrawPieces(HDC, BYTE [MAXDEPTH+2][BOARDSIZE]);
static void PaintMove(HDC, BYTE [BOARDSIZE], int, BYTE, BYTE);
static void Main_OnInitMenuPopup(HWND, HMENU, UINT, BOOL);
static void Main_OnCommand(HWND, int, HWND, UINT);
static void NewGame(HWND);
static void ClearBoard(BYTE [MAXDEPTH+2][BOARDSIZE]);
static void UserPasses(HWND);
static void ShowBestMove(HWND);
static void Main_OnLButtonDown(HWND, BOOL, int, int, UINT);
static void Main_OnLButtonUp(HWND, int, int, UINT);
static void Main_OnSettingChange(HWND, UINT, PCTSTR);
static void IsGameOver(HWND, BYTE [MAXDEPTH+2][BOARDSIZE], int);
static int GetBestMove(HDC, BYTE [MAXDEPTH+2][BOARDSIZE], int, BYTE, BYTE, int, int, int);
static int IsValidMove(BYTE [], int, BYTE, BYTE);
static void MakeMove(BYTE [], int, BYTE, BYTE);
static int Score(BYTE [], BYTE, BYTE);
static int FinalScore(BYTE [], BYTE, BYTE);
static void Message(HWND, PCTSTR, ...);
static void SetMessage(HWND, PCTSTR);
static void CALLBACK InvertMessage(HWND, UINT, UINT, DWORD);
static BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
static BOOL IsPocketPC2002(void);

/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Initializes the application.  Registers the window class,      *
 *           creates and displays the main window and enters the message    *
 *           loop.                                                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG msg;

    hwnd = FindWindow(g_szAppName, NULL);
    if (hwnd)
    {
        SetForegroundWindow(hwnd);
        return 0;
    }

    g_fPocketPC2002 = IsPocketPC2002();
    g_hInst = hInstance;

    g_hbrBkgnd = CreateSolidBrush(RGB(0xFF,0xFF,0));
    g_hbrHint = CreateSolidBrush(RGB(0xFF,0,0));
    if (!g_hbrBkgnd || !g_hbrHint) return 1;

    if (!hPrevInstance)
    {
        WNDCLASS wc;

        wc.lpszClassName = g_szAppName;
        wc.lpfnWndProc = MainWndProc;
        wc.style = CS_VREDRAW|CS_HREDRAW;
        wc.hInstance = hInstance;
        wc.hIcon = NULL;
        wc.hCursor = NULL;
        wc.hbrBackground = g_hbrBkgnd;
        wc.lpszMenuName = NULL;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        if (!RegisterClass(&wc))
            return 1;
    }

    hwnd = CreateWindowEx(
        (g_fPocketPC2002 ? 0 : WS_EX_CAPTIONOKBTN),
        g_szAppName,
        NULL,
        WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (!hwnd)
        return 1;

    // When the main window is created using CW_USEDEFAULT the height of the menubar (if one
    // is created is not taken into account). So we resize the window after creating it
    // if a menubar is present.
    if (g_hwndMB)
    {
        RECT rcWin, rcMB;

        GetWindowRect(hwnd, &rcWin);
        GetWindowRect(g_hwndMB, &rcMB);
        rcWin.bottom -= (rcMB.bottom - rcMB.top);
        MoveWindow(hwnd, rcWin.left, rcWin.top, rcWin.right - rcWin.left, rcWin.bottom - rcWin.top, FALSE);
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteBrush(g_hbrBkgnd);
    DeleteBrush(g_hbrHint);

    return 0;
}

/****************************************************************************
 *                                                                          *
 * Function: MainWndProc                                                    *
 *                                                                          *
 * Purpose : Handles messages for the main window.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, Main_OnCreate);
        HANDLE_MSG(hwnd, WM_DESTROY, Main_OnDestroy);
        HANDLE_MSG(hwnd, WM_SIZE, Main_OnSize);
        HANDLE_MSG(hwnd, WM_PAINT, Main_OnPaint);
        HANDLE_MSG(hwnd, WM_INITMENUPOPUP, Main_OnInitMenuPopup);
        HANDLE_MSG(hwnd, WM_COMMAND, Main_OnCommand);
        HANDLE_MSG(hwnd, WM_LBUTTONDOWN, Main_OnLButtonDown);
        HANDLE_MSG(hwnd, WM_LBUTTONUP, Main_OnLButtonUp);
        HANDLE_MSG(hwnd, WM_SETTINGCHANGE, Main_OnSettingChange);
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnCreate                                                  *
 *                                                                          *
 * Purpose : Handles a WM_CREATE message.                                   *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL Main_OnCreate(HWND hwnd, CREATESTRUCT *pcs)
{
    SHMENUBARINFO mbi;
    HDC hdc;

    //
    // Create the menubar.
    //
    memset(&mbi, 0, sizeof(mbi));
    mbi.cbSize = sizeof(mbi);
    mbi.hwndParent = hwnd;
    mbi.nToolBarId = IDM_MAIN_MENU;
    mbi.hInstRes = g_hInst;
    mbi.nBmpId = 0;
    mbi.cBmpImages = 0;
    if (!SHCreateMenuBar(&mbi))
        return FALSE;

    g_hwndMB = mbi.hwndMB;

    //
    // Calculate the height of a character.
    //
    hdc = GetDC(hwnd);
    if (hdc)
    {
        TEXTMETRIC tm;
        GetTextMetrics(hdc, &tm);
        g_cyChar = tm.tmHeight;
        ReleaseDC(hwnd, hdc);
    }

    //
    // Load resource-strings.
    //
    LoadString(g_hInst, IDS_REVERSI, g_szReversi, NELEMS(g_szReversi));
    LoadString(g_hInst, IDS_REVERSIPRACTICE, g_szReversiPractice, NELEMS(g_szReversiPractice));
    LoadString(g_hInst, IDS_PASS, g_szPass, NELEMS(g_szPass));
    LoadString(g_hInst, IDS_MUSTPASS, g_szMustPass, NELEMS(g_szMustPass));
    LoadString(g_hInst, IDS_TIE, g_szTie, NELEMS(g_szTie));
    LoadString(g_hInst, IDS_LOSS, g_szLoss, NELEMS(g_szLoss));
    LoadString(g_hInst, IDS_WON, g_szWon, NELEMS(g_szWon));
    LoadString(g_hInst, IDS_ILLEGAL, g_szIllegal, NELEMS(g_szIllegal));
    LoadString(g_hInst, IDS_NOPASS, g_szNoPass, NELEMS(g_szNoPass));

    //
    // Load GDI objects.
    //
    g_hbrWhite = GetStockBrush(WHITE_BRUSH);
    g_hbrBlack = GetStockBrush(BLACK_BRUSH);
    g_hbrPat = GetStockBrush(LTGRAY_BRUSH);
    g_hbrComputer = g_hbrBlack;
    g_hbrHuman = g_hbrWhite;

    NewGame(hwnd);

    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnDestroy                                                 *
 *                                                                          *
 * Purpose : Handles a WM_DESTROY message.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnDestroy(HWND hwnd)
{
    DestroyWindow(g_hwndMB);
    PostQuitMessage(0);
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnSize                                                    *
 *                                                                          *
 * Purpose : Handles a WM_SIZE message.                                     *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    if (state != SIZE_MINIMIZED)
    {
        RECT rcClient;
        int cxClient, cyClient;
        int cxBoard, cyBoard;

        GetClientRect(hwnd, &rcClient);
        cxClient = rcClient.right - rcClient.left;
        cyClient = rcClient.bottom - rcClient.top - MENU_HEIGHT;

        if (cxClient < cyClient)
        {
            if (cxClient < 45) cxClient = 45;
            g_cxSquare = cxClient / (9 + 1);
            g_cySquare = g_cxSquare;
        }
        else
        {
            if (cyClient < 45) cyClient = 45;
            g_cySquare = cyClient / (9 + 1);
            g_cxSquare = g_cySquare;
        }

        cyBoard = 8 * g_cySquare;
        cxBoard = 8 * g_cxSquare;
        g_nDimension = cyBoard / 30;

        g_cxBorder = (cxClient > cxBoard) ? (cxClient - cxBoard) / 2 : 0;
        g_cyBorder = (cyClient > cyBoard) ? (cyClient - cyBoard) / 2 : 0;

        SetRect(&g_rcMess, 0, 0, cxClient, g_cyChar);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnPaint                                                   *
 *                                                                          *
 * Purpose : Handles a WM_PAINT message.                                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;

    BeginPaint(hwnd, &ps);

    if (g_pszMess)
    {
        SetBkMode(ps.hdc, TRANSPARENT);
        DrawText(ps.hdc, g_pszMess, -1, &g_rcMess, DT_CENTER|DT_VCENTER);
    }

    if (ps.rcPaint.bottom > g_rcMess.bottom)
        DrawBoard(ps.hdc, g_board);

    EndPaint(hwnd, &ps);
}

/****************************************************************************
 *                                                                          *
 * Function: DrawBoard                                                      *
 *                                                                          *
 * Purpose : Draws the board.                                               *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void DrawBoard(HDC hdc, BYTE board[MAXDEPTH+2][BOARDSIZE])
{
    int cxBoard, cyBoard;
    int xRightEdge, yBottomEdge;
    int i;
    POINT apt[5];

    cyBoard = 8 * g_cySquare;
    cxBoard = 8 * g_cxSquare;

    /* draw black rectangles for the shadow */
    SelectBrush(hdc, g_hbrBlack);
    PatBlt(hdc, g_cxBorder + 2 * g_nDimension, g_cyBorder + 2 * g_nDimension, cxBoard, cyBoard, PATCOPY);

    /* draw gray rectangle for the board */
    SelectBrush(hdc, g_hbrPat);
    PatBlt(hdc, g_cxBorder, g_cyBorder, cxBoard, cyBoard, PATCOPY);

    /* draw the grid for the board */
    SelectBrush(hdc, g_hbrBlack);
    for (i = g_cxBorder; i <= g_cxBorder + cxBoard; i += g_cxSquare)
        PatBlt(hdc, i, g_cyBorder, 1, cyBoard, PATCOPY);
    for (i = g_cyBorder; i <= g_cyBorder + cyBoard; i += g_cySquare)
        PatBlt(hdc, g_cxBorder, i, cxBoard, 1, PATCOPY);

    xRightEdge = g_cxBorder + cxBoard;
    yBottomEdge = g_cyBorder + cyBoard;

    /* draw depth of board (above shadow) */
    SelectBrush(hdc, g_hbrPat);
    for (i = 1; i < g_nDimension; i++)
        PatBlt(hdc, xRightEdge + i, g_cyBorder + i, 1, cyBoard, PATCOPY);
    for (i = 1; i < g_nDimension; i++)
        PatBlt(hdc, g_cxBorder + i, yBottomEdge + i, cxBoard, 1, PATCOPY);

    /* draw black lines around the depth of the board */
    SelectBrush(hdc, g_hbrBlack);
    apt[0].x = xRightEdge; apt[0].y = g_cyBorder;
    apt[1].x = xRightEdge + g_nDimension; apt[1].y = g_cyBorder + g_nDimension;
    apt[2].x = xRightEdge + g_nDimension; apt[2].y = yBottomEdge + g_nDimension;
    apt[3].x = g_cxBorder + g_nDimension; apt[3].y = yBottomEdge + g_nDimension;
    apt[4].x = g_cxBorder; apt[4].y = yBottomEdge;
    Polyline(hdc, apt, 5);

    apt[0].x = xRightEdge + g_nDimension; apt[0].y = yBottomEdge + g_nDimension;
    apt[1].x = xRightEdge; apt[1].y = yBottomEdge;
    Polyline(hdc, apt, 2);

    DrawPieces(hdc, board);
}

/****************************************************************************
 *                                                                          *
 * Function: DrawPieces                                                     *
 *                                                                          *
 * Purpose : Draws the pieces on the board.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void DrawPieces(HDC hdc, BYTE board[MAXDEPTH+2][BOARDSIZE])
{
    int x, y;
    int sq;

    for (x = 0; x < 8; x++)
    {
        for (y = 0; y < 8; y++)
        {
            if ((sq = board[0][x*10+y+11]) != EMPTY)
            {
                SelectBrush(hdc, (sq == COMPUTER) ? g_hbrComputer : g_hbrHuman);

                Ellipse(hdc,
                    x * g_cxSquare + g_cxBorder + 2,
                    y * g_cySquare + g_cyBorder + 2,
                    x * g_cxSquare + g_cxBorder + 2 + g_cxSquare - 4,
                    y * g_cySquare + g_cyBorder + 2 + g_cySquare - 4);
            }
        }
    }
}

/****************************************************************************
 *                                                                          *
 * Function: PaintMove                                                      *
 *                                                                          *
 * Purpose : Combines MakeMove and Paint, without screen flicker.           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void PaintMove(HDC hdc, BYTE board[BOARDSIZE], int move, BYTE friend, BYTE enemy)
{
    int d;

    if (move != PASS)
    {
        int x = ((move - 11) / 10) * g_cxSquare + g_cxBorder + 2;
        int y = ((move - 11) % 10) * g_cySquare + g_cyBorder + 2;
        const int *p;

        if (friend == COMPUTER)
        {
            int i;
            for (i = 0; i < 3; i++)
            {
                SelectBrush(hdc, g_hbrPat);
                PatBlt(hdc, x, y, g_cxSquare - 4, g_cySquare - 4, PATCOPY);
                Sleep(200);

                SelectBrush(hdc, g_hbrComputer);
                Ellipse(hdc, x, y, x + g_cxSquare - 4, y + g_cySquare - 4);
                Sleep(200);
            }
        }
        else
        {
            SelectBrush(hdc, g_hbrHuman);
            Ellipse(hdc, x, y, x + g_cxSquare - 4, y + g_cySquare - 4);
        }

        p = g_aiDirection;
        while ((d = *p++) != 0)
        {
            int sq = move;

            if (board[sq += d] == enemy)
            {
                while (board[sq += d] == enemy)
                    ;

                if (board[sq] == friend)
                {
                    while (board[sq -= d] == enemy)
                    {
                        g_board[0][sq] = board[sq] = friend;
                        x = ((sq - 11) / 10) * g_cxSquare + g_cxBorder + 2;
                        y = ((sq - 11) % 10) * g_cySquare + g_cyBorder + 2;
                        Ellipse(hdc, x, y, x + g_cxSquare - 4, y + g_cySquare - 4);
                    }
                }
            }
        }

        board[move] = friend;
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnInitMenuPopup                                           *
 *                                                                          *
 * Purpose : Handles a WM_INITMENUPOPUP message.                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu)
{
    if (!fSystemMenu)
    {
        /* we don't care if it's the wrong popup */
        CheckMenuItem(hMenu, IDM_LEVEL_EASY, (g_depth == 1) ? MF_CHECKED : MF_UNCHECKED);
        CheckMenuItem(hMenu, IDM_LEVEL_MEDIUM, (g_depth == 2) ? MF_CHECKED : MF_UNCHECKED);
        CheckMenuItem(hMenu, IDM_LEVEL_HARD, (g_depth == 4) ? MF_CHECKED : MF_UNCHECKED);
        CheckMenuItem(hMenu, IDM_LEVEL_VHARD, (g_depth == 6) ? MF_CHECKED : MF_UNCHECKED);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnCommand                                                 *
 *                                                                          *
 * Purpose : Handles a WM_COMMAND message.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (g_fThinking)
        return;

    switch (id)
    {
        case IDM_GAME_NEW:
            NewGame(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
            break;

        case IDM_GAME_PASS:
            UserPasses(hwnd);
            break;

        case IDM_GAME_HINT:
            ShowBestMove(hwnd);
            break;

        case IDM_GAME_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0L);
            break;

        case IDM_GAME_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDW_ABOUT), hwnd, AboutDlgProc);
            break;

        case IDM_LEVEL_EASY:
            g_depth = 1;  /* must be at least 1 */
            break;

        case IDM_LEVEL_MEDIUM:
            g_depth = 2;
            break;

        case IDM_LEVEL_HARD:
            g_depth = 4;
            break;

        case IDM_LEVEL_VHARD:
            g_depth = 6;
            break;

        case IDOK:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            /* MessageBeep(MB_ICONERROR); */
            break;

        default:
            FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
            break;
    }
}

/****************************************************************************
 *                                                                          *
 * Function: NewGame                                                        *
 *                                                                          *
 * Purpose : Makes initializations for a new game.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void NewGame(HWND hwnd)
{
    SetWindowText(hwnd, g_szReversi);
    SetMessage(hwnd, NULL);

    g_fThinking = FALSE;
    g_fFirstMove = TRUE;
    g_fCanPass = TRUE;  /* okay to pass on the first move */

    ClearBoard(g_board);
}

/****************************************************************************
 *                                                                          *
 * Function: ClearBoard                                                     *
 *                                                                          *
 * Purpose : Clears the board and positions the starting pieces.            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void ClearBoard(BYTE board[MAXDEPTH+2][BOARDSIZE])
{
    int i, j;
    int k;

    for (i = 0; i <= MAXDEPTH; i++)
    {
        for (j = 0; j <= 99; j++)
            board[i][j] = EDGE;
    }

    for (i = 0; i <= MAXDEPTH; i++)
    {
        for (j = 11; j <= 81; j += 10)
        {
            for (k = j; k < j+8; k++)
                board[i][k] = EMPTY;
        }

        board[i][45] = COMPUTER;
        board[i][54] = COMPUTER;
        board[i][44] = HUMAN;
        board[i][55] = HUMAN;
    }
}

/****************************************************************************
 *                                                                          *
 * Function: UserPasses                                                     *
 *                                                                          *
 * Purpose : The user has choosen to pass.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void UserPasses(HWND hwnd)
{
    if (g_fCanPass)
    {
        HDC hdc;

        SetMessage(hwnd, NULL);

        hdc = GetDC(hwnd);
        if (hdc)
        {
            g_fThinking = TRUE;

            GetBestMove(hdc, g_board, PASS, HUMAN, COMPUTER, 0, -INFIN, INFIN);
            PaintMove(hdc, g_board[0], g_bestmove[0], COMPUTER, HUMAN);
            IsGameOver(hwnd, g_board, g_bestmove[0]);
            ReleaseDC(hwnd, hdc);

            g_fThinking = FALSE;
        }
    }
    else
    {
        /* You can't pass pass now, you dumb... */
        MessageBox(hwnd, g_szNoPass, g_szReversi, MB_OK|MB_ICONASTERISK);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: ShowBestMove                                                   *
 *                                                                          *
 * Purpose : Hints about the best move for the user.                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void ShowBestMove(HWND hwnd)
{
    const int *movesp;
    HDC hdc;
    int x, y;

    if (g_fCanPass && !g_fFirstMove)
        return;

    SetWindowText(hwnd, g_szReversiPractice);

    g_fThinking = TRUE;

    if (g_fFirstMove)
    {
        x = 4;  /* the first hint is hardcoded */
        y = 2;
    }
    else
    {
        if (g_depth == 1)
        {
            int move;
            movesp = g_moves;
            for (;;)
            {
                move = *movesp++;
                if (IsValidMove(g_board[0], move, HUMAN, COMPUTER))
                    break;
            }
            y = (move - 11) % 10;
            x = (move - 11) / 10;
        }
        else
        {
            GetBestMove(NULL, g_board, g_bestmove[0], COMPUTER, HUMAN, 1, -INFIN, INFIN);
            y = (g_bestmove[1] - 11) % 10;
            x = (g_bestmove[1] - 11) / 10;
        }
    }

    hdc = GetDC(hwnd);
    if (hdc)
    {
        MSG msg;
        int i;

        x = x * g_cxSquare + g_cxBorder + 2;
        y = y * g_cySquare + g_cyBorder + 2;

        for (i = 0; i < 10 && !PeekMessage(&msg, hwnd, WM_LBUTTONDOWN, WM_LBUTTONDOWN, PM_NOREMOVE); i++)
        {
            SelectBrush(hdc, g_hbrHint);
            Ellipse(hdc, x, y, x + g_cxSquare - 4, y + g_cySquare - 4);
            Sleep(200);

            SelectBrush(hdc, g_hbrPat);
            PatBlt(hdc, x, y, g_cxSquare - 4, g_cySquare - 4, PATCOPY);
            Sleep(200);
        }

        ReleaseDC(hwnd, hdc);
    }

    g_fThinking = FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnLButtonDown                                             *
 *                                                                          *
 * Purpose : Handles a WM_LBUTTONDOWN or WM_LBUTTONDBLCLK message.          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    SetCapture(hwnd);
    g_fMouseDown = TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnLButtonUp                                               *
 *                                                                          *
 * Purpose : Handles a WM_LBUTTONUP message.                                *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    ReleaseCapture();

    if (!g_fThinking && g_fMouseDown)
    {
        int cyBoard = 8 * g_cySquare;
        int cxBoard = 8 * g_cxSquare;
        int move;
        HDC hdc;

        if (g_cxSquare == 0 || g_cySquare == 0)
            return;

        if (x > g_cxBorder && x < (g_cxBorder + cxBoard) && y > g_cyBorder && y < (g_cyBorder + cyBoard))
        {
            x = (x - g_cxBorder) / g_cxSquare;
            y = (y - g_cyBorder) / g_cySquare;
            move = x * 10 + y + 11;

            if (IsValidMove(g_board[0], move, HUMAN, COMPUTER))
            {
                g_board[0][move] = HUMAN;

                SetMessage(hwnd, NULL);

                hdc = GetDC(hwnd);
                if (hdc)
                {
                    g_fFirstMove = FALSE;
                    g_fThinking = TRUE;

                    GetBestMove(hdc, g_board, move, HUMAN, COMPUTER, 0, -INFIN, INFIN);
                    MakeMove(g_board[0], move, HUMAN, COMPUTER);
                    PaintMove(hdc, g_board[0], g_bestmove[0], COMPUTER, HUMAN);
                    IsGameOver(hwnd, g_board, g_bestmove[0]);
                    ReleaseDC(hwnd, hdc);

                    g_fThinking = FALSE;
                }
            }
            else
            {
                /* No, no, you can't move to *that* square! */
                MessageBox(hwnd, g_szIllegal, g_szReversi, MB_OK|MB_ICONASTERISK);
            }
        }
    }

    g_fMouseDown = FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnSettingChange                                           *
 *                                                                          *
 * Purpose : Handles a WM_SETTINGCHANGE message.                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnSettingChange(HWND hwnd, UINT uiAction, PCTSTR pszSection)
{
    switch (uiAction)
    {
        case SPI_SETSIPINFO:
        {
            SHACTIVATEINFO sai;
            memset(&sai, 0, sizeof(SHACTIVATEINFO));
            /* Adjust the window size based on the SIP panel state */
            SHHandleWMSettingChange(hwnd, -1, 0, &sai);
            break;
        }
    }
}

/****************************************************************************
 *                                                                          *
 * Function: IsGameOver                                                     *
 *                                                                          *
 * Purpose : Checks if the game is over.                                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void IsGameOver(HWND hwnd, BYTE b[MAXDEPTH+2][BOARDSIZE], int bestmove)
{
    const int *movesp;
    int move;
    int sq;

    g_fCanPass = TRUE;
    movesp = g_moves;
    move = PASS;
    while ((sq = *movesp++) != 0)
    {
        if (IsValidMove(b[0], sq, HUMAN, COMPUTER))
            g_fCanPass = FALSE;
        else if (IsValidMove(b[0], sq, COMPUTER, HUMAN))
            move = sq;
    }

    if (g_fCanPass)
    {
        if (bestmove == PASS || move == PASS)
        {
            int hc = 0, cc = 0;
            int i;

            for (i = 11; i <= 88; i++)
            {
                if (b[0][i] == HUMAN)
                    hc++;
                else if (b[0][i] == COMPUTER)
                    cc++;
            }

            if (hc > cc)
                Message(hwnd, g_szWon, hc - cc);
            else if (hc < cc)
                Message(hwnd, g_szLoss, cc - hc);
            else
                Message(hwnd, g_szTie);
        }
        else
        {
            /* You must pass */
            Message(hwnd, g_szMustPass);
        }
    }
    else if (bestmove == PASS)
    {
        /* I must pass, in case you wonder... */
        Message(hwnd, g_szPass);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: GetBestMove                                                    *
 *                                                                          *
 * Purpose : Figures out the best move for the computer (or user).          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static int GetBestMove(HDC hdc, BYTE board[MAXDEPTH+2][BOARDSIZE], int move,
    BYTE friend, BYTE enemy, int ply, int vmin, int vmax)
{
    BYTE *cur_board, *p, *q;
    const int *movesp;
    int *best_movep;
    int i;
    int sq, value, cur_move;

    cur_board = &board[ply+1][0];

    p = &board[ply][11];
    q = &board[ply+1][11];
    for (i = 11; i <= 88; i++)
        *q++ = *p++;

    best_movep = &g_bestmove[ply];
    if (move == PASS)
    {
        if (ply == g_depth)
        {
            movesp = g_moves;
            while ((sq = *movesp++) != 0)
            {
                if (IsValidMove(cur_board, sq, enemy, friend))
                    return Score(cur_board, friend, enemy);
            }
            return FinalScore(cur_board, friend, enemy);
        }
    }
    else
    {
        if (ply == 0)
        {
            PaintMove(hdc, cur_board, move, friend, enemy);
        }
        else
        {
            MakeMove(cur_board, move, friend, enemy);
            if (ply == g_depth) return Score(cur_board, friend, enemy);
        }
    }

    movesp = g_moves;
    cur_move = PASS;
    *best_movep = PASS;
    while ((sq = *movesp++) != 0)
    {
        if (IsValidMove(cur_board, sq, enemy, friend))
        {
            cur_move = sq;
            value = GetBestMove(hdc, board, cur_move, enemy, friend, ply+1, -vmax, -vmin);
            if (value > vmin)
            {
                vmin = value;
                *best_movep = cur_move;
                if (value >= vmax) goto cutoff;  /* alpha-beta cutoff */
            }
        }
    }
    if (cur_move == PASS)
    {
        if (move == PASS)  /* two passes in a row mean game is over */
        {
            return FinalScore(cur_board, friend, enemy);
        }
        else
        {
            value = GetBestMove(hdc, board, PASS, enemy, friend, ply+1, -vmax, -vmin);
            if (value > vmin) vmin = value;
        }
    }

cutoff:
    return -vmin;
}

/****************************************************************************
 *                                                                          *
 * Function: IsValidMove                                                    *
 *                                                                          *
 * Purpose : Returns true if the move is valid, otherwise false.            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static int IsValidMove(BYTE board[], int move, BYTE friend, BYTE enemy)
{
    if (board[move] == EMPTY)
    {
        const int *p = g_aiDirection;
        int d;

        while ((d = *p++) != 0)
        {
            int sq = move;

            if (board[sq += d] == enemy)
            {
                while (board[sq += d] == enemy)
                    ;

                if (board[sq] == friend)
                    return TRUE;
            }
        }
    }

    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: MakeMove                                                       *
 *                                                                          *
 * Purpose : Makes a move (capture the enemy pieces).                       *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void MakeMove(BYTE board[], int move, BYTE friend, BYTE enemy)
{
    if (move != PASS)
    {
        const int *p = g_aiDirection;
        int d;

        while ((d = *p++) != 0)
        {
            int sq = move;

            if (board[sq += d] == enemy)
            {
                while (board[sq += d] == enemy)
                    ;

                if (board[sq] == friend)
                {
                    while (board[sq -= d] == enemy)
                        board[sq] = friend;
                }
            }
        }

        board[move] = friend;
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Score                                                          *
 *                                                                          *
 * Purpose : Calculates the current score.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static int Score(BYTE board[], BYTE friend, BYTE enemy)
{
    int fpoints = 0;
    int epoints = 0;
    int ecount = 0;
    int *valuep;
    int v;
    BYTE *boardp;

    static int value[79] =
    {
         99, -8,  8,  6,  6,  8, -8, 99,  0,
          0, -8,-24, -4, -3, -3, -4,-24, -8, 0,
          0,  8, -4,  7,  4,  4,  7, -4,  8, 0,
          0,  6, -3,  4,  0,  0,  4, -3,  6, 0,
          0,  6, -3,  4,  0,  0,  4, -3,  6, 0,
          0,  8, -4,  7,  4,  4,  7, -4,  8, 0,
          0, -8,-24, -4, -3, -3, -4,-24, -8, 0,
          0, 99, -8,  8,  6,  6,  8, -8, 99, INFIN
    };

    static int value2[79]=
    {
         99, -8,  8,  6,  6,  8, -8, 99,  0,
          0, -8,-24,  0,  1,  1,  0,-24, -8, 0,
          0,  8,  0,  7,  4,  4,  7,  0,  8, 0,
          0,  6,  1,  4,  1,  1,  4,  1,  6, 0,
          0,  6,  1,  4,  1,  1,  4,  1,  6, 0,
          0,  8,  0,  7,  4,  4,  7,  0,  8, 0,
          0, -8,-24,  0,  1,  1,  1,-24, -8, 0,
          0, 99, -8,  8,  6,  6,  8, -8, 99, INFIN
    };

    if (board[11] != EMPTY || board[18] != EMPTY || board[81] != EMPTY || board[88] != EMPTY)
    {
        valuep = value2;

        if (board[11] == EMPTY)
            value2[12-11] = -8, value2[21-11] = -8, value2[22-11] = -24;
        else
            value2[12-11] = 12, value2[21-11] = 12, value2[22-11] = 8;

        if (board[18] == EMPTY)
            value2[17-11] = -8, value2[28-11] = -8, value2[27-11] = -24;
        else
            value2[17-11] = 12, value2[28-11] = 12, value2[27-11] = 8;

        if (board[81] == EMPTY)
            value2[82-11] = -8, value2[71-11] = -8, value2[72-11] = -24;
        else
            value2[82-11] = 12, value2[71-11] = 12, value2[72-11] = 8;

        if (board[88] == EMPTY)
            value2[87-11] = -8, value2[78-11] = -8, value2[77-11] = -24;
        else
            value2[87-11] = 12, value2[78-11] = 12, value2[77-11] = 8;
    }
    else
    {
        valuep = value;
    }

    boardp = &board[11];
    while ((v = *valuep++) != INFIN)
    {
        BYTE bv = *boardp++;
        if (bv == friend)
        {
            fpoints += v;
        }
        else if (bv == enemy)
        {
            epoints += v;
            ecount++;
        }
    }

    if (!ecount)     /* any enemy pieces on the board? */
        return WIN;  /* if not, we just won! */
    else
        return fpoints - epoints;
}

/****************************************************************************
 *                                                                          *
 * Function: FinalScore                                                     *
 *                                                                          *
 * Purpose : Calculates the final score.                                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static int FinalScore(BYTE board[], BYTE friend, BYTE enemy)
{
    int count = 0;
    int i;

    for (i = 11; i <= 88; i++)
    {
        if (board[i] == friend)
            count++;
        else if (board[i] == enemy)
            count--;
    }

    if (count > 0)
        return WIN + count;
    else if (count < 0)
        return LOSS + count;
    else
        return 0;
}

/****************************************************************************
 *                                                                          *
 * Function: Message                                                        *
 *                                                                          *
 * Purpose : Displays a message at the top of the window (flashing).        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Message(HWND hwnd, PCTSTR pszStr, ...)
{
    static TCHAR szBuffer[80];
    va_list marker;

    va_start(marker, pszStr);
    wvsprintf(szBuffer, pszStr, marker);
    va_end(marker);

    SetMessage(hwnd, szBuffer);

    g_cFlashTimes = 0;
    g_cMaxFlashTimes = 8;
    SetTimer(hwnd, TID_FLASH, 200, InvertMessage);
}

/****************************************************************************
 *                                                                          *
 * Function: SetMessage                                                     *
 *                                                                          *
 * Purpose : Makes sure the message is displayed.                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void SetMessage(HWND hwnd, PCTSTR pszMess)
{
    g_pszMess = pszMess; g_cMaxFlashTimes = 0;
    InvalidateRect(hwnd, &g_rcMess, TRUE);
    UpdateWindow(hwnd);
}

/****************************************************************************
 *                                                                          *
 * Function: InvertMessage                                                  *
 *                                                                          *
 * Purpose : Timer callback for message flashing.                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void CALLBACK InvertMessage(HWND hwnd, UINT msg, UINT id, DWORD dwTime)
{
    if (++g_cFlashTimes <= g_cMaxFlashTimes)
    {
        HDC hdc = GetDC(hwnd);
        if (hdc)
        {
            PatBlt(hdc, g_rcMess.left, g_rcMess.top, g_rcMess.right -
                g_rcMess.left, g_rcMess.bottom - g_rcMess.top, DSTINVERT);
            ReleaseDC(hwnd, hdc);
        }
    }
    else
    {
        /* Done flashing */
        KillTimer(hwnd, TID_FLASH);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: AboutDlgProc                                                   *
 *                                                                          *
 * Purpose : Handles messages for the About dialog.                         *
 *                                                                          *
 * History : Date      Reason                                               *
 *           01-02-19  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL CALLBACK AboutDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    if (msg == WM_INITDIALOG)
        return TRUE;

    if (msg == WM_COMMAND)
    {
        EndDialog(hwndDlg, TRUE);
        return TRUE;
    }

    return FALSE;
}

/****************************************************************************
 *                                                                          *
 * Function: IsPocketPC2002                                                 *
 *                                                                          *
 * Purpose : Return true if we are running on Pocket PC 2002.               *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-02-17  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL IsPocketPC2002(void)
{
    TCHAR szPlatform[32];
    OSVERSIONINFO osver;

    GetVersionEx(&osver);
    if (osver.dwMajorVersion == 3 && SystemParametersInfo(SPI_GETPLATFORMTYPE,
        sizeof(szPlatform), szPlatform, 0) != 0 && lstrcmp(szPlatform, L"PocketPC") == 0)
        return TRUE;

    return FALSE;
}

