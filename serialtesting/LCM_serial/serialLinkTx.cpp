#include "serialLinkCommon.hpp"
#include "kinematics.hpp"
#define PI 3.14159265358979323846
Messaging OmnibotMessaging(&sendMessageUART, &rxMsgCallback);

//LCM handler functions
class SpeedCommandHandler{
	private:
	Kinematics kin_;
	float psi_;
	public:
	SpeedCommandHandler(Kinematics kin) : kin_(kin) {}
	void speed_command_handler(
		const lcm::ReceiveBuffer *rbuf,
		const std::string & chan,
		const omnibot_speed_command_t *lcmMsg) {

		std::cerr<<"Preparing to send speed command message to Nucleo\n";
		std::cerr<< lcmMsg->v_x <<", "<< lcmMsg->v_y <<", "<< lcmMsg->w_z << "\n";

		// float vel_a = -20*lcmMsg->v_y - 2.68*lcmMsg->w_z;
		// float vel_b = 17.32*lcmMsg->v_x + 10*lcmMsg->v_y - 2.68*lcmMsg->w_z;
		// float vel_c = -17.32*lcmMsg->v_x + 10*lcmMsg->v_y - 2.68*lcmMsg->w_z;
		float vx_gbl = lcmMsg->v_x;
		float vy_gbl = lcmMsg->v_y;
		float wz_gbl = lcmMsg->w_z;

		// TODO: verify this is the right transform
		float vx_lcl =   vx_gbl * cos(psi_) + vy_gbl * sin(psi_);
		float vy_lcl = - vx_gbl * sin(psi_) + vy_gbl * cos(psi_);

		Kinematics::KiwiVels kiwi_vel = 
			kin_.forwardKinematicsLocal(vx_lcl, vy_lcl, wz_gbl);
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

	void kiwi_command_handler(
		const lcm::ReceiveBuffer *rbuf,
		const std::string & chan,
		const omnibot_kiwi_command_t *lcmMsg) {
		// std::cout << "kiwi_command_handler!" << std::endl;
		velocityCmd velocityCmd_msg = {
			lcmMsg->v_a,
			lcmMsg->v_b,
			lcmMsg->v_c, 0x00};
		Messaging::Message uartMsg;
		std::cout << (uint16_t)(lcmMsg->v_a) << ' '
			<< (uint16_t)(lcmMsg->v_b) << ' ' 
			<< (uint16_t)(lcmMsg->v_c) << '\n';

		OmnibotMessaging.generateMessage(
			&uartMsg,
			(void*)&velocityCmd_msg,
			Messaging::VELOCITY_CMD);

		OmnibotMessaging.txMessage(&uartMsg);
	}

	void global_pid_handler(
		const lcm::ReceiveBuffer *rbuf,
		const std::string & chan,
		const omnibot_global_pid_t *lcmMsg) {
		// std::cout << "kiwi_command_handler!" << std::endl;
		globalPID globalPID_msg = {
			lcmMsg->p,
			lcmMsg->i,
			lcmMsg->d};
		Messaging::Message uartMsg;
		std::cout << (lcmMsg->p) << ' '
			<< (lcmMsg->i) << ' ' 
			<< (lcmMsg->d) << '\n';

		OmnibotMessaging.generateMessage(
			&uartMsg,
			(void*)&globalPID_msg,
			Messaging::GLOBAL_PID);

		OmnibotMessaging.txMessage(&uartMsg);
	}

	void odometry_handler(
		const lcm::ReceiveBuffer *rbuf,
		const std::string & chan,
		const odometry_t *lcmMsg) {
		
		psi_ = lcmMsg->psi;
	}
};

int main (int argc, char *argv[]) {
	int fd = serialOpen("/dev/serial0", UARTBAUD);
	if (fd < 0) {
		std::cout << "Error while opening device... " << "errno = " << errno << std::endl;
		perror("Something went wrong with open()");
		exit(1);
	}

	lcm::LCM lcm;
	Kinematics kin(0.05, 0.134);
	SpeedCommandHandler handler(kin);
	if(!lcm.good()) return 1;

	std::cerr << "we are running!!!\n" << std::endl;
	
	lcm.subscribe("OMNIBOT_SPEED_COMMAND",
	              &SpeedCommandHandler::speed_command_handler,
	              &handler);

	lcm.subscribe("OMNIBOT_KIWI_COMMAND",
	              &SpeedCommandHandler::kiwi_command_handler,
	              &handler);

	lcm.subscribe("OMNIBOT_GLOBAL_PID",
	              &SpeedCommandHandler::global_pid_handler,
	              &handler);

	lcm.subscribe("ODOMETRY",
	              &SpeedCommandHandler::odometry_handler,
	              &handler);

	while(0 == lcm.handle()){
		// define a timeout (for erroring out) and the delay time
		usleep(1E6/80);
	}

  return 0 ;
}

