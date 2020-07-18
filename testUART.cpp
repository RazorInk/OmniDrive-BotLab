#include <stdio.h>
#include <string.h>
#include <iostream>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <fcntl.h>

extern int errno;

int main ()
{

	int serial_fd = open("/dev/serial0", O_RDWR);
	if (serial_fd < 0) {
		std::cout << "Error while opening device... " << "errno = " << errno << std::endl;
		perror("Something went wrong with open()");
		exit(1);
	}

  int fd ;
  int count ;

  fd = serialOpen("/dev/serial0", 921600);

	std::cout << serial_fd << std::endl;
	std::cout << fd << std::endl;
	// return 0;

	int fd_used = fd;

	// serialFlush(fd);
  wiringPiSetup();
  
  while (true)
  {
		if (serialDataAvail(fd_used) == -1) {std::cerr << errno << std::endl;;}
    while (serialDataAvail(fd_used) > 0)
    {
			std::cout << (char)serialGetchar(fd_used) << ' ' ;
    	fflush (stdout) ;
    }
    // std::cout << '\n';
  }

  return 0 ;
}
