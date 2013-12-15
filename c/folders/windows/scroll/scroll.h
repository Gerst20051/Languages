#include<windows.h>
int HandleScrollMessages(HWND hwnd,UINT msg, WPARAM wParam, LPARAM lParam);
void SetScrolls(HWND hwnd, RECT* lpRect, POINT *lpPoint);
void GetScrolls(HWND hwnd, RECT* lpRect, POINT *lpPoint);
