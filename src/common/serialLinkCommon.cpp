#include <common/serialLinkCommon.hpp>

extern int errno;

void sendMessageUART(Messaging::Message* msg_buf) {
	int fd = serialOpen("/dev/serial0", UARTBAUD);
	write (fd, (uint8_t*)msg_buf, 64);
	// serialFlush(fd);
	serialClose(fd);
	return;
}

void rxMsgCallback(Messaging::Message &msg) {
	lcm::LCM lcmInstance;
	switch(msg.msgType) {
		case Messaging::VELOCITY_CMD:          {
		}break;
		case Messaging::MANUAL_CMD:            {
		}break;
		case Messaging::HEARTBEAT:             {
		}break;
		case Messaging::STATE_TRANSITION:      {
		}break;
		case Messaging::MOTOR_REMAP:           {
		}break;
		case Messaging::GLOBAL_PID:            {
		}break;
		case Messaging::SINGLE_PID:            {
		}break;
		case Messaging::NULLMSG:               {
		}break;
		case Messaging::NUCLEO_GENERAL_UPDATE: {
			nucleoGeneralUpdate nucleoGeneralUpdate_data = 
				*((nucleoGeneralUpdate*)(msg.msgData));
			omnibot_encoder_t encoder;
			// omnibot_imu_t imu;
			// std::cout << "here2" << std::endl;

			encoder.a_delta = nucleoGeneralUpdate_data.a_delta;
			encoder.b_delta = nucleoGeneralUpdate_data.b_delta;
			encoder.c_delta = nucleoGeneralUpdate_data.c_delta;

			// std::copy(
			// 	std::begin(nucleoGeneralUpdate_data.gyro),
			// 	std::end(nucleoGeneralUpdate_data.gyro), std::begin(imu.gyro));
			// std::copy(
			// 	std::begin(nucleoGeneralUpdate_data.accel),
			// 	std::end(nucleoGeneralUpdate_data.accel), std::begin(imu.accel));
			// std::copy(
			// 	std::begin(nucleoGeneralUpdate_data.mag),
			// 	std::end(nucleoGeneralUpdate_data.mag), std::begin(imu.mag));

			lcmInstance.publish("OMNIBOT_ENCODERS", &encoder);
			// lcmInstance.publish("OMNIBOT_IMU", &imu);
		}break;
		default:                               {
		}break;
	}
	return;
}
