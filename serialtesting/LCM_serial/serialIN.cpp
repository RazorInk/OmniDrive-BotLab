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

//#include <lcm/lcm.h>
#include <lcm/lcm-cpp.hpp>
#include "lcmtypes/omnibot_encoder_t.hpp"
#include "lcmtypes/omnibot_imu_t.hpp"
#include "nu2pi.h"

extern int errno;

Messaging::Message temp;
lcm::LCM lcmInstance;

void sendMessageUART(Messaging::Message* msg_buf) {
	int fd = serialOpen("/dev/serial0", 512000);
	write (fd, (uint8_t*)msg_buf, 64);
	return;
}

void messageReaction(Messaging::Message &msg) {
	nu2pi *temp = (nu2pi*)&(msg.msgData);
	omnibot_encoder_t encoder;
	omnibot_imu_t imu;

	encoder.a_delta = temp->a_delta;
	encoder.b_delta = temp->b_delta;
	encoder.c_delta = temp->c_delta;

	std::copy(std::begin(temp->gyro), std::end(temp->gyro), std::begin(imu.gyro));
	std::copy(std::begin(temp->accel), std::end(temp->accel), std::begin(imu.accel));
	std::copy(std::begin(temp->mag), std::end(temp->mag), std::begin(imu.mag));


	lcmInstance.publish("OMNIBOT_ENCODER", &encoder);
	lcmInstance.publish("OMNIBOT_IMU", &imu);
	return;
}
Messaging OmnibotMessaging(&sendMessageUART, &messageReaction);


int main (int argc, char *argv[])
{
	uint8_t sdata_out[64];
	int serial_fd = open("/dev/serial0", O_RDWR);
	if (serial_fd < 0) {
		std::cout << "Error while opening device... " << "errno = " << errno << std::endl;
		perror("Something went wrong with open()");
		exit(1);
	}

  	int fd ;

  	fd = serialOpen("/dev/serial0", 512000);
	if(!lcmInstance.good())
            return 1;


	std::cout << fd << std::endl;
	int fd_used = fd;



	printf("we are running!!!\n");
	
	while(0 == lcmInstance.handle()){
		// define a timeout (for erroring out) and the delay time
		if (serialDataAvail(fd_used) == -1) {std::cerr << errno << std::endl;;}

		while (serialDataAvail(fd_used) < 64) {}
		read (fd_used, sdata_out, 64);
		OmnibotMessaging.rxMessageSequence(sdata_out);

		//usleep(1E6/80);
	}

  return 0 ;
}

