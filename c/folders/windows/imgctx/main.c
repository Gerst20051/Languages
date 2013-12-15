/*
 * Example of using IImgCtx interface from MSHTML.DLL
 */

#include <windows.h>
#define COBJMACROS
#include "iimgctx.h"

//--------------------------------------------------------------------------- 

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	static IImgCtx *pImage = NULL;

	switch (msg) 
	{ 
		case WM_CREATE:
		{
			// Create an IImgCtx object.
			HRESULT hr = CoCreateInstance(&CLSID_IImgCtx, NULL, CLSCTX_ALL, &IID_IImgCtx, (void**)&pImage);
			if (SUCCEEDED(hr))
			{
				// Second argument to IImgCtx_Load is an URL.
#if 0
				// Example of a Web URL - TODO: Change the web address !!
				hr = IImgCtx_Load(pImage, L"http://www.smorgasbordet.com/pellesc/images/screen1.png", 0);
#else
				// Example of a local file URL - TODO: Change the filename !!
				hr = IImgCtx_Load(pImage, L"file://c:./parrot.png", 0);
#endif
			}
			// No need to continue if it failed...
			return SUCCEEDED(hr) ? 0 : -1;
		}

		case WM_ERASEBKGND:
			if (pImage)
			{
				// Check download state.
				ULONG ulState = 0;
				HRESULT hr = IImgCtx_GetStateInfo(pImage, &ulState, NULL, TRUE);
				// Don't erase background if we got the image - this will reduce flicker.
				if (SUCCEEDED(hr) && (ulState & IMGLOAD_COMPLETE) != 0)
					return 1;
			}
			// Do the default thing...
			return DefWindowProc(hwnd, msg, wParam, lParam);

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps); 
			if (pImage)
			{
				// Check download state.
				ULONG ulState = 0;
				HRESULT hr = IImgCtx_GetStateInfo(pImage, &ulState, NULL, TRUE);
				if (SUCCEEDED(hr))
				{
					RECT rcBounds;
					GetClientRect(hwnd, &rcBounds);

					if (ulState & IMGLOAD_LOADING)
					{
						// Still loading - wait 50 msec and request again.
						DrawText(ps.hdc, "Loading, please wait...", -1, &rcBounds, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
						Sleep(50);
						InvalidateRect(hwnd, NULL, TRUE);
					}
					else if (ulState & IMGLOAD_COMPLETE)
					{
						// Download successfully complete.
						hr = IImgCtx_Draw(pImage, ps.hdc, &rcBounds);
					}
					else
					{
						// Download failed.
						DrawText(ps.hdc, "Failure loading image!", -1, &rcBounds, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
					}
				}
			}
			EndPaint(hwnd, &ps);
			return 0;
		}

		case WM_DESTROY:
			if (pImage)
			{
				// Clean up this COM mess...
				IImgCtx_Release(pImage);
				pImage = NULL;
			}
			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

//--------------------------------------------------------------------------- 

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR szCmdLine, int nCmdShow) 
{ 
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wc;

	// Initialize the COM library.
	CoInitialize(NULL);

	// Register the main window class.
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW|CS_VREDRAW; 
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "ImageTest";
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	// Create the main window - here we will start loading the image.
	hwnd = CreateWindow("ImageTest", "IImgCtx interface Sample", WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hInst, NULL);

	// Display the main window.
	ShowWindow(hwnd, nCmdShow); 
	UpdateWindow(hwnd); 

	// Message pump.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Free the COM library.
	CoUninitialize();

	// We are *so* done...
	return msg.wParam;
}
