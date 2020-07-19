#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <errno.h>

// #include <wiringPi.h>
// #include <wiringSerial.h>
#include "customSerial.h"
#include <errno.h>
#include <fcntl.h>

#include <unistd.h>

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

  fd = serialOpen("/dev/serial0", 512000);

	std::cout << serial_fd << std::endl;
	std::cout << fd << std::endl;
	std::cout << std::hex << std::endl;
	// return 0;

	// int fd_used = serial_fd;
	int fd_used = fd;

	std::ofstream outfile;
  outfile.open("cpp_output.txt");

	// serialFlush(fd);
  // wiringPiSetup();
	uint8_t sdata[64];
	uint8_t x ;
	int j = 0;
  
  while (true)
  {
		if (serialDataAvail(fd_used) == -1) {std::cerr << errno << std::endl;;}
    if (serialDataAvail(fd_used) >= 64)
    {
			for (int i = 0; i < 64; ++i) {
				// sdata[i] = static_cast<uint8_t>(serialGetchar(fd_used));
  			read (fd_used, &x, 1);
				// if (x == 0) std::cerr << "found a 0! \n" << std::endl;
				sdata[i] = x;
				// std::cout << std::hex << +static_cast<uint8_t>(serialGetchar(fd_used));
				// std::cout << "0x" << std::hex << +sdata[i] << " ";
				outfile << "0x" << std::hex << +sdata[i] << " ";
			}
			// std::cout << std::endl;
			outfile << std::endl;
			std::cout << std::dec << j << std::endl;
			++j;
    	// fflush (stdout) ;
    }
    // std::cout << '\n';
  }

  return 0 ;
}
