#include <ncurses.h>
#include <math.h>
#include <lcmtypes/omnibot_speed_command_t.hpp>
#include <lcm/lcm-cpp.hpp>
#include <iostream>
#include <cassert>

#define EPSILON 0.01f

bool floatEqual(double a, double b);

int main(int argc, char** argv) {
	float lin_speed = 1.6;
	float rot_speed = 2.0;
	if(argc != 3) {
		std::cout << "not enough arguments.\n" << "usage: ./keyboard <lin_speed> <rot_speed>" << std::endl;
		exit(1);
	}
	lin_speed = std::stof(argv[1]);
	rot_speed = std::stof(argv[2]);
	std::cout << "using " << lin_speed << " m/s and " << rot_speed << " rad/s" << std::endl;
	lcm::LCM lcmInstance;
	omnibot_speed_command_t cmd = {0,0,0};
	initscr();
	cbreak();
	int ch;
	nodelay(stdscr, TRUE);

	bool flag1 = false;

	while (1) {
		lcmInstance.handleTimeout(10); // ms
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
				cmd.v_x = (floatEqual(cmd.v_x, lin_speed)) ? 0 : lin_speed;
				break;
				case 's' :
				cmd.v_x = (floatEqual(cmd.v_x, -lin_speed)) ? 0 : -lin_speed;
				break;
				case 'a' :
				cmd.v_y = (floatEqual(cmd.v_y, lin_speed)) ? 0 : lin_speed;
				break;
				case 'd' :
				cmd.v_y = (floatEqual(cmd.v_y, -lin_speed)) ? 0 : -lin_speed;
				break;
				case 'q' :
				cmd.w_z = (floatEqual(cmd.w_z, rot_speed)) ? 0 : rot_speed;
				break;
				case 'e' :
				cmd.w_z = (floatEqual(cmd.w_z, -rot_speed)) ? 0 : -rot_speed;
				break;
				case 'f' :
				cmd.v_x = lin_speed;
				cmd.v_y = 0;
				cmd.w_z = rot_speed;
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

bool floatEqual(double a, double b) {
    return fabs(a - b) < EPSILON;
}