
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

int red, green, blue, yellow,orange,black;
unsigned long foreground, background;


//*************************************************************************************************************************
//funkcja przydzielania kolorow

int AllocNamedColor(char *name, Display* display, Colormap colormap)
  {
    XColor col;
    XParseColor(display, colormap, name, &col);
    XAllocColor(display, colormap, &col);
    return col.pixel;
  } 

//*************************************************************************************************************************
// inicjalizacja zmiennych globalnych okreslajacych kolory

int init_colors(Display* display, int screen_no, Colormap colormap)
{
  background = WhitePixel(display, screen_no);  //niech tlo bedzie biale
  foreground = BlackPixel(display, screen_no);  //niech ekran bedzie czarny
  red=AllocNamedColor("red", display, colormap);
  green=AllocNamedColor("green", display, colormap);
  blue=AllocNamedColor("blue", display, colormap);
  yellow=AllocNamedColor("yellow", display, colormap);
  orange=AllocNamedColor("orange",display,colormap);
  black=AllocNamedColor("black",display,colormap);
}

//*************************************************************************************************************************
// Glowna funkcja zawierajaca petle obslugujaca zdarzenia */

int main(int argc, char *argv[])
{
  char            icon_name[] = "Grafika";
  char            title[]     = "Grafika komputerowa";
  Display*        display;    //gdzie bedziemy wysylac dane (do jakiego X servera)
  Window          window;     //nasze okno, gdzie bedziemy dokonywac roznych operacji
  GC              gc;         //tu znajduja sie informacje o parametrach graficznych
  XEvent          event;      //gdzie bedziemy zapisywac pojawiajace sie zdarzenia
  KeySym          key;        //informacja o stanie klawiatury 
  Colormap        colormap;
  int             screen_no;
  XSizeHints      info;       //informacje typu rozmiar i polozenie ok
  XPoint points1[]={{110,150},{120,160},{140,140},{140,190},{160,190},{160,110}};
  XPoint points1b[]={{108,148},{118,162},{138,142},{138,192},{162,192},{162,108}};
  XPoint points7[]={{220,130},{220,140},{250,140},{230,190},{250,190},{270,110},{250,130}};
  XPoint points7b[]={{218,126},{218,142},{248,142},{228,192},{252,192},{272,108},{252,126}};
  
  char            buffer[8];  //gdzie bedziemy zapamietywac znaki z klawiatury
  int             hm_keys;    //licznik klawiszy
  int             to_end;
  Bool            close1;
  Bool click=FALSE;
  int factor=10;

  display    = XOpenDisplay("");                //otworz polaczenie z X serverem pobierz dane od zmiennej srodowiskowej DISPLAY ("")
  screen_no  = DefaultScreen(display);          //pobierz domyslny ekran dla tego wyswietlacza (0)
  colormap = XDefaultColormap(display, screen_no);
  init_colors(display, screen_no, colormap);

  //okresl rozmiar i polozenie okna
  info.x = 0;
  info.y = 0;
  info.width = 800;
  info.height = 600;
  info.flags = PPosition | PSize;

  //majac wyswietlacz, stworz okno - domyslny uchwyt okna
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),info.x, info.y, info.width, info.height, 7/* grubosc ramki */, foreground, background);
  XSetStandardProperties(display, window, title, icon_name, None, argv, argc, &info);
  //utworz kontekst graficzny do zarzadzania parametrami graficznymi (0,0) domyslne wartosci
  gc = XCreateGC(display, window, 0, 0);
  XSetBackground(display, gc, background);
  XSetForeground(display, gc, foreground);

  //okresl zdarzenia jakie nas interesuja, np. nacisniecie klawisza
  XSelectInput(display, window, (KeyPressMask | ExposureMask | ButtonPressMask| ButtonReleaseMask | Button1MotionMask));
  XMapRaised(display, window);  //wyswietl nasze okno na samym wierzchu wszystkich okien
      
  to_end = FALSE;

 /* petla najpierw sprawdza, czy warunek jest spelniony
     i jesli tak, to nastepuje przetwarzanie petli
     a jesli nie, to wyjscie z petli, bez jej przetwarzania */
  while (to_end == FALSE)
  {
    XNextEvent(display, &event);  // czekaj na zdarzenia okreslone wczesniej przez funkcje XSelectInput
    XSetForeground(display,gc,yellow);
    if (click==TRUE)
      {
	click=FALSE;
	if (close1==TRUE)
	  {
	    for (int n=0; n<6; n++)
	      {
		if (n==2 || n==3)
		  {
		    points1[n].y-=10;
		    points1b[n].y-=10;
		  }
		else
		  {
		    points1[n].y+=10;
		    points1b[n].y+=10;
	          }
	      }
	  }
	else
	  {
	  }
      }
    XFillRectangle(display,window,gc,0,0,800,600);
    XSetForeground(display,gc,black);
    XFillRectangle(display,window,gc,50,50,300,200);
    XFillArc(display,window,gc,50,180,300,125,360*32,360*32);
    XSetForeground(display,gc,orange);
    XFillRectangle(display,window,gc,52,52,296,196);
    XFillArc(display,window,gc,52,182,296,121,360*32,360*32);
    XSetForeground(display,gc,black);
    XFillPolygon(display,window,gc,points1b,6,0,0);
    XFillArc(display,window,gc,points1b[3].x,points1b[3].y-7,24,12,360*32,360*32);
    XFillPolygon(display,window,gc,points7b,7,0,0);
    XFillArc(display,window,gc,points7b[3].x,points7b[3].y-7,24,12,360*32,360*32);
    XSetForeground(display,gc,blue);
    XFillPolygon(display,window,gc,points1,6,0,0);
    XFillArc(display,window,gc,points1[3].x,points1[3].y-5,20,10,360*32,360*32);
    XSetForeground(display,gc,green);
    XFillPolygon(display,window,gc,points7,7,0,0);
    XFillArc(display,window,gc,points7[3].x,points7[3].y-5,20,10,360*32,360*32);

    switch(event.type)
    {
      case Expose:
        if (event.xexpose.count == 0)
        {
         
        }
        break;

      case MappingNotify:
        XRefreshKeyboardMapping(&event.xmapping); // zmiana ukladu klawiatury - w celu zabezpieczenia sie przed taka zmiana trzeba to wykonac
        break;

      case ButtonPress:
        if (event.xbutton.button == Button1)  // sprawdzenie czy wcisnieto lewy przycisk		
        {
   		              
    
        }
        break;


      case KeyPress:
        hm_keys = XLookupString(&event.xkey, buffer, 8, &key, 0);
        if (hm_keys == 1)
        {
          if (buffer[0] == 'q') to_end = TRUE;        // koniec programu
          
        }
    case MotionNotify:
      click=TRUE;
      if (event.xbutton.x<200)
	{
	  close1=TRUE;
	}
      else
	{
	  close1=FALSE;
	}
      default:
        break;
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
