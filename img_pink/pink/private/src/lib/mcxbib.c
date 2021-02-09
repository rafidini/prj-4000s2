/* $Id: mcxbib.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* ==================================================================== */
/* a simple drawing library */
/* from "X Window Application Programming" */

#include <mcxbib.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

/* ====================================================== */
/* globals */
/* ====================================================== */

#define NBCOLORS 128

Display       *theDisplay;
Window        theWindow;
GC            theGC;
XFontStruct   *theFont;
Colormap      theColormap;
int32_t           theScreen;
int32_t           theDepth;
uint32_t thePlane[1];
uint32_t thePixel[NBCOLORS];
XColor        theColors[NBCOLORS];
uint32_t theBlackPixel;
uint32_t theWhitePixel;

/* ==================================================================== */
/* init code to talk to the X server */
/* ==================================================================== */

/* =========================================== */
void initX()
/* =========================================== */
/* sets up the connection to the X server      */
{
  theDisplay = XOpenDisplay(NULL);
  if (theDisplay == NULL) {
    fprintf(stderr, "Error: cannot establish a connection to the X server\n");
    exit(1);
  } /* if */
  
  theScreen = DefaultScreen( theDisplay );
  theDepth  = DefaultDepth( theDisplay, theScreen );
  theBlackPixel  = BlackPixel( theDisplay, theScreen );
  theWhitePixel  = WhitePixel( theDisplay, theScreen );
  theFont = XLoadQueryFont(theDisplay, FONTE18);

} /* initX() */

/* =========================================== */
void getXInfo()
/* =========================================== */
{
  
} /* getXInfo() */

/* ==================================================================== */
/* routine to close down X - 
   from "X Window Application Programming" */
/* ==================================================================== */

/* =========================================== */
void quitX()
/* =========================================== */
/* closes the connection to the X server       */
{
  XCloseDisplay( theDisplay );
  
} /* quitX() */


/* ==================================================================== */
/* window opening routine - from "X Window Application Programming" */
/* ==================================================================== */

/* ==================================== */
int32_t createGC(Window theNewWindow, GC     *theNewGC) 
/* creates a Graphic Context for the given window     */
/* (necessary to draw into the window)                */
/* returns 0 if error, 1 if OK                        */
/* ==================================== */
{
  XGCValues theGCValues;

  *theNewGC = XCreateGC( theDisplay, theNewWindow, 
                         (uint32_t)0, &theGCValues );
  if (*theNewGC == 0) return(0);
  else {
    XSetForeground( theDisplay, *theNewGC, theBlackPixel );
    XSetBackground( theDisplay, *theNewGC, theWhitePixel );
    return(1);
  } /* else */
} /* createGC() */

#define LINESOLID 0
#define LINEONOFFDASH 1
#define LINEDOUBLEDASH 2

/* ==================================== */
void changeLineStyle(GC theGC, int32_t linewidth,  int32_t dash) 
/* ==================================== */
{
  XGCValues theGCValues;

  theGCValues.line_width = linewidth;
  switch (dash % 3) {
    case LINESOLID: theGCValues.line_style = LineSolid; break;
    case LINEONOFFDASH: theGCValues.line_style = LineOnOffDash; break;
    case LINEDOUBLEDASH: theGCValues.line_style = LineDoubleDash; break;
  } /* switch */
  
  XChangeGC( theDisplay, theGC, (GCLineWidth | GCLineStyle), &theGCValues);
} /* changeLineStyle() */

/* ==================================== */
Window openWindow(int32_t x, int32_t y, int32_t width, int32_t height, int32_t flag, GC *theNewGC) 
/* opens a new window - returns the window id         */
/* ==================================== */
{
  Window               theNewWindow;

  /* open a window on the display */
  theNewWindow = XCreateSimpleWindow(
                    theDisplay,
                    RootWindow(theDisplay, theScreen),
                    x, y, width, height,
                    BORDER_WIDTH,
                    0,
                    theWhitePixel);

  /* create a Graphic Context for the window */
  if ( createGC(theNewWindow, theNewGC) == 0 ) {
    XDestroyWindow(theDisplay, theNewWindow);
    return( (Window)0 );
  } /* if */

  XSetFont(theDisplay, *theNewGC, theFont->fid);

  /* ask X to place the window visibility on the screen */
  XMapWindow( theDisplay, theNewWindow );

  /* flush X requests to the X server */
  XFlush( theDisplay );

  /* return the window id */
  return( theNewWindow );

} /* openWindow() */

  
/* ==================================== */
void allocNamedColor(Display *theDisplay, Colormap *default_cmap, uint32_t *thePixel, 
                     char * color_name, uint32_t color_code)
/* ==================================== */
{
  XColor exact_def;

  if (!XParseColor(theDisplay, *default_cmap, color_name, &exact_def))
  {
    fprintf(stderr, "openWindow : XParseColor failed\n");
    exit(0);
  }
#ifdef DEBUG_COLOR
printf("requested color %d %d %d\n", exact_def.red, exact_def.green, exact_def.blue);
#endif
  if (!XAllocColor(theDisplay, *default_cmap, &exact_def))
  {
    fprintf(stderr, "openWindow : XAllocColor failed\n");
    exit(0);
  }
#ifdef DEBUG_COLOR
printf("actually got color %d %d %d", exact_def.red, exact_def.green, exact_def.blue);
printf("   pixel value %d\n", exact_def.pixel);    
#endif
  thePixel[color_code] = exact_def.pixel;
}

/* ==================================== */
Window openColorWindow(int32_t x, int32_t y, int32_t width, int32_t height, int32_t flag, GC *theNewGC) 
/* opens a new window - returns the window id         */
/* ==================================== */
{
  Window theNewWindow;
  int32_t flags;
  Status status;
  int32_t i;
  int32_t default_depth;
  Visual *default_visual;
  Colormap default_cmap;
  int32_t screen_num = 0;
  XVisualInfo visual_info;
  static char *visual_class[] = {"StaticGray", "Grayscale", "StaticColor", "PseudoColor", 
    "TrueColor", "DirectColor"};
  XColor exact_def;

  default_depth = DefaultDepth (theDisplay, screen_num);
  default_visual = DefaultVisual (theDisplay, screen_num);
  default_cmap = DefaultColormap (theDisplay, screen_num);

#ifdef DEBUG_COLOR
printf("default depth = %d\n", default_depth);
#endif
  i = 5;
  while (!XMatchVisualInfo(theDisplay, screen_num, default_depth, i, &visual_info)) i--;

#ifdef DEBUG_COLOR
printf("found a %s class visual at depth %d\n", visual_class[i], default_depth);
#endif

  for (i = 0; i < 64; i++)           /* 64 premieres couleurs : echelle de gris */
  {
    exact_def.red = exact_def.green = exact_def.blue = 1024 * i;
#ifdef DEBUG_COLOR
printf("requested color %d %d %d\n", exact_def.red, exact_def.green, exact_def.blue);
#endif
    if (!XAllocColor(theDisplay, default_cmap, &exact_def))
    {
      fprintf(stderr, "openWindow : XAllocColor failed\n");
      exit(0);
    }
#ifdef DEBUG_COLOR
printf("actually got color %d %d %d", exact_def.red, exact_def.green, exact_def.blue);
printf("   pixel value %d\n", exact_def.pixel);    
#endif
    thePixel[i] = exact_def.pixel;
  }
  
  allocNamedColor(theDisplay, &default_cmap, thePixel, "red",     MC_RED);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "green",   MC_GREEN);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "blue",    MC_BLUE);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "yellow",  MC_YELLOW);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "cyan",    MC_CYAN);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "magenta", MC_MAGENTA);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "orange",  MC_ORANGE);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "purple",  MC_PURPLE);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "brown",   MC_BROWN);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "pink",    MC_PINK);
  allocNamedColor(theDisplay, &default_cmap, thePixel, "LightGrey", MC_LIGHTGREY);

  /* open a window on the display */
  theNewWindow = XCreateSimpleWindow(
                    theDisplay,
                    RootWindow(theDisplay, theScreen),
                    x, y, width, height,
                    BORDER_WIDTH,
                    0,
                    theBlackPixel);

  /* create a Graphic Context for the window */
  if ( createGC(theNewWindow, theNewGC) == 0 ) {
    XDestroyWindow(theDisplay, theNewWindow);
    return( (Window)0 );
  } /* if */

  XSetFont(theDisplay, *theNewGC, theFont->fid);

  /* ask X to place the window visibility on the screen */
  XMapWindow( theDisplay, theNewWindow );

  /* flush X requests to the X server */
  XFlush( theDisplay );

  /* return the window id */
  return( theNewWindow );

} /* openColorWindow() */

/* ==================================================================== */
/* routines to deal with mouse button events - 
   from "X Window Application Programming" */
/* ==================================================================== */

#define EV_MASK (ButtonPressMask|ButtonReleaseMask|KeyPressMask|EnterWindowMask)

/* ==================================== */
void WaitMouseEvent(int32_t *x, int32_t *y) 
/* blocks awaiting an event      */
/* ==================================== */
{
  XEvent theEvent;
  XComposeStatus theComposeStatus;
  XNextEvent( theDisplay, &theEvent );
  if (theEvent.type != ButtonPress) WaitMouseEvent(x,y);
  else
  {
    *x = theEvent.xbutton.x;
    *y = theEvent.xbutton.y;
  }
} /* WaitMouseEvent() */

/* ============================= */
int32_t WaitKbdEvent() 
/* ============================= */
/* blocks awaiting an event      */
{
  XEvent theEvent;
  XComposeStatus theComposeStatus;
  XNextEvent( theDisplay, &theEvent );
  if (theEvent.type == KeyPress)
    return theEvent.xkey.keycode;
  else
    return -1;
} /* WaitKbdEvent() */

/* ==================================== */
int32_t WaitEvent(int32_t *x, int32_t *y) 
/* blocks awaiting an event      */
/* ==================================== */
{
  XEvent theEvent;
  XComposeStatus theComposeStatus;
  XNextEvent( theDisplay, &theEvent );
  if (theEvent.type == KeyPress)
    *x = theEvent.xkey.keycode;
  else
  {
    *x = theEvent.xbutton.x;
    *y = theEvent.xbutton.y;
  }
  return theEvent.type;
} /* WaitEvent() */

/* ==================================== */
int32_t WaitAnyEvent(int32_t *x, int32_t *y, int32_t *c) 
/* blocks awaiting any event      */
/* ==================================== */
{
  XEvent theEvent;
  XComposeStatus theComposeStatus;
  XNextEvent( theDisplay, &theEvent );
  if (theEvent.type == KeyPress)
    *x = theEvent.xkey.keycode;
  else
  {
    *x = theEvent.xbutton.x;
    *y = theEvent.xbutton.y;
    *c = theEvent.xbutton.button;
  }
  return theEvent.type;
} /* WaitAnyEvent() */

/* ==================================== */
void initEvents(Window theWindow) 
/* ==================================== */
{
  (void)XSelectInput( theDisplay, theWindow, EV_MASK );
} /* initEvents() */

/* ==================================================================== */
/* window drawing routines - from "X Window Application Programming" */
/* ==================================================================== */

/* ==================================== */
void setColToWhite(GC gc)  
/* ==================================== */
{
  (void)XSetForeground(theDisplay, gc, theWhitePixel);
}

/* ==================================== */
void setColToBlack(GC gc)  
/* ==================================== */
{
  (void)XSetForeground(theDisplay, gc, theBlackPixel);
}

/* ==================================== */
void drawLine(Window theWindow, GC theGC, int32_t x1, int32_t y1,  int32_t x2, int32_t y2) 
/* ==================================== */
{
  (void)XDrawLine(theDisplay, theWindow, theGC, x1, y1, x2, y2);
} /* drawLine() */

/* ==================================== */
void drawPoint(Window theWindow, GC theGC, int32_t x, int32_t y) 
/* ==================================== */
{
  (void)XDrawPoint(theDisplay, theWindow, theGC, x, y);
} /* drawPoint() */

/* ==================================== */
void fillRectangle(Window theWindow, GC theGC, int32_t x, int32_t y, int32_t w, int32_t h) 
/* ==================================== */
{
  (void)XFillRectangle(theDisplay, theWindow, theGC, x, y, w, h);
} /* fillRectangle() */

/* ==================================================================== */
/* routines to deal with text - 
   from "X Window Application Programming" */
/* ==================================================================== */

/* ==================================== */
XFontStruct * initFont (GC theGC, char fontName[]) 
/* ==================================== */
{
  XFontStruct *fontStruct;

  fontStruct = XLoadQueryFont( theDisplay, fontName );
  if (fontStruct != 0) XSetFont(theDisplay, theGC, fontStruct->fid);
  return(fontStruct);
} /* initFont() */

/* ==================================== */
void drawString(Window theWindow, GC theGC, int32_t x, int32_t y, char *str) 
/* ==================================== */
{
  XDrawString( theDisplay, theWindow, theGC, x, y, str, strlen(str) );
} /* drawString() */


/* ==================================================================== */
/* ==================================================================== */
/* Higher level functions, allowing a simpler interface */
/* ==================================================================== */
/* ==================================================================== */

/* ==================================================================== */
/* drawing routines */
/* ==================================================================== */

/* ==================================== */
void ColToWhite() 
/* ==================================== */
{ 
  XSetForeground(theDisplay, theGC, theWhitePixel); 
}

/* ==================================== */
void ColToBlack() 
/* ==================================== */
{ 
  XSetForeground(theDisplay, theGC, theBlackPixel); 
}

/* ==================================== */
void Col( uint32_t c )                  /* for windows created by openWindow() */
/* ==================================== */
{ 
  XSetForeground(theDisplay, theGC, c); 
}

/* ==================================== */
void Color( uint32_t c )                  /* for windows created by openColorWindow() */
/* ==================================== */
{ 
  XSetForeground(theDisplay, theGC, thePixel[c]); 
}

/* ==================================== */
void Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2)  
/* ==================================== */
{
  XDrawLine(theDisplay, theWindow, theGC, x1, y1, x2, y2);
}

/* ==================================== */
void Point(int32_t x, int32_t y) 
/* ==================================== */
{ 
  XDrawPoint(theDisplay, theWindow, theGC, x, y);
}

/* ==================================== */
void FRectangle(int32_t x, int32_t y, int32_t w, int32_t h) 
/* ==================================== */
{
  XFillRectangle(theDisplay, theWindow, theGC, x, y, w, h);
}

/* ==================================== */
void Rectangle(int32_t x, int32_t y, int32_t w, int32_t h) 
/* ==================================== */
{
  Line(x, y, x+w, y);  
  Line(x, y, x, y+h);  
  Line(x, y+h, x+w, y+h);  
  Line(x+w, y, x+w, y+h);  
}

/* ==================================================================== */
/* routines to deal with text - 
   from "X Window Application Programming" */
/* ==================================================================== */

/* ==================================== */
void SetFont (char *fontName) 
/* ==================================== */
{
  XFontStruct *fontStruct;

  fontStruct = XLoadQueryFont( theDisplay, fontName );
  if (fontStruct != 0) XSetFont(theDisplay, theGC, fontStruct->fid);
}

/* ==================================== */
void String(int32_t x, int32_t y, char *str) 
/* ==================================== */
{
  XDrawString( theDisplay, theWindow, theGC, x, y, str, strlen(str) );
}

/* ==================================================================== */
/* routines to initiate, refresh and terminate */
/* ==================================================================== */

/* ==================================== */
void InitGraphics(int32_t x, int32_t y, int32_t w, int32_t h) 
/* ==================================== */
{
  initX();

  theWindow = openWindow( x, y, w, h, 
                          0,        /* 0: not a pop-up */
                          &theGC );

  initEvents( theWindow );
}

/* ==================================== */
void InitColorGraphics(int32_t x, int32_t y, int32_t w, int32_t h) 
/* ==================================== */
{
  initX();

  theWindow = openColorWindow( x, y, w, h, 
                          0,        /* 0: not a pop-up */
                          &theGC );

  initEvents( theWindow );
}

/* ==================================== */
void FlushGraphics()
/* ==================================== */
{
  /* send output to the screen */
  XFlush( theDisplay );
}
 
/* ==================================== */
void TerminateGraphics()
/* ==================================== */
{
  XDestroyWindow( theDisplay, theWindow );

  quitX();
}

#ifdef TESTX
void main () 
{
  int32_t x, y;
  int32_t col;
  
  InitGraphics( 100, 100, /* x, y */
                300, 300  /* w, h */
              );

  /* draw into the window */
  Line( 10, 10, 100, 100 );
     /* x1  y1   x2   y2 */
  /* send output to the screen */
  FlushGraphics();

  printf("press mouse button to continue\n");
  WaitMouseEvent(&x, &y);
  printf("mouse event - x = %d ; y = %d\n", x, y);

  ColToWhite();
  FRectangle(0, 0, 300, 300);
  ColToBlack();

  String(20, 20, "coucou !");
  FlushGraphics();

  printf("press a key\n");
  do
  {
    x = WaitKbdEvent();
    printf("kbd event - x = %d\n", x);
  } while (x != 1);

  TerminateGraphics();
 
} /* main() */
#endif
