void SelectFirstItemInTree();
void TreeSelChanged();

void FillWinTree();
HTREEITEM AddWinTreeItem(HWND hWnd, HTREEITEM hParent);
void GetTreeItemText(HWND hWnd, LPSTR szBuf);
LRESULT SendTreeMsg(UINT  Msg,WPARAM  wParam, LPARAM  lParam );
LRESULT SendTabMsg(UINT  Msg,WPARAM  wParam, LPARAM  lParam );

void ShowWinProps();
HWND GetSelectedWndInTree();
LRESULT SendMainDlgMessage(int nIDDlgItem, UINT  Msg,WPARAM  wParam, LPARAM  lParam);
void UpdateWinProps();
void AdjustTabWindow(HWND hSubWin);
void ShowSubWin(int nIndex);
void InsertTab(LPSTR lpszText, int iImage, LPARAM lParam);
void InsertTabs();
void RefreshWinTree();
BOOL SelectWndInTree(HWND hWnd, HTREEITEM hItem = TVI_ROOT);
BOOL IsClass(LPCSTR lpszClass, LPCSTR lpszStart, LPCSTR lpszEnd);
void RefreshCurrTreeItem();
HWND GetWndInTreeItem(HTREEITEM hItem);
void SetSubWin(HWND hWnd, int nIndex);
HTREEITEM FindNextWindow(LPSTR lpszWinCaption, DWORD dwFlags);
void OpenFindReplace(HWND hwndDlg);
void CloseFindData();
void SetAutoRefreshTimer(HWND hwndDlg);

#define MAX_TREE_ITEM_TEXT 1024
#define NUM_OF_SUBWINS 7
#define MAX_FIND_BUFFER 255

#define FIND_ANY_PART 8
#define FIND_IN_WINTEXT 1
#define FIND_IN_CLASSNAME 2
#define FIND_IN_ITEMID 3
#define FIND_IN_ALL 7
#define MAX_APP_ICONS 255
#define REFRESH_TIMER_ID 1
