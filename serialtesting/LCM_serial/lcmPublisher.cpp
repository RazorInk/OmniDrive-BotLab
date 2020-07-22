#include "lcmtypes/omnibot_speed_command_t.hpp"
#include <lcm/lcm-cpp.hpp>
#include <iostream>
#include <cassert>


int main(int argc, char** argv)
{
	lcm::LCM lcmInstance;
	int time = 0;
	while(true)
	{
		lcmInstance.handleTimeout(500);  // higher larger rec

		omnibot_speed_command_t cmd;
		switch(time){
		case 0:
			cmd.v_x = 1;
			cmd.v_y = 0;
			cmd.w_z = 0;
			time ++;
			break;
		case 1:
			cmd.v_x = 0;
			cmd.v_y = 0;
			cmd.w_z = 0;
			time ++;
			break;
		case 2:
			cmd.v_x = 0;
			cmd.v_y = 1;
			cmd.w_z = 0;
			time ++;
			break;
		case 3:
			cmd.v_x = 0;
			cmd.v_y = 0;
			cmd.w_z = 0;
			time ++;
			break;
		case 4:
			cmd.v_x = -1;
			cmd.v_y = 0;
			cmd.w_z = 0;
			time ++;
			break;
		case 5:
			cmd.v_x = 0;
			cmd.v_y = 0;
			cmd.w_z = 0;
			time ++;
			break;
		case 6:
			cmd.v_x = 0;
			cmd.v_y = -1;
			cmd.w_z = 0;
			time++;
			break;
		case 7:
			cmd.v_x = 0;
			cmd.v_y = 0;
			cmd.w_z = 0;
			time = 0;
			break;
		}

					lcmInstance.publish("OMNIBOT_SPEED_COMMAND", &cmd);
		std::cout<< "Published!\n";
	}

	return 0;
}
