#include <stdint.h>
#include <lcmtypes/omnibot_global_pid_t.hpp>
#include <lcm/lcm-cpp.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

int main(int argc, char** argv) {
	lcm::LCM lcmInstance;
	omnibot_global_pid_t pid_msg;
	pid_msg.p = 0.012;
	pid_msg.i = 0.009;
	pid_msg.d = 0.005;

	lcmInstance.publish("OMNIBOT_GLOBAL_PID", &pid_msg);
}