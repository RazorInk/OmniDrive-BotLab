#include "serialLinkCommon.hpp"
Messaging OmnibotMessaging(&sendMessageUART, &rxMsgCallback);


int main (int argc, char *argv[]) {
	lcm::LCM lcmInstance;
	uint8_t sdata_out[64];
	int fd = serialOpen("/dev/serial0", 512000);
	if (fd < 0) {
		std::cout << "Error while opening device... " << "errno = " << errno << std::endl;
		perror("Something went wrong with open()");
		exit(1);
	}

	if(!lcmInstance.good()) return 1;

	printf("we are running!!!\n");
	
	while(0 == lcmInstance.handle()){
		if (serialDataAvail(fd) == -1) {std::cerr << errno << std::endl;;}

		while (serialDataAvail(fd) < 64) {}
		read (fd, sdata_out, 64);
		OmnibotMessaging.rxMessageSequence(sdata_out);
	}

  return 0 ;
}

