#ifndef _DDUTIL_H_
#define _DDUTIL_H_

extern IDirectDrawPalette *DDLoadPalette(IDirectDraw7 *, LPCSTR);
extern IDirectDrawSurface7 *DDLoadBitmap(IDirectDraw7 *, LPCSTR, int, int);
extern HRESULT DDReLoadBitmap(IDirectDrawSurface7 *, LPCSTR);
extern HRESULT DDCopyBitmap(IDirectDrawSurface7 *, HBITMAP, int, int, int, int);
extern DWORD DDColorMatch(IDirectDrawSurface7 *, COLORREF);
extern HRESULT DDSetColorKey(IDirectDrawSurface7 *, COLORREF);

#endif /* _DDUTIL_H_ */
