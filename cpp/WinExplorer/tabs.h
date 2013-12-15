HWND CreatePropsGeneral(HINSTANCE hInst, HWND hParent);
HWND CreatePropsPosition(HINSTANCE hInst, HWND hParent);
HWND CreatePropsClass(HINSTANCE hInst, HWND hParent);
HWND CreatePropsStyle(HINSTANCE hInst, HWND hParent);
HWND CreatePropsExStyle(HINSTANCE hInst, HWND hParent);
HWND CreatePropsMore(HINSTANCE hInst, HWND hParent);

void PropsToWinGeneral(HWND hWnd);
void WinToPropsGeneral(HWND hWnd);
void WinToPropsPosition(HWND hWnd);
void PropsToWinPosition(HWND hWnd);
void WinToPropsClass(HWND hWnd);
HWND CreatePropsFont(HINSTANCE hInst, HWND hParent);
void WinToPropsFont(HWND hWnd);
void WinToPropsStyle(HWND hWnd);
void WinToPropsExStyle(HWND hWnd);
void WinToPropsMore(HWND hWnd);

void DestroyWin(HWND hwnd);


#define NUM_OF_CHARSET 18
#define BASE_WIN_STYLE 5000
#define BASE_WIN_SPECIFIC_STYLE BASE_WIN_STYLE + 16
#define BASE_WIN_EXSTYLE 6000