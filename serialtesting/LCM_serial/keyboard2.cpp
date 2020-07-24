// taken from https://blog.robertelder.org/detect-keyup-event-linux-terminal/

/*  Dependencies:

    sudo apt-get install libx11-dev

    I used the following command to compile and run this code:
    g++ main.cpp -g -L/usr/X11/lib -lX11 && ./a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include "lcmtypes/omnibot_speed_command_t.hpp"
#include <lcm/lcm-cpp.hpp>
#include <iostream>
#include <cassert>

bool was_it_auto_repeat(Display * d, XEvent * event, int current_type, int next_type){
  /*  Holding down a key will cause 'autorepeat' to send fake keyup/keydown events, but we want to ignore these: '*/
  if(event->type == current_type && XEventsQueued(d, QueuedAfterReading)){
    XEvent nev;
    XPeekEvent(d, &nev);
    return (nev.type == next_type && nev.xkey.time == event->xkey.time && nev.xkey.keycode == event->xkey.keycode);
  }
  return false;
}

int main () {
  Display * d = XOpenDisplay(NULL);
  Window win = XCreateSimpleWindow(d, RootWindow(d, 0), 1, 1, 400, 300, 0, BlackPixel(d, 0), BlackPixel(d, 0));
  XSelectInput(d, win, KeyPressMask | KeyReleaseMask | ClientMessage);
  XMapWindow(d, win);
  XFlush(d);
  XEvent event;
  Atom closeMessage = XInternAtom(d, "WM_DELETE_WINDOW", True);
  XSetWMProtocols(d, win, &closeMessage, 1);
  bool done = false;

	lcm::LCM lcmInstance;
	omnibot_speed_command_t cmd;

	long key;

  while(!done) {
    XNextEvent(d, &event);
    switch(event.type) {
      case KeyPress: {
				key = (long) XLookupKeysym(&event.xkey, 0);
        fprintf(stdout, "key #%ld was pressed.\n", key);
				std::cout << key << std::endl;
        switch(key) {
					case 'w' :
					cmd.v_x = 0.5;
					break;
					case 's' :
					cmd.v_x = -0.5;
					break;
					case 'a' :
					cmd.v_y = 0.5;
					break;
					case 'd' :
					cmd.v_y = -0.5;
					break;
					case 'q' :
					cmd.w_z = 0.5;
					break;
					case 'e' :
					cmd.w_z = -0.5;
					break;
				}
				break;
      }case KeyRelease:{
				key = (long) XLookupKeysym(&event.xkey, 0);
        if(was_it_auto_repeat(d, &event, KeyRelease, KeyPress)){
          XNextEvent(d, &event); /* Consume the extra event so we can ignore it. */
        }else{
          fprintf(stdout, "key #%ld was released.\n", (long) XLookupKeysym(&event.xkey, 0));
        }
        break;
      }case ClientMessage:{
        if ((Atom)event.xclient.data.l[0] == closeMessage) {
          done = true;
        }
        break;
      }
    }
  }
  XDestroyWindow(d, win);
  XCloseDisplay(d);
  return 0;
}