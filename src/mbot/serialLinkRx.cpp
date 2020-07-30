#include <common/serialLinkCommon.hpp>
Messaging OmnibotMessaging(&sendMessageUART, &rxMsgCallback);


int main (int argc, char *argv[]) {
	lcm::LCM lcmInstance;
	uint8_t sdata_out[64];
	int fd = serialOpen("/dev/serial0", UARTBAUD);
	if (fd < 0) {
		std::cout << "Error while opening device... " << "errno = " << errno << std::endl;
		perror("Something went wrong with open()");
		exit(1);
	}

	if(!lcmInstance.good()) return 1;

	printf("we are running!!!\n");
	
	while(1){
		if (serialDataAvail(fd) == -1) {std::cerr << errno << std::endl;;}

		while (serialDataAvail(fd) < 64) {}
		// std::cout << "here" << std::endl;
		read (fd, sdata_out, 64);
		// for (int i = 0; i < 64; ++i) std::cout << (int)sdata_out[i] << ' ';
		// std::cout << std::endl;
		OmnibotMessaging.rxMessageSequence(sdata_out);
		lcmInstance.handleTimeout(5);
	}

  return 0 ;
}

