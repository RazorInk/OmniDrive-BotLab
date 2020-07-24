#include "serialLinkCommon.hpp"
#include "kinematics.hpp"
Messaging OmnibotMessaging(&sendMessageUART, &rxMsgCallback);

//LCM handler functions
class speedCommandHandler{
	private:
	Kinematics kin_;
	public:
	speedCommandHandler(Kinematics kin) : kin_(kin) {}
	void speed_command_handler(
		const lcm::ReceiveBuffer *rbuf,
		const std::string & chan,
		const omnibot_speed_command_t *lcmMsg) {

		std::cerr<<"Preparing to send speed command message to Nucleo\n";
		std::cerr<< lcmMsg->v_x <<", "<< lcmMsg->v_y <<", "<< lcmMsg->w_z << "\n";

		// float vel_a = -20*lcmMsg->v_y - 2.68*lcmMsg->w_z;
		// float vel_b = 17.32*lcmMsg->v_x + 10*lcmMsg->v_y - 2.68*lcmMsg->w_z;
		// float vel_c = -17.32*lcmMsg->v_x + 10*lcmMsg->v_y - 2.68*lcmMsg->w_z;


		Kinematics::KiwiVels kiwi_vel = 
			kin_.forwardKinematics(lcmMsg->v_x, lcmMsg->v_y, lcmMsg->w_z);
		float vel_a = kiwi_vel.va;
		float vel_b = kiwi_vel.vb;
		float vel_c = kiwi_vel.vc;

		std::cout << vel_a << ' ' << vel_b << ' ' << vel_c << '\n';

		velocityCmd velocityCmd_msg = {
			(int8_t)(vel_a/52.36*127),
			(int8_t)(vel_b/52.36*127),
			(int8_t)(vel_c/52.36*127), 0x00};
		
		printf("%d, %d, %d\n",
			(int8_t)(vel_a/52.36*100),
			(int8_t)(vel_b/52.36*100),
			(int8_t)(vel_c/52.36*100));
		Messaging::Message uartMsg;
		OmnibotMessaging.generateMessage(
			&uartMsg,
			(void*)&velocityCmd_msg,
			Messaging::VELOCITY_CMD);

		OmnibotMessaging.txMessage(&uartMsg);
	}
};

int main (int argc, char *argv[]) {
	int fd = serialOpen("/dev/serial0", 512000);
	if (fd < 0) {
		std::cout << "Error while opening device... " << "errno = " << errno << std::endl;
		perror("Something went wrong with open()");
		exit(1);
	}

	lcm::LCM lcm;
	Kinematics kin(0.05, 0.134);
	speedCommandHandler handler(kin);
	if(!lcm.good()) return 1;

	std::cerr << "we are running!!!\n" << std::endl;
	
	lcm.subscribe("OMNIBOT_SPEED_COMMAND",
	              &speedCommandHandler::speed_command_handler,
	              &handler);

	while(0 == lcm.handle()){
		// define a timeout (for erroring out) and the delay time
		usleep(1E6/80);
	}

  return 0 ;
}

