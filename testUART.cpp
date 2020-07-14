#include <stdio.h>
#include <string.h>
#include <iostream>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int fd ;
  int count ;

  fd = serialOpen ("/dev/ttyACM0", 921600);
  wiringPiSetup ();
  
  for (count = 0 ; count < 10 ; count++)
  {
    while (serialDataAvail (fd))
    {
	std::cout << serialGetchar (fd) << ' ' ;
    	fflush (stdout) ;
    }
    std::cout << '\n';
  }

  return 0 ;
}
