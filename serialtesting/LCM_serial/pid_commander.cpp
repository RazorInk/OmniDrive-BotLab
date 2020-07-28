#include <stdint.h>
#include "lcmtypes/omnibot_kiwi_command_t.hpp"
#include <lcm/lcm-cpp.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

int main(int argc, char** argv) {
	lcm::LCM lcmInstance;
	omnibot_kiwi_command_t cmd;
	char ch;
	int16_t speed;
	std::string input = "";

	while (1) {
		std::cout << "enter <'a', 'b', 'c'> for motor ID or 's' to stop: ";
		getline(std::cin, input);
		if (input.length() == 1) {
			ch = input[0];
		}
		else {
			std::cout << "that didn't work!" << std::endl;
			break;
		}
		std::cout << std::endl;
		if (ch == 's') {
			cmd.v_a = 0;
			cmd.v_b = 0;
			cmd.v_c = 0;
			lcmInstance.publish("OMNIBOT_KIWI_COMMAND", &cmd);
		}
		else {
			std::cout << "enter speed between -128 and 127: ";
			getline(std::cin, input);
			// This code converts from string to number safely.
			std::stringstream myStream(input);
			if (myStream >> speed) {}
			else {
				std::cout << "that didn't work!" << std::endl;
				break;
			}
			if (speed > 127 || speed < -128) {
				std::cout << "command out of bounds" << std::endl;
				break;
			}
			std::cout << speed << std::endl << std::endl;
			switch(ch) {
				case 'a' :
				cmd.v_a = speed;
				cmd.v_b = 0;
				cmd.v_c = 0;
				break;
				case 'b' :
				cmd.v_a = 0;
				cmd.v_b = speed;
				cmd.v_c = 0;
				break;
				case 'c' :
				cmd.v_a = 0;
				cmd.v_b = 0;
				cmd.v_c = speed;
				break;
			}
			lcmInstance.publish("OMNIBOT_KIWI_COMMAND", &cmd);
		}
	}
}