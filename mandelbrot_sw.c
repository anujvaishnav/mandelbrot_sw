#include "mandelbrot_sw.h"

int createWindow(int width, int height)
{
  unsigned long black, white;
  
  //Open Display
  dis = XOpenDisplay(getenv("DISPLAY"));
  if (dis == NULL) {
    printf("Couldn't open display.\n");
    return -1;
  }

  //Create Window
  int const x = 0, y = 0, border_width = 1;
  int screen    = DefaultScreen(dis);
  Window ro     = DefaultRootWindow(dis);
  black = BlackPixel(dis,screen),  /* get color black */
  white = WhitePixel(dis, screen);  /* get color white */
  
  win = XCreateSimpleWindow(dis, ro, x, y, width, height, border_width, 
                              black, white);
                              
  XMapWindow(dis, win); //Make window visible
  XStoreName(dis, win, "Mandelbrot"); // Set window title
  
  /* this routine determines which types of input are allowed in
     the input.  see the appropriate section for details...
  */
  XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);
  
  /* create the Graphics Context */
  gc = XCreateGC(dis, ro, 0, NULL);
  
  /* here is another routine to set the foreground and background
     colors _currently_ in use in the window.
  */
  XSetBackground(dis, gc, white);
  XSetForeground(dis, gc, black);
  
  /* clear the window and bring it on top of the other windows */
  XClearWindow(dis, win);
  XMapRaised(dis, win);
  
  return 0;
}

int buildColor(int red, int green, int blue)
{  
 return(
        (((int)(red)%256)<<16)+
        (((int)(green)%256)<<8)+
        (((int)(blue)%256)));
} 

void drawPixel(int x, int y, int color)
{
  XSetForeground(dis, gc, color);
  XDrawPoint(dis, win, gc, x, y);
}

void close_display()
{
  XFreeGC(dis, gc);
  XDestroyWindow(dis, win);
  XCloseDisplay(dis);
  exit(1);
}

int mandelbrot(uint32_t ImageWidth, uint32_t ImageHeight, uint32_t MaxIterations, 
               double cRe, double cIm, uint32_t zoom)
{

  double Re_factor = 0.01 / zoom;
  double Im_factor = 0.01 / zoom;
  
  double MinRe = cRe - Re_factor*(ImageWidth/2);
  double MinIm = cIm - Im_factor*(ImageHeight/2);

  double MaxRe = MinRe + Re_factor*ImageWidth;
  double MaxIm = MinIm + Im_factor*ImageHeight;

  uint32_t colour_unit = (uint32_t)((1 << 24) / (MaxIterations));

  for(unsigned y = 0; y < ImageHeight; y++)
  {
    double c_im = MaxIm - y*Im_factor;
    for(unsigned x = 0; x < ImageWidth; x++)
    {
      double c_re = MinRe + x*Re_factor;

      // Calculate whether c belongs to the Mandelbrot set or
      // not and draw a pixel at coordinates (x,y) accordingly
      double Z_re = c_re, Z_im = c_im; // Set Z = c
      bool isInside = true;
      unsigned n = 0;
      
      for(n = 0; n < MaxIterations; n++)
      {
        double Z_im2 = Z_im*Z_im;
        double Z_re2 = Z_re*Z_re;
        
        if(Z_re2 + Z_im2 > 4) // |z| > 2
        {
          isInside = false;
          break;
        }
        /*
          N.B. Z^2 = (a + bi)^2 = (a^2 - b^2) + (2ab)i
        */
        
        Z_im = 2*Z_re*Z_im + c_im;
        Z_re = Z_re2 - Z_im2 + c_re;
        
      }
      
      if(isInside) 
      { 
        drawPixel(x, y, buildColor(0, 0, 0));
      } // if
      else
      {
        drawPixel(x, y, colour_unit * n);        
      }
    } // for
  } // for
} // mandelbrot()

void main()
{
  unsigned int ImageWidth = 500;
  unsigned int ImageHeight = 500;
  unsigned int MaxIterations = 50;
  
  int zoom = 1;
  double cRe = -1.25;
  double cIm = -0.18;

  if(createWindow(ImageWidth, ImageHeight) != -1)
  {
    printf("created window\n");
    
/*    XEvent ev;*/
    
    XEvent event;    /* the XEvent declaration !!! */
    KeySym key;    /* a dealie-bob to handle KeyPress Events */  
    char text[255];    /* a char buffer for KeyPress Events */
    
    bool quit = false;
    bool zoom_on = false;
    while (!quit) 
    {
      if(XCheckWindowEvent(dis, win, KeyPressMask, &event))
      {
        if(event.type == KeyPress
           && XLookupString(&event.xkey, text, 255, &key, 0) == 1) 
        {
          /* use the XLookupString routine to convert the invent
             KeyPress data into regular text.  Weird but necessary...
          */
          if (text[0]=='q') 
            quit = true;
            
          switch(text[0])
          {
            // quit
            case 'q': 
              quit = true;
              break;
            // toggle zoom
            case 'z': 
              zoom_on = !zoom_on;
              break;
            // reset
            case 'r': 
              zoom = 1;
              zoom_on = false;
              
              cRe = -1.25;
              cIm = -0.18;
              break;
            // Move left and right  
            case 'a':
              cRe -= 0.1;
              break;
            case 'd':
              cRe += 0.1;
              break;
            case 'w':
              cIm += 0.1;
              break;
            case 's': 
              cIm -= 0.1;
              break;
              
            default:
              break;
          } // switch
        } // if key press
      } // if
      
      // continue drawing otherwise
      mandelbrot(ImageWidth, ImageHeight, MaxIterations, cRe, cIm, zoom);
      
      if(zoom_on)
        zoom++;
      
    } // while
    
    close_display();
  } // if
  else
  {
    perror("Could not create window. Exiting...");
    exit(-1);
  } // else
} // main()
