#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#include "customSerial.h"
#include "messaging.hpp"

extern int errno;

Messaging::Message temp;

void sendMessageUART(Messaging::Message* msg_buf) {
	int fd = serialOpen("/dev/serial0", 512000);
	write (fd, (uint8_t*)msg_buf, 64);
	return;
}

void messageReaction(Messaging::Message &msg) {
	temp = msg;
	return;
}

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

	Messaging OmnibotMessaging(&sendMessageUART, &messageReaction);

	uint8_t sdata_in[64];
	uint8_t sdata_out[64];
	uint8_t x;
	int j = 0;
  int8_t byte;

	long long int chunk_in;
	long long int chunk_out;

	size_t numTests = atoi(argv[1]);
	size_t numFails = 0;

  while (j < numTests)
  {
		srand(j); //i.e. non-associated value such as time (time(NULL) from ctime/time.h)
		byte = rand() & 0xFF;
		pi2nu msg_test = {byte, byte, byte, byte};
		Messaging::Message msg_to_send;
		OmnibotMessaging.generateMessage(&msg_to_send, msg_test);
		OmnibotMessaging.sendMessage(&msg_to_send);

		if (serialDataAvail(fd_used) == -1) {std::cerr << errno << std::endl;;}

		while (serialDataAvail(fd_used) < 64) {}
		read (fd_used, sdata_out, 64);
		OmnibotMessaging.rxMessageSequence(sdata_out);
		if(msg_to_send.msgHash != temp.msgHash) numFails++;
		std::cout << std::dec << j << " " << std::hex << msg_to_send.msgHash
		          << " " << temp.msgHash << std::endl;
		++j;
  }

	std::cout << std::dec << "\nfailure rate: " << numFails << "/" << numTests << std::endl;

  return 0 ;
}
