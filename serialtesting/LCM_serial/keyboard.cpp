#include <ncurses.h>
#include "lcmtypes/omnibot_speed_command_t.hpp"
#include <lcm/lcm-cpp.hpp>
#include <iostream>
#include <cassert>

int main(int argc, char** argv) {
	lcm::LCM lcmInstance;
	omnibot_speed_command_t cmd;
	initscr();
	cbreak();
	int ch;
	nodelay(stdscr, TRUE);

	bool flag1 = false;

	while (1) {
		// 13 ms
		lcmInstance.handleTimeout(40);
		if ((ch = getch()) == ERR) {
			// if (!flag1) {
			// 	cmd.v_x = 0;
			// 	cmd.v_y = 0;
			// 	cmd.w_z = 0;
			// 	lcmInstance.publish("OMNIBOT_SPEED_COMMAND", &cmd);
			// }
			// flag1 = true;
		}
		else {
			flag1 = false;
			switch(ch) {
				case 'w' :
				cmd.v_x = 0.5;
				cmd.v_y = 0;
				cmd.w_z = 0;
				break;
				case 's' :
				cmd.v_x = -0.5;
				cmd.v_y = 0;
				cmd.w_z = 0;
				break;
				case 'a' :
				cmd.v_x = 0;
				cmd.v_y = 0.5;
				cmd.w_z = 0;
				break;
				case 'd' :
				cmd.v_x = 0;
				cmd.v_y = -0.5;
				cmd.w_z = 0;
				break;
				case 'q' :
				cmd.v_x = 0;
				cmd.v_y = 0;
				cmd.w_z = 1.5;
				break;
				case 'e' :
				cmd.v_x = 0;
				cmd.v_y = 0;
				cmd.w_z = -1.5;
				break;
				case ' ' :
				cmd.v_x = 0;
				cmd.v_y = 0;
				cmd.w_z = 0;
				break;
			}
			lcmInstance.publish("OMNIBOT_SPEED_COMMAND", &cmd);
		}
	}
}