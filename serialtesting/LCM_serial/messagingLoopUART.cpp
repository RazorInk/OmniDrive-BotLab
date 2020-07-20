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

#include <lcm/lcm.h>
#include "/lcmtypes/omnibot_speed_command_t.h"

// global variables
lcm_t * lcm;

//LCM handler functions
void speed_command_handler( const lcm_recv_buf_t *rbuf,
                            const char *channel,
                            const omnibot_speed_command_t *msg,
                            void *user);

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
	int serial_fd = open("/dev/serial0", O_RDWR);
	if (serial_fd < 0) {
		std::cout << "Error while opening device... " << "errno = " << errno << std::endl;
		perror("Something went wrong with open()");
		exit(1);
	}

  	int fd ;

  	fd = serialOpen("/dev/serial0", 512000);

	std::cout << fd << std::endl;
	int fd_used = fd;

	Messaging OmnibotMessaging(&sendMessageUART, &messageReaction);

	printf("we are running!!!\n");
	
	mbot_motor_command_t_subscribe(lcm,
						OMNIBOT_VELOCITY_COMMAND_CHANNEL,
						motor_command_handler,
						NULL);
	while(1){
		// define a timeout (for erroring out) and the delay time
		lcm_handle_timeout(lcm, 1);
		usleep(1E6/80);
	}
    lcm_destroy(lcm);

  return 0 ;
}

void speed_command_handler(const lcm_recv_buf_t *rbuf, const char *channel,
                          const omnibot_motor_command_t *msg, void *user){
	float vel_a = -20*msg->v_y - 2.68*msg->w_z;
	float vel_b = 17.32*msg->v_x + 10*msg->v_y - 2.68*msg->w_z;
	float vel_c = -17.32*msg->v_x + 10*msg->v_y - 2.68*msg->w_z;
	pi2nu msg_test = {(int8_t)(vel_a/52.36*100), (int8_t)(vel_b/52.36*100), (int8_t)(vel_c/52.36*100), 0x00};
	Messaging::Message msg_to_send;
	OmnibotMessaging.generateMessage(&msg_to_send, msg_test);
	OmnibotMessaging.sendMessage(&msg_to_send);
}