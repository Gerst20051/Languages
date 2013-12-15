// VazBreak a BreakOut game by PhilVaz
// Full Screen GDI game
// Apr 17, 2003

// converted to Pelles C
// Nov 21, 2004 by tiwag

// INCLUDES ///////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <mmsystem.h>  // for PlaySound (along with winmm.lib)
#include <stdlib.h>    // for rand functions
#include <stdio.h>     
#include <math.h>

#include "VazBreak.h"  // sounds and icon resources

#define bool BOOL

// DEFINES ////////////////////////////////////////////////

// defines for windows

#define WINDOW_CLASS_NAME "WINCLASS1"

#define WINDOW_WIDTH   800   // size of game window
#define WINDOW_HEIGHT  600

#define GAME_SPEED      20   // speed of game (increase to go slower)

#define GAME_STATE_DEMO_INIT   0   // demo initialization state
#define GAME_STATE_DEMO_RUN    1   // demo running state
#define GAME_STATE_GAME_INIT   2   // game initialization state
#define GAME_STATE_GAME_RUN    3   // game running state
#define GAME_STATE_GAME_RESET  4   // game reset state (new level)
#define GAME_STATE_GAME_PAUSE  5   // game pause state (no movement)

// MACROS /////////////////////////////////////////////////

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// GLOBALS ////////////////////////////////////////////////

HWND game_window = NULL;           // global game window handle
HINSTANCE game_instance = NULL;    // global game instance handle
HDC game_dc = NULL;                // global device context (for GDI) handle

DEVMODE game_screen;               // global for full screen mode

// global pen and brush
HPEN red_pen = NULL;
HBRUSH red_brush = NULL;
HPEN green_pen = NULL;
HBRUSH green_brush = NULL;
HPEN blue_pen = NULL;
HBRUSH blue_brush = NULL;
HPEN white_pen = NULL;
HBRUSH white_brush = NULL;
HPEN black_pen = NULL;
HBRUSH black_brush = NULL;

// global game variables

int xball, yball, xmove, ymove; // ball position and speed

int xpaddle;       // x paddle position changes
int ypaddle = 560; // y paddle position is constant

int xmouse, xoldmouse; // current x and old x position of mouse

int xblock[150];       // blocks x top-left rect position
int yblock[150];       // blocks y top-left rect position
bool block_exist[150]; // 1 (TRUE) if exists (not hit), 0 (FALSE) if gone (hit)

int game_state = GAME_STATE_DEMO_INIT;  // start in demo state

char text[80]; // for display text output

int game_score = 0;   // set score to zero
int game_level = 1;   // set level to one
int blocks_hit = 0;   // set total blocks hit to zero
int paddles_left = 0; // at Game Over number of paddles is zero

// FUNCTIONS //////////////////////////////////////////////

void GameInit(void);      // game initialization, go full screen etc
void GameMain(void);      // game main loop and processing
void GameQuit(void);      // game quit and clean up

void DrawBlocks(void);    // clears screen, draws border and blocks
void DisplayScore(void);  // display text score/level/paddles bottom of screen

void SetBall(int);   // set ball initial position and speed (+3 slow, +9 fast)
void SetPaddle(void);    // set paddle in middle of screen
void MoveBall(void);     // moves ball once
void MovePaddle(void);   // moves paddle once (checks for arrow keys left/right)



// WINPROC ////////////////////////////////////////////////

LRESULT CALLBACK WinProc(HWND hwnd, 
                         UINT msg, 
                         WPARAM wparam, 
                         LPARAM lparam)
{  // this is the main message handler of the system

   HDC         hdc; // handle to a device context
   PAINTSTRUCT ps;  // used in WM_PAINT

   switch(msg) // what is the message
   {    
      case WM_CREATE: 
      {
         // do initialization stuff here

         return(0); // return success
      } break;

      case WM_MOUSEMOVE:
      {
         xoldmouse = xmouse;
         xmouse = (int)LOWORD(lparam);
      }

      case WM_PAINT: 
      {
         hdc = BeginPaint(hwnd, &ps); // validate the window
                  
         EndPaint(hwnd, &ps);

         return(0); // return success
      }  break;

      case WM_DESTROY: 
      {
         PostQuitMessage(0); // kill the application, sends a WM_QUIT message 

         return(0); // return success
      }  break;

      default:break;

   } // end switch

// process any default messages 

return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hinstance,
                   HINSTANCE hprevinstance,
                   LPSTR lpcmdline,
                   int ncmdshow)
{

   WNDCLASSEX winclass; // this will hold the class we create
   HWND hwnd;           // generic window handle
   MSG msg;             // generic message

   // save the game instance handle
   game_instance = hinstance;

   // first fill in the window class structure

   winclass.cbSize        = sizeof(WNDCLASSEX);
   winclass.style         = CS_DBLCLKS | CS_OWNDC | 
                            CS_HREDRAW | CS_VREDRAW;
   winclass.lpfnWndProc   = WinProc;
   winclass.cbClsExtra    = 0;
   winclass.cbWndExtra    = 0;
   winclass.hInstance     = hinstance;
   winclass.hIcon         = LoadIcon(game_instance, MAKEINTRESOURCE(ICON_VAZGAMES));
   winclass.hIconSm       = LoadIcon(game_instance, MAKEINTRESOURCE(ICON_VAZGAMES));   
   winclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
   winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
   winclass.lpszMenuName  = NULL;
   winclass.lpszClassName = WINDOW_CLASS_NAME;

   // register the window class
   if (!RegisterClassEx(&winclass)) return(0);

   // create the window
   if (!(hwnd = CreateWindowEx(0,                   // extended style
                               WINDOW_CLASS_NAME,      // class
                               "VazBreak",             // title
                               WS_POPUP | WS_VISIBLE,  // use POPUP for full screen
                               0,0,           // initial game window x,y
                               WINDOW_WIDTH,  // initial game width
                               WINDOW_HEIGHT, // initial game height
                               NULL,          // handle to parent 
                               NULL,          // handle to menu
                               hinstance,     // instance of this application
                               NULL)))        // extra creation parms
   return(0);

   // save the game window handle
   game_window = hwnd;    

   GameInit();   // game initialization function called here

   __try
   {
      // enter main event loop using PeekMessage() to retrieve messages
      while(TRUE)
      {

         // get initial tick count to keep game speed constant
         DWORD start_tick = GetTickCount();

         // is there a message in queue, if so get it
         while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
         { 
            // test if this is a quit
            if (msg.message == WM_QUIT) __leave;

            // translate any accelerator keys
            TranslateMessage(&msg);

            // send the message to WinProc
            DispatchMessage(&msg);

         } // end while

         GameMain();  // game main processing function called here

         // check for <ESC> key and send quit game
         if (KEYDOWN(VK_ESCAPE)) SendMessage (hwnd, WM_CLOSE, 0, 0);

         // wait until we hit correct game speed frame rate
         while ((GetTickCount() - start_tick) < GAME_SPEED);

      } // end while
   }
   __finally
   {
      GameQuit();  // game quit function and clean up before exit called here
   }

   return(msg.wParam); // return to Windows

} // end WinMain

// BEGIN GAME CODE ////////////////////////////////////////

///////////////////////////////////////////////////////////
//
// GAME INITIALIZATION
//
///////////////////////////////////////////////////////////

void GameInit(void)
{

   srand(GetTickCount()); // seed the random numbers

   // initialize globals
   red_pen = CreatePen(PS_SOLID, 1, RGB(255,0,0));
   red_brush = CreateSolidBrush(RGB(255,0,0));
   green_pen = CreatePen(PS_SOLID, 1, RGB(0,255,0));
   green_brush = CreateSolidBrush(RGB(0,255,0));
   blue_pen = CreatePen(PS_SOLID, 1, RGB(0,0,255));
   blue_brush = CreateSolidBrush(RGB(0,0,255));
   white_pen = CreatePen(PS_SOLID, 1, RGB(255,255,255));
   white_brush = CreateSolidBrush(RGB(255,255,255));
   black_pen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
   black_brush = CreateSolidBrush(RGB(0,0,0));

   // temporary change to full screen mode
   ZeroMemory(&game_screen, sizeof(game_screen)); // clear out size of DEVMODE struct

   game_screen.dmSize = sizeof(game_screen);
   game_screen.dmPelsWidth = WINDOW_WIDTH;
   game_screen.dmPelsHeight = WINDOW_HEIGHT;
   game_screen.dmBitsPerPel = 16;
   game_screen.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

   ChangeDisplaySettings(&game_screen, CDS_FULLSCREEN);

   game_dc = GetDC(game_window); // get the GDI device context

   ShowCursor(FALSE);

} // END OF GameInit


///////////////////////////////////////////////////////////
//
// GAME MAIN LOOP AND PROCESSING
//
///////////////////////////////////////////////////////////

void GameMain(void)
{

   switch(game_state)
   {
      case GAME_STATE_DEMO_INIT:
      {

         DrawBlocks();

         SetTextColor(game_dc, RGB(255,255,255)); // white text color
         SetBkColor(game_dc, RGB(0,0,0));         // black background

         sprintf(text,"VazBreak a game by PhilVaz (c) 2003");
         TextOut(game_dc, 285, 50, text, strlen(text));

         sprintf(text,"Press <space bar> to begin");
         TextOut(game_dc, 310, 70, text, strlen(text));

         sprintf(text,"Arrow keys or mouse moves the paddle");
         TextOut(game_dc, 285, 90, text, strlen(text));

         sprintf(text,"   !!! Press <ESC key> to end !!!   ");
         TextOut(game_dc, 310, 240, text, strlen(text));

         DisplayScore();
         SetBall(9);     // move ball fast for demo

         game_state = GAME_STATE_DEMO_RUN;

      } break;

      case GAME_STATE_DEMO_RUN:
     {

         MoveBall();

        // check for <SPACE BAR> key and if pressed move to GAME_INIT state
         if (KEYDOWN(VK_SPACE)) game_state = GAME_STATE_GAME_INIT;

      } break;

      case GAME_STATE_GAME_INIT:
     {

        DrawBlocks();

        game_score = 0; game_level = 1; paddles_left = 3;

        DisplayScore();

        SetBall(3);
        SetPaddle();

        game_state = GAME_STATE_GAME_RUN;


      } break;

      case GAME_STATE_GAME_RUN:
     {

         MoveBall();
         MovePaddle();

         //
         // CHECK FOR COLLISION WITH PADDLE
         //

         // is the ball y position at paddle position?

         if (yball >= 544)
         {

            // if so, check if paddle missed ball

            if ((xball + 16 < xpaddle - 1) || (xball > xpaddle + 61))
            {

               paddles_left -= 1;  // subtract one from paddles

               // check if no paddles left then game over, go back to DEMO
               if (paddles_left < 1) game_state = GAME_STATE_DEMO_INIT;

               // erase current ball
               SelectObject(game_dc, black_pen);
               SelectObject(game_dc, black_brush);
               Ellipse(game_dc, xball, yball, xball + 16, yball + 16);

               // set new ball position and re-display score

               if (game_level == 1) SetBall(3);
               if (game_level == 2) SetBall(4);
               if (game_level > 2) SetBall(5);

               DisplayScore();

               PlaySound(MAKEINTRESOURCE(SOUND_MISS), game_instance, SND_ASYNC | SND_RESOURCE);

            } // end of paddle missed ball

            else  // paddle did hit ball, determine deflection angle (xmove)
            {

               if (xball < xpaddle + 6) xmove = -9;

               if ((xball > xpaddle + 5)  && (xball < xpaddle + 11)) xmove = -7;
               if ((xball > xpaddle + 10) && (xball < xpaddle + 16)) xmove = -5;
               if ((xball > xpaddle + 15) && (xball < xpaddle + 21)) xmove = -3;
               if ((xball > xpaddle + 20) && (xball < xpaddle + 26)) xmove = -2;
               if ((xball > xpaddle + 25) && (xball < xpaddle + 36)) xmove = 0;
               if ((xball > xpaddle + 35) && (xball < xpaddle + 41)) xmove = 2;
               if ((xball > xpaddle + 40) && (xball < xpaddle + 46)) xmove = 3;
               if ((xball > xpaddle + 45) && (xball < xpaddle + 51)) xmove = 5;
               if ((xball > xpaddle + 50) && (xball < xpaddle + 56)) xmove = 7;

               if (xball > xpaddle + 55) xmove = 9;

               ymove = -ymove;

               PlaySound(MAKEINTRESOURCE(SOUND_PADDLE), game_instance, SND_ASYNC | SND_RESOURCE);

            } // end of paddle hit ball


         } // end of if

         //
         // CHECK FOR COLLISION WITH BLOCKS
         //

         int i;   // for counting loop
         int x,y; // for x and y block position

         // used to indicate at least one block hit, in the (rare?) case
         // that two blocks are hit, the y direction is only changed once
         bool hit_one = FALSE;

         RECT rect; // rectangle info

         for (i = 0; i < 135; i++)
         {

            x = xblock[i]; y = yblock[i];

            // if the block still exists, test the four corners of the ball against the
            // block x and y position (which is the top-left of block rectangle)

            if ( block_exist[i] &&
              ((xball+xmove+16 > x && xball+xmove+16 < x+47 && yball+ymove+16 > y && yball+ymove+16 < y+15) ||
              (xball+xmove > x && xball+xmove < x+47 && yball+ymove+16 > y && yball+ymove+16 < y+15) ||
              (xball+xmove+16 > x && xball+xmove+16 < x+47 && yball+ymove > y && yball+ymove < y+15) ||
              (xball+xmove > x && xball+xmove < x+47 && yball+ymove > y && yball+ymove < y+15)) )

            {  // block is hit

               block_exist[i] = FALSE;
               hit_one = TRUE;

               // erase the block

               rect.left = x; rect.top = y; rect.right = x+47; rect.bottom = y+15;

               SelectObject(game_dc, black_pen);
               SelectObject(game_dc, black_brush);
               FillRect(game_dc, &rect, black_brush);

               if (i > 89)  // bottom (green) row worth 10 points
               {
                  game_score += 10;

                  PlaySound(MAKEINTRESOURCE(SOUND_HIT1), game_instance, SND_ASYNC | SND_RESOURCE);
               }

               if (i > 44 && i < 90)  // middle (blue) row worth 20 points
               {
                  game_score += 20;

                  if (ymove < 0)
                  {
                     ymove = -6;
                     if (game_level == 2) ymove = -7;
                     if (game_level > 2) ymove = -8;
                  }

                  if (ymove > 0)
                  {
                     ymove = 6;
                     if (game_level == 2) ymove = 7;
                     if (game_level > 2) ymove = 8;
                  }

                  PlaySound(MAKEINTRESOURCE(SOUND_HIT2), game_instance, SND_ASYNC | SND_RESOURCE);
               }

               if (i < 45)  // top (red) row worth 50 points
               {
                  game_score += 50;

                  if (ymove < 0)
                  {
                     ymove = -9;
                     if (game_level == 2) ymove = -10;
                     if (game_level > 2) ymove = -12;
                  }

                  if (ymove > 0)
                  {
                     ymove = 9;
                     if (game_level == 2) ymove = 10;
                     if (game_level > 2) ymove = 12;
                  }

                  PlaySound(MAKEINTRESOURCE(SOUND_HIT3), game_instance, SND_ASYNC | SND_RESOURCE);
               }

               DisplayScore();  // refresh score

               blocks_hit += 1; // increment blocks hit and test if ALL hit
               if (blocks_hit > 134) game_state = GAME_STATE_GAME_RESET;

            } // end of if hit block

         } // end of for

         // if at least one block was hit, then bounce y direction of ball

         if (hit_one) ymove = -ymove;

         // if 'p' key pressed, set state to GAME_PAUSE

         if (KEYDOWN(80)) game_state = GAME_STATE_GAME_PAUSE;

      } break;

      case GAME_STATE_GAME_RESET:
      {

         DrawBlocks();

         game_score += 500; game_level += 1;

         DisplayScore();

         if (game_level == 1) SetBall(3);
         if (game_level == 2) SetBall(4);
         if (game_level > 2) SetBall(5);

         SetPaddle();

         game_state = GAME_STATE_GAME_RUN;

      } break;

      case GAME_STATE_GAME_PAUSE:
      {

         // if <SPACE BAR> key pressed resume game
         if (KEYDOWN(VK_SPACE)) game_state = GAME_STATE_GAME_RUN;

      } break;

   } // end switch

} // END OF GameMain


///////////////////////////////////////////////////////////
//
// GAME QUIT AND CLEAN UP
//
///////////////////////////////////////////////////////////

void GameQuit(void)
{

   // delete the pens and brushes
   DeleteObject(red_pen);
   DeleteObject(red_brush);
   DeleteObject(green_pen);
   DeleteObject(green_brush);
   DeleteObject(blue_pen);
   DeleteObject(blue_brush);

   DeleteObject(white_pen);
   DeleteObject(white_brush);

   DeleteObject(black_pen);
   DeleteObject(black_brush);

   // release the device context (for GDI) from the game window
   ReleaseDC(game_window, game_dc);

   // return to original display settings
   ChangeDisplaySettings(NULL,0);


} // END OF GameQuit

//////////////////////////////////////////////
//
// clears screen, draws border and blocks
//
/////////////////////////////////////////////

void DrawBlocks(void)
{
   int i;      // for loop
   int x,y;    // for x and y block positions
   RECT rect;  // rectangle info

   // clear entire screen

   rect.left = 0; rect.top = 0;
   rect.right = 800; rect.bottom = 600;

   SelectObject(game_dc, black_pen);
   SelectObject(game_dc, black_brush);

   FillRect(game_dc, &rect, black_brush);

   // left border only change right coord

   rect.right = 25;

   SelectObject(game_dc, white_pen);
   SelectObject(game_dc, white_brush);

   FillRect(game_dc, &rect, white_brush);

   // top border

   rect.right = 800; rect.bottom = 25;

   FillRect(game_dc, &rect, white_brush);

   // right border

   rect.left = 778; rect.bottom = 600;

   FillRect(game_dc, &rect, white_brush);

   // set x and y positions of blocks

   x = 28;  // starting x position of first block
   y = 128; // starting y position of first block

   for (i = 0; i < 135; i++)   // exactly 135 blocks
   {

      // fill in all the blocks

      rect.left = x; rect.top = y;
      rect.right = x + 47; rect.bottom = y + 15;
     
      if (i > 89)  // bottom row green
      {
         SelectObject(game_dc, green_pen);
         SelectObject(game_dc, green_brush);
         FillRect(game_dc, &rect, green_brush);
      }

      if (i > 44 && i < 90)  // middle row blue
      {
         SelectObject(game_dc, blue_pen);
         SelectObject(game_dc, blue_brush);
         FillRect(game_dc, &rect, blue_brush);
      }

      if (i < 45)  // top row red
      {
         SelectObject(game_dc, red_pen);
         SelectObject(game_dc, red_brush);
         FillRect(game_dc, &rect, red_brush);
      }

      block_exist[i] = TRUE;     // set block exist
      xblock[i]=x; yblock[i]=y;  // save block x and y position

     x += 50;  // blocks are 47 + 3 (gap) pixels wide

     if (x > 728)
     {
        y += 18; x = 28;  // blocks are 15 + 3 (gap) pixels high
      }

   } // end of for

   blocks_hit = 0;

   PlaySound(MAKEINTRESOURCE(SOUND_SERVE), game_instance, SND_ASYNC | SND_RESOURCE);

} // END OF DrawBlocks

/////////////////////////////////////////////////////////
//
// display text score/level/paddles bottom of screen
//
/////////////////////////////////////////////////////////

void DisplayScore(void)
{

   SetTextColor(game_dc, RGB(255,255,255));  // white text color
   SetBkColor(game_dc, RGB(0,0,0)); // black background

   sprintf(text,"SCORE = %d", game_score);
   TextOut(game_dc, 80, 580, text, strlen(text));

   sprintf(text,"LEVEL = %d", game_level);
   TextOut(game_dc, 210, 580, text, strlen(text));

   sprintf(text,"PADDLES = %d", paddles_left);
   TextOut(game_dc, 340, 580, text, strlen(text));

   sprintf(text,"VazBreak a game by PhilVaz (c) 2003");
   TextOut(game_dc, 500, 580, text, strlen(text));

} // END OF DisplayScore

/////////////////////////////////////////////////
//
// set ball at speed, can be slow +3 or fast +9
//
/////////////////////////////////////////////////

void SetBall(int speed)
{

   // set x and y initial ball position in proper range
   xball = (rand()%600) + 78; yball = 300;

   // set x movement random, y movement at specified speed
   xmove = (rand()%18) - 9;
   if (xmove == 0) xmove = 2;   // don't allow 0 for initial x movement
   if (xmove == -1) xmove = -2; // don't allow -1 or +1 for angle
   if (xmove == 1) xmove = 2; 

   ymove = speed;

} // END OF SetBall

///////////////////////////////////////////
//
// paddle set in middle of screen
//
///////////////////////////////////////////

void SetPaddle(void)
{

   RECT rect;

   xpaddle = 370;

   rect.left = xpaddle; rect.top = ypaddle;
   rect.right = xpaddle + 60; rect.bottom = ypaddle + 15;

   SelectObject(game_dc, white_pen);
   SelectObject(game_dc, white_brush);

   FillRect(game_dc, &rect, white_brush);

} // END OF SetPaddle

//////////////////////////////
//
// moves ball once
//
/////////////////////////////

void MoveBall(void)
{

   // erase current ball
   SelectObject(game_dc, black_pen);
   SelectObject(game_dc, black_brush);
   Ellipse(game_dc, xball, yball, xball + 16, yball + 16);

   // move ball position
   xball += xmove; yball += ymove;

   // check for bounce on x axis
   if (xball < 25 || xball > 762)
   {
      xmove = -xmove; xball += xmove; // x bounce and push back
      if (game_state == GAME_STATE_GAME_RUN)
      {
         PlaySound(MAKEINTRESOURCE(SOUND_SIDE), game_instance, SND_ASYNC | SND_RESOURCE);
      }
   }

   // if in GAME_RUN state, check if reached paddle position and hold there
   if ((game_state == GAME_STATE_GAME_RUN) && (yball > 544)) yball = 544;

   // check for bounce on y axis
   if (yball < 27 || yball > 560)
   {
      ymove = -ymove; yball += ymove; // y bounce and push back
      if (game_state == GAME_STATE_GAME_RUN)
      {
         PlaySound(MAKEINTRESOURCE(SOUND_TOP), game_instance, SND_ASYNC | SND_RESOURCE);
      }
   }

   // if in DEMO_RUN state check for bounce just before the new blocks
   if ((game_state == GAME_STATE_DEMO_RUN) && (yball < 287))
   {
      ymove = -ymove; yball += ymove; // y bounce and push back
   }

   // draw new ball position
   SelectObject(game_dc, white_pen);
   SelectObject(game_dc, white_brush);
   Ellipse(game_dc, xball, yball, xball + 16, yball + 16);
   
} // END OF MoveBall

/////////////////////////////////////////////////////////////
//
// checks for left/right arrow keys and moves paddle once
//
/////////////////////////////////////////////////////////////

void MovePaddle(void)
{

   RECT rect;
   int xold = xpaddle;  // save old (previous) position of paddle

   // check if left or right arrow key down

   if (KEYDOWN(VK_LEFT))  xpaddle -= 15;
   if (KEYDOWN(VK_RIGHT)) xpaddle += 15;

   if (xoldmouse != xmouse) xpaddle = xmouse;

   if (xpaddle < 26)  xpaddle = 26;
   if (xpaddle > 716) xpaddle = 716;

   if (xold != xpaddle)   // if the paddle was moved
   {

      // erase old position of paddle

      rect.left = xold; rect.top = ypaddle;
      rect.right = xold + 60; rect.bottom = ypaddle + 15;

      SelectObject(game_dc, black_pen);
      SelectObject(game_dc, black_brush);

      FillRect(game_dc, &rect, black_brush);

      // draw new position of paddle

      rect.left = xpaddle; rect.right = xpaddle + 60;

      SelectObject(game_dc, white_pen);
      SelectObject(game_dc, white_brush);

      FillRect(game_dc, &rect, white_brush);

   } // end of if

} // END OF MovePaddle

// END GAME CODE //////////////////////////////////////////
