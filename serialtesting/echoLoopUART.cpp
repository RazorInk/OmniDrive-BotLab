#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <time.h>

// #include <wiringPi.h>
// #include <wiringSerial.h>
#include "customSerial.h"
#include <errno.h>
#include <fcntl.h>

#include <unistd.h>

extern int errno;

int main (int argc, char *argv[])
{
	if (argc != 2) {
		std::cout << "no inputs or too many inputs. specify number of tests." << std::endl;
		exit(1);
	}
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

	// std::ofstream outfile;
  // outfile.open("cpp_output.txt");
	uint8_t sdata_in[64];
	uint8_t sdata_out[64];
	uint8_t x;
	int j = 0;
  uint8_t byte;

	long long int chunk_in;
	long long int chunk_out;

	size_t numTests = atoi(argv[1]);
	size_t numFails = 0;

  while (j < numTests)
  {
		for (int i = 0; i < 64; ++i) {
			srand(time(NULL)); //i.e. non-associated value such as time (time(NULL) from ctime/time.h)
			byte = rand() & 0xFF;
			sdata_in[i] = byte;
		}
		write (fd_used, sdata_in, 64);

		if (serialDataAvail(fd_used) == -1) {std::cerr << errno << std::endl;;}

		while (serialDataAvail(fd_used) < 64) {}

		read (fd_used, sdata_out, 64);
		std::cout << std::dec << j << std::endl;
		++j;
		// for (int i = 0; i < 64; ++i) if (sdata_out[i] != sdata_in[i]) numFails++; break;
		for (int i = 0; i < 8; ++i) {
			chunk_in = *((long long int*)(sdata_in + i*8));
			chunk_out = *((long long int*)(sdata_out + i*8));
			if ((chunk_in^chunk_out) > 0) numFails++; break;
		}
    // std::cout << '\n';
  }

	std::cout << "\nfailure rate: " << numFails << "/" << numTests << std::endl;

  return 0 ;
}
