/****************************************************************************
 *                                                                          *
 * Filename: spincube.c                                                     *
 *                                                                          *
 * Purpose : OpenGL sample for Pelles C for Windows.                        *
 *                                                                          *
 *           Original source code from: nehe.gamedev.net                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glaux.lib")
#pragma comment(lib, "glu32.lib")

HDC hDC = NULL;                 // Private GDI Device Context
HGLRC hRC = NULL;               // Permanent Rendering Context
HWND hWnd = NULL;               // Window handle
HINSTANCE hInstance;            // Instance handle

BOOL g_fKeys[256];              // Array Used For The Keyboard Routine
BOOL g_fActive = TRUE;          // Window Active Flag Set To TRUE By Default
BOOL g_fFullScreen = TRUE;      // Fullscreen Flag Set To Fullscreen Mode By Default

GLfloat xrot;                   // X Rotation
GLfloat yrot;                   // Y Rotation
GLfloat xspeed = 0.1f;          // X Rotation Speed
GLfloat yspeed = 2.0f;          // Y Rotation Speed
GLfloat z = -7.0f;              // Depth Into The Screen

GLfloat LightAmbient[] =  { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };

GLuint filter = 1;              // Which Filter To Use
GLuint texture[3];              // Storage For 3 Textures

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/****************************************************************************
 *                                                                          *
 * Function: LoadBMP                                                        *
 *                                                                          *
 * Purpose : Loads A Bitmap Image.                                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static AUX_RGBImageRec *LoadBMP(char *Filename)
{
    if (Filename)
    {
        FILE *fp = fopen(Filename, "r");
        if (fp)
        {
            fclose(fp);
            return auxDIBImageLoad(Filename);
        }
    }

    return NULL;
}

/****************************************************************************
 *                                                                          *
 * Function: LoadGLTextures                                                 *
 *                                                                          *
 * Purpose : Load Bitmaps And Convert To Textures.                          *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL LoadGLTextures(void)
{
    BOOL fStatus = FALSE;

    AUX_RGBImageRec *TextureImage[1];

    memset(TextureImage, 0, sizeof(void *)*1);

    // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
    if (TextureImage[0] = LoadBMP("logo.bmp"))
    {
        fStatus = TRUE;

        // Create Three Textures
        glGenTextures(3, &texture[0]);

        // Create Nearest Filtered Texture
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        // Create Linear Filtered Texture
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        // Create MipMapped Texture
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
    }

    if (TextureImage[0])
    {
        if (TextureImage[0]->data)
            free(TextureImage[0]->data);

        free(TextureImage[0]);
    }

    return fStatus;
}

/****************************************************************************
 *                                                                          *
 * Function: ReSizeGLScene                                                  *
 *                                                                          *
 * Purpose : Resize And Initialize The GL Window.                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
    // Prevent A Divide By Zero
    if (height == 0) height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/****************************************************************************
 *                                                                          *
 * Function: InitGL                                                         *
 *                                                                          *
 * Purpose : All Setup For OpenGL Goes Here.                                *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static int InitGL(GLvoid)
{
    if (!LoadGLTextures())
        return FALSE;

    glEnable(GL_TEXTURE_2D);                                // Enable Texture Mapping
    glShadeModel(GL_SMOOTH);                                // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                   // Black Background
    glClearDepth(1.0f);                                     // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                                // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                 // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);      // Really Nice Perspective Calculations

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);         // Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);         // Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);        // Position The Light
    glEnable(GL_LIGHT1);                                    // Enable Light One

    glColor4f(1.0f, 1.0f, 1.0f, 0.5);                       // Full Brightness.  50% Alpha
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);                       // Set The Blending Function For Translucency

    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: DrawGLScene                                                    *
 *                                                                          *
 * Purpose : Here's Where We Do All The Drawing.                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static int DrawGLScene(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, z);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, texture[filter]);

    glBegin(GL_QUADS);
    // Front Face
    glNormal3f( 0.0f, 0.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glNormal3f( 0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    // Top Face
    glNormal3f( 0.0f, 1.0f,  0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Face
    glNormal3f( 0.0f,-1.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    // Right face
    glNormal3f( 1.0f, 0.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glNormal3f(-1.0f, 0.0f,  0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();

    xrot += xspeed;
    yrot += yspeed;
    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: KillGLWindow                                                   *
 *                                                                          *
 * Purpose : Properly Kill The Window.                                      *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static GLvoid KillGLWindow(GLvoid)
{
    if (g_fFullScreen)
    {
        ChangeDisplaySettings(NULL, 0);
        ShowCursor(TRUE);
    }

    if (hRC)
    {
        if (!wglMakeCurrent(NULL, NULL))
            MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK|MB_ICONINFORMATION);

        if (!wglDeleteContext(hRC))
            MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK|MB_ICONINFORMATION);

        hRC = NULL;
    }

    if (hDC && !ReleaseDC(hWnd,hDC))
    {
        MessageBox(NULL, "Release Device Context Failed.","SHUTDOWN ERROR", MB_OK|MB_ICONINFORMATION);
        hDC = NULL;
    }

    if (hWnd && !DestroyWindow(hWnd))
    {
        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK|MB_ICONINFORMATION);
        hWnd = NULL;
    }

    if (!UnregisterClass("OpenGL", hInstance))
    {
        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK|MB_ICONINFORMATION);
        hInstance = NULL;
    }
}

/****************************************************************************
 *                                                                          *
 * Function: CreateGLWindow                                                 *
 *                                                                          *
 * Purpose : This Code Creates Our OpenGL Window.                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL CreateGLWindow(char *title, int width, int height, int bits, BOOL fullscreenflag)
{
    static PIXELFORMATDESCRIPTOR pfd = {0};
    GLuint PixelFormat;
    WNDCLASS wc;
    DWORD dwExStyle;
    DWORD dwStyle;
    RECT rcWindow;

    SetRect(&rcWindow, 0, 0, width, height);

    g_fFullScreen = fullscreenflag;

    hInstance = GetModuleHandle(NULL);

    wc.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "OpenGL";

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL,"Failed To Register The Window Class.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (g_fFullScreen)
    {
        DEVMODE dmScreenSettings;

        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = width;
        dmScreenSettings.dmPelsHeight = height;
        dmScreenSettings.dmBitsPerPel = bits;
        dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
            if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
            {
                g_fFullScreen = FALSE;
            }
            else
            {
                // Pop Up A Message Box Letting User Know The Program Is Closing.
                MessageBox(NULL,"Program Will Now Close.", "ERROR", MB_OK|MB_ICONSTOP);
                return FALSE;
            }
        }
    }

    if (g_fFullScreen)
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
        ShowCursor(FALSE);
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx(&rcWindow, dwStyle, FALSE, dwExStyle);

    // Create The Window
    if (!(hWnd = CreateWindowEx(dwExStyle, "OpenGL", title, dwStyle|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
        0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom -rcWindow.top, NULL, NULL, hInstance, NULL)))
    {
        KillGLWindow();
        MessageBox(NULL,"Window Creation Error.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }


    if (!(hDC = GetDC(hWnd)))
    {
        KillGLWindow();
        MessageBox(NULL, "Can't Create A GL Device Context.","ERROR", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = bits;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
    {
        KillGLWindow();
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (!SetPixelFormat(hDC, PixelFormat, &pfd))
    {
        KillGLWindow();
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (!(hRC = wglCreateContext(hDC)))
    {
        KillGLWindow();
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (!wglMakeCurrent(hDC, hRC))
    {
        KillGLWindow();
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    ReSizeGLScene(width, height);

    // Initialize Our Newly Created GL Window
    if (!InitGL())
    {
        KillGLWindow();
        MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: WndProc                                                        *
 *                                                                          *
 * Purpose : Window callback procedure.                                     *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_ACTIVATE:
        {
            g_fActive = !HIWORD(wParam);
            return 0;
        }

        case WM_SYSCOMMAND:
        {
            switch (wParam)
            {
                    case SC_SCREENSAVE:         // Screensaver Trying To Start?
                    case SC_MONITORPOWER:       // Monitor Trying To Enter Powersave?
                    return 0;                   // Prevent From Happening
            }
            break;
        }

        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_KEYDOWN:
        {
            g_fKeys[wParam] = TRUE;
            return 0;
        }

        case WM_KEYUP:
        {
            g_fKeys[wParam] = FALSE;
            return 0;
        }

        case WM_SIZE:
        {
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
            return 0;
        }
    }

    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Here is were the show starts.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    BOOL fDone = FALSE;
    MSG msg;

    // Ask The User Which Screen Mode They Prefer
    if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO|MB_ICONQUESTION) == IDNO)
        g_fFullScreen = FALSE;

    // Create Our OpenGL Window
    if (!CreateGLWindow("OpenGL sample for Pelles C for Windows", 640, 480, 16, g_fFullScreen))
        return 0;

    g_fKeys['B'] = TRUE;
    g_fKeys['L'] = TRUE;

    while (!fDone)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                fDone = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
            if ((g_fActive && !DrawGLScene()) || g_fKeys[VK_ESCAPE])
            {
                fDone = TRUE;
            }
            else
            {
                static BOOL fLKeyPressed = FALSE;  // L Pressed?
                static BOOL fFKeyPressed = FALSE;  // F Pressed?
                static BOOL fBKeyPressed = FALSE;  // B Pressed?

                SwapBuffers(hDC);  // Swap Buffers (Double Buffering)

                if (g_fKeys['L'] && !fLKeyPressed)
                {
                    static BOOL fLighting = FALSE;

                    fLKeyPressed = TRUE;

                    fLighting = !fLighting;
                    if (!fLighting)
                        glDisable(GL_LIGHTING);
                    else
                        glEnable(GL_LIGHTING);
                }
                if (!g_fKeys['L'])
                    fLKeyPressed = FALSE;

                if (g_fKeys['B'] && !fBKeyPressed)
                {
                    static BOOL fBlending = FALSE;

                    fBKeyPressed = TRUE;

                    fBlending = !fBlending;
                    if (fBlending)
                    {
                        glEnable(GL_BLEND);             // Turn Blending On
                        glDisable(GL_DEPTH_TEST);       // Turn Depth Testing Off
                    }
                    else
                    {
                        glDisable(GL_BLEND);            // Turn Blending Off
                        glEnable(GL_DEPTH_TEST);        // Turn Depth Testing On
                    }
                }
                if (!g_fKeys['B'])
                    fBKeyPressed = FALSE;

                if (g_fKeys['F'] && !fFKeyPressed)
                {
                    fFKeyPressed = TRUE;

                    if (++filter > 2)
                        filter = 0;
                }
                if (!g_fKeys['F'])
                    fFKeyPressed = FALSE;

                if (g_fKeys[VK_PRIOR])
                    z -= 0.02f;

                if (g_fKeys[VK_NEXT])
                    z += 0.02f;

                if (g_fKeys[VK_UP])
                    xspeed -= 0.01f;

                if (g_fKeys[VK_DOWN])
                    xspeed += 0.01f;

                if (g_fKeys[VK_RIGHT])
                    yspeed += 0.01f;

                if (g_fKeys[VK_LEFT])
                    yspeed -= 0.01f;
            }
        }
    }

    // Shutdown
    KillGLWindow();
    return msg.wParam;
}
