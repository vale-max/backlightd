#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/extensions/dpms.h>
#include <chrono>
#include <thread>
#include <csignal>
 
using namespace std;
using namespace std::chrono;
 
void signalHandler (int signum) 
{
 
    syslog(LOG_INFO,"backlightd: stopped");
    exit(signum);
}  

void backlight_on()
{

    int fd = open("/sys/class/backlight/soc:backlight/bl_power", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/backlight/soc:backlight/bl_power");
        exit(1);
    }

    if (write(fd, "0", 1) != 1) {
        perror("Error writing to /sys/class/backlight/soc:backlight/bl_power");
        exit(1);
    }

    close(fd);
    syslog(LOG_INFO,"backlightd: set backlight on"); 
//    fprintf(stderr, "Set backlight on\n"); 
}
 
void backlight_off()
{
   int fd = open("/sys/class/backlight/soc:backlight/bl_power", O_WRONLY);
    if (fd == -1) {
        perror("Unable to open /sys/class/backlight/soc:backlight/bl_power");
        exit(1);
    }

    if (write(fd, "4", 1) != 1) {
        perror("Error writing to /sys/class/backlight/soc:backlight/bl_power");
        exit(1);
    }

    close(fd);
    syslog(LOG_INFO,"backlightd: set backlight off"); 
//   fprintf(stderr, "backlightd: set backlight off\n");
}
 
int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);
    Display *dpy;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL)
    {
        fprintf(stderr, "%s:  unable to open display\n", argv[0]);
        exit(EXIT_FAILURE);
    }
 
    BOOL prev = true;
    BOOL onoff;
    CARD16 state;
    syslog(LOG_INFO,"backlightd: started"); 
    backlight_on();
    while(true)
    {
        DPMSInfo(dpy, &state, &onoff);
        switch (onoff)
        {
           case false:
              if (prev == false)
                 {
                    backlight_on();
                    prev=true;
                 }
              break;
           case true:
              switch (state)
              {
                 case DPMSModeOn:
                     if (prev == false)
                        {
                          backlight_on();
                          prev=true;
                        }
                     break;
                 case DPMSModeSuspend:    
                 case DPMSModeStandby:    
                 case DPMSModeOff:
                     if (prev == true)
                        {
                          backlight_off();
                          prev=false;
                        }
                     break;
                 default:
                     break;
              }
              break;
           default:
              break;
        }
        this_thread::sleep_for(milliseconds(200));
    }
    return 0;
}

