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
#include "lcmtypes/omnibot_speed_command_t.hpp"



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
Messaging OmnibotMessaging(&sendMessageUART, &messageReaction);

//LCM handler functions
class speedCommandHandler{
	public:
	void speed_command_handler(const lcm::ReceiveBuffer *rbuf,
                            const std::string & chan,
                            const omnibot_speed_command_t *msg){
		std::cout<<"Hi\n";
		float vel_a = -20*msg->v_y - 2.68*msg->w_z;
		float vel_b = 17.32*msg->v_x + 10*msg->v_y - 2.68*msg->w_z;
		float vel_c = -17.32*msg->v_x + 10*msg->v_y - 2.68*msg->w_z;
		std::cout << vel_a << ' ' << vel_b << ' ' << vel_c << '\n';
		pi2nu msg_test = {(int8_t)(vel_a/52.36*127), (int8_t)(vel_b/52.36*127), (int8_t)(vel_c/52.36*127), 0x00};
		printf("%d, %d, %d\n",(int8_t)(vel_a/52.36*100), (int8_t)(vel_b/52.36*100), (int8_t)(vel_c/52.36*100));
		Messaging::Message msg_to_send;
		OmnibotMessaging.generateMessage(&msg_to_send, msg_test);
		OmnibotMessaging.sendMessage(&msg_to_send);
	}
};

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
	lcm::LCM lcm;
	speedCommandHandler handler;
	if(!lcm.good())
            return 1;


	std::cout << fd << std::endl;
	int fd_used = fd;



	printf("we are running!!!\n");
	
	lcm.subscribe("OMNIBOT_SPEED_COMMAND",&speedCommandHandler::speed_command_handler,&handler);	
	while(0 == lcm.handle()){
		// define a timeout (for erroring out) and the delay time
		usleep(1E6/80);
	}

  return 0 ;
}

