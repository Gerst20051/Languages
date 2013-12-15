#ifndef __bcxfn_h__
#define __bcxfn_h__

void    BCX_Set_Scale(HWND hwnd);
void    Center (HWND hwnd, HWND Xhwnd, HWND Yhwnd);
HBITMAP BCX_LoadBMP (char *, int=0);
HOOKPROC CALLBACK SBProc (UINT, WPARAM, LPARAM);
char*   GetFileName (char*,char*,int=0,HWND=0,DWORD=0,char* =0);
char    *ucase (char *S);
char    *curdir (void);
int     instr(char* mane,char* match,int offset,int sensflag);
char    *_stristr_(char *String, char *Pattern);
BOOL    FindFirstInstance (char *Application);
HWND    BCX_Label(char*,HWND,int=0,int=0,int=0,int=0,int=0,int=0,int=0);
HWND    BCX_Group(char*,HWND,int,int,int,int,int,int=0,int=0);
HFONT   BCX_Set_Font (char *,int,int=0,int=0,int=0,int=0);
HBITMAP BCX_LoadImage (char *, int=0);

#endif
