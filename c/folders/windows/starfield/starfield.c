// -GDI Starfield-
// Illustrates the basics of Windows programming
// Written by J. Farrell

#define WIN32_LEAN_AND_MEAN // the coolest line of code ever ^_^

// INCLUDES //////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

// DEFINES ///////////////////////////////////////
#define WIN_CLASS_NAME "MyClass" // window class name
#define KEYSTATE(n) ((GetAsyncKeyState(n) & 0x8000) ? TRUE: FALSE) // that macro I told you about

// GLOBALS ///////////////////////////////////////
HDC hMainDC;
HINSTANCE hInstance; // Windows stuff
HWND hWnd;

float stars[300][5]; // starfield - I made this out of floats simply
                     // because of all the divisions, to keep things a
                     // little more accurate and avoid a horde of casts.
                     //
                     // Columns 0, 1, and 2, are 3D coordinates (x, y, z),
                     // and columns 3 and 4 are screen coordinates.

int nWidth = 400, nHeight = 300; // current window size

// PROTOTYPES ////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
void DeleteStars(void);
void DrawStar(int x, int y, int z, int color);
void InitStars(void);
void MoveStars(void);
void PlotStars(void);

//////////////////////////////////////////////////////////////////////
// WINDOWPROC
// This is the message handler for the window class. Nothin' fancy,
// just the basics.
//////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg,
							WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;  // used to paint the window
	HDC hdc;         // device context handle

	switch(msg)
	{
	case WM_CREATE:  // any initialization functions could go here
		return(0);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);  // validate window - just let BeginPaint use the brush
		EndPaint(hwnd, &ps);          // release DC
		return(0);
		break;

	case WM_DESTROY: // any shutdown functions could go here
		PostQuitMessage(0);    
		return(0);
		break;

	case WM_SIZE:
		// extract new window size
		nWidth = LOWORD(lparam);
		nHeight = HIWORD(lparam);
		return(0);
		break;

	case WM_CLOSE:
		// confirm quit
		if (MessageBox(hwnd, "Are you sure you want to quit?",
			"Quit Starfield", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
			return(DefWindowProc(hwnd, msg, wparam, lparam));
		else
			return(0);
		break;
	}

	// call the default message handler for anything else
	return(DefWindowProc(hwnd, msg, wparam, lparam));
}

//////////////////////////////////////////////////////////////////////
// WINMAIN
// Program entry point. Defines a window class, creates a window, sets
// the stars' initial positions, and runs the main loop.
//////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hinstance,     // app instance
				   HINSTANCE hprevinstance, // previous app instance
				   LPSTR lpcmdline,         // command line string
				   int ncmdshow)            // display parameter
{
	WNDCLASSEX winclass; // our window class
	HWND hwnd;           // handle to our window
	MSG msg;             // a message variable
	DWORD nStartTime;

	winclass.cbSize =        sizeof(WNDCLASSEX);
	winclass.style =         CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc =   WindowProc;     // message handler
	winclass.cbClsExtra =    0;              // extra class info
	winclass.cbWndExtra =    0;              // extra window info
	winclass.hInstance =     hinstance;	     // application handle
	winclass.hIcon =         LoadIcon(NULL, IDI_WINLOGO);
	winclass.hCursor =       LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName =  NULL;           // no menu, thanks
	winclass.lpszClassName = WIN_CLASS_NAME; // identifier
	winclass.hIconSm =       LoadIcon(NULL, IDI_WINLOGO); // small icon
	
	// register class
	if (!RegisterClassEx(&winclass))
		return(0);
	
	// create a window
	if (!(hwnd = CreateWindowEx(0,            // extended style
			     WIN_CLASS_NAME,                 // class identifier
				 "Learning Windows GDI - Starfield", // window title
				 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				                                 // parameters
				 CW_USEDEFAULT, CW_USEDEFAULT,   // initial position (I think I forgot to tell you that you can do this, heh, sorry)
				 nWidth, nHeight,                // initial size
				 NULL,                           // handle to parent (desktop)
				 NULL,                           // handle to menu (none)
				 hinstance,                      // app handle
				 NULL)))                         // extra parameters
		return(0);
	
	// set globals
	hWnd = hwnd;
	hInstance = hinstance;
	hMainDC = GetDC(hWnd);

	// initialize starfield
	InitStars();

	// main loop - inifinite (can only be exited by encountering WM_QUIT)
	while(TRUE)
	{
		// check the message queue
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT) // exit loop on quit
				break;

			// send the message on its merry way
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// check the time - this is another thing I should have told you
		// about! GetTickCount() returns the number of milliseconds since
		// the system was started. It's a good way to lock your display
		// to a certain frame rate, although higher-resolution timers are
		// available.
		nStartTime = GetTickCount();

		// update the display - no double-buffering here because I'm
		// keeping things simple, and because the simplicity of this
		// program means it runs pretty fast, so you probably won't
		// even notice the flicker
		DeleteStars();
		MoveStars();
		PlotStars();

		// lock to 30 FPS - 33ms per frame
		while ((GetTickCount() - nStartTime) < 33);

		// post a WM_CLOSE message if escape key is being pressed
		if (KEYSTATE(VK_ESCAPE))
			PostMessage(hwnd, WM_CLOSE, 0, 0);
	}

	// release the device context
	ReleaseDC(hWnd, hMainDC);

	// return to Windows - this is the exit code of the WM_QUIT message
	return(msg.wParam);
}

//////////////////////////////////////////////////////////////////////
// DeleteStars()
// Simply runs through the array of stars and plots them all in black.
//////////////////////////////////////////////////////////////////////
void DeleteStars(void)
{
	int a;
	for (a=0; a<300; a++)
		DrawStar((int)stars[a][3], (int)stars[a][4], (int)stars[a][2], 0);
}

//////////////////////////////////////////////////////////////////////
// DrawStar()
// Draws a star at the specified coordinates using the specified color.
//////////////////////////////////////////////////////////////////////
void DrawStar(int x, int y, int z, int color)
{
	int a, b, max;

	// size based on distance from camera
	max = 3 - (int)(z/33);

	// draw one pixel at a time
	for (a=0; a<max; a++)
		for (b=0; b<max; b++)
			SetPixel(hMainDC, x+a, y+b, RGB(color, color, color));
}

//////////////////////////////////////////////////////////////////////
// InitStars()
// Assigns all the stars random locations.
//////////////////////////////////////////////////////////////////////
void InitStars(void)
{
	int a;
	// seed random number generator
	srand(GetTickCount());

	// choose random coordinates for each star
	for (a=0; a<300; a++)
	{
		stars[a][0] = (float)(rand()%70 - 35);
		stars[a][1] = (float)(rand()%70 - 35);
		stars[a][2] = (float)(rand()%100 + 1);
	}
}

//////////////////////////////////////////////////////////////////////
// MoveStars()
// Decreases every star's z-coordinate by 3, bringing them closer. If a
// star's z-coordinate becomes zero, it has reached the "camera" and so
// is re-initialized with a z-coordinate of 100 greater than its current
// z-coordinate.
//////////////////////////////////////////////////////////////////////
void MoveStars(void)
{
	int a;
	for (a=0; a<300; a++)
	{
		stars[a][2] -= 3;
		if (stars[a][2] <= 0)
		{
			// re-initialize if star is too close
			stars[a][0] = (float)(rand()%70 - 35);
			stars[a][1] = (float)(rand()%70 - 35);
			stars[a][2] += 100;
		}

		// perspective transformation - this is interesting! Note the
		// transformation from 3D coordinates to 2D:
		//
		// x2D = x3D / z3D * (screen width) + (half of screen width)
		// y2D = y3D / z3D * (screen height) + (half of screen height)
		//
		// Pretty neat, hey?

		// Note also that since I'm using the variable screen dimensions,
		// the starfield changes size automatically if you resize the
		// window. 
		stars[a][3] = (stars[a][0] / stars[a][2]) * nWidth + (nWidth>>1);
		stars[a][4] = (stars[a][1] / stars[a][2]) * nHeight + (nHeight>>1);
	}
}

//////////////////////////////////////////////////////////////////////
// PlotStars()
// Plots all the stars with colors based on their distances from the
// user's point of view.
//////////////////////////////////////////////////////////////////////
void PlotStars(void)
{
	int a, col;
	for (a=0; a<300; a++)
	{
		// assign color based on z-coordinate and draw the star
		col = 255 - (int)(stars[a][2]*2);
		DrawStar((int)stars[a][3], (int)stars[a][4], (int)stars[a][2], col);
	}
}

