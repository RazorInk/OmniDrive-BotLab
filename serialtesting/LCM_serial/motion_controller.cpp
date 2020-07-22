#include "lcmtypes/omnibot_speed_command_t.hpp"
#include "lcmtypes/odometry_t.hpp"
#include "lcmtypes/pose_xyt_t.hpp"
#include "lcmtypes/robot_path_t.hpp"
//#include "common/pose_trace.hpp"
#include <lcm/lcm-cpp.hpp>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cmath>
#include <signal.h>
#define PI 3.14159265358979323846

float clamp_speed(float speed)
{
    if(speed < -1.0f)
    {
        return -1.0f;
    }
    else if(speed > 1.0f)
    {
        return 1.0f;
    }

    return speed;
}
float angle_diff(float angle1, float angle2){
        float diff = angle2 - angle1;
        while(diff < -PI) diff+=2.0*PI;
        while(diff > PI) diff-=2.0*PI;
        return diff;
}


class MotionController
{
public:

	/**
	* Constructor for MotionController.
	*/
	MotionController(lcm::LCM * instance) : lcmInstance(instance)
	{
	}

	/**
	* updateCommand calculates the new motor command to send to the Mbot. This method is called after each call to
	* lcm.handle. You need to check if you have sufficient data to calculate a new command, or if the previous command
	* should just be used again until for feedback becomes available.
	*
	* \return   The motor command to send to the mbot_driver.
	*/
	omnibot_speed_command_t updateCommand(void)
	{
		const float kSlowdownDist = 0.2f; //0.26f;
		const float kDesiredSpeed = 5.0f; //0.55f;
		const float kMinSpeed = 0.02f; //0.4f;
		const float kTurnSpeed = 0.7f; //0.3f;

		omnibot_speed_command_t cmd;
		cmd.v_x = 0.0f;
		cmd.v_y = 0.0f;
		cmd.w_z = 0.0f;
		//cmd.utime = now();

		if(haveReachedTarget()) {
			std::cout << "TARGET REACHED\n";
			bool haveTarget = assignNextTarget();

			if(!haveTarget) {
					std::cout << "COMPLETED PATH!\n";
			}
		}

		if(!targets_.empty()) {
			std::cout << "NOT EMPTY\n";
			// Use feedback based on heading error for line-of-sight vector pointing to the target.
			pose_xyt_t target = targets_.back();
	    std::cout << "back\n";
			// Convert odometry to the global coordinates
			pose_xyt_t pose = currentPose();
	    std::cout << "current\n";
			double targetHeading = target.psi;
			double error = angle_diff(targetHeading, pose.psi);
			std::cout << "targetHeading: " << targetHeading << ", pose Theta: " << pose.psi << std::endl;
			std::cout << "Angle error:" << error << '\n';

	    float turnspeed = kTurnSpeed;
			std::cout << "start calculatiing\n";
	    //if the error is small, vary the turnspeed linearly with the error
			if(std::abs(error) < 0.7) turnspeed = 4 * std::abs(error);
			turnspeed = std::min(turnspeed, kTurnSpeed);//don't turn faster than the desired turnspeed

			// Turn left if the target is to the left
			if(error > 0.0)
			{
					std::cout << "Turning left\n";
					cmd.w_z = turnspeed+0.2;
			}
			// Turn right if the target is to the right
			else // if(error < 0.0)
			{
					std::cout << "Turning right\n";
					cmd.w_z = -turnspeed-0.2;
			}
	    float dX = target.x - pose.x;
	    float dY = target.y - pose.y;

	    float dx = dX * cos(pose.psi) + dY * sin(pose.psi);
	    float dy = -dX * sin(pose.psi) + dY * cos(pose.psi);
	    float v_x = kDesiredSpeed;
	    float v_y = kDesiredSpeed;

	    if(dx < kSlowdownDist) {
				v_x = kMinSpeed + sqrt(dx);
			}

	    if(dy < kSlowdownDist)
			v_y = kMinSpeed + sqrt(dy);
	    cmd.v_x = v_x;
	    cmd.v_y = v_y;
	    //v_x = std::max(0.0f, v_x);
	    //v_y = std::max(0.0f, v_y);

	    // cmd.v_x = clamp_speed(v_x);
	    // cmd.v_y = clamp_speed(v_y);
		}
		return cmd;
	}

	void handlePath(const lcm::ReceiveBuffer* buf,
		const std::string& channel,
		const robot_path_t* path) {

		targets_ = path->path;
		std::reverse(targets_.begin(), targets_.end()); // store first at back to allow for easy pop_back()

		std::cout << "received new path at time: " << path->utime << "\n";
		for(auto pose : targets_){
				std::cout << "(" << pose.x << "," << pose.y << "," << pose.psi << "); ";
		}std::cout << "\n";
		assignNextTarget();

		//confirm.utime = now();
		//confirm.creation_time = path->utime;
		//confirm.channel = channel;

		//confirm that the path was received
		//lcmInstance->publish(MESSAGE_CONFIRMATION_CHANNEL, &confirm);
	}

	void handleOdometry(const lcm::ReceiveBuffer* buf,
		const std::string& channel,
		const odometry_t* odometry) {
		odomPose.x = odometry->x;
		odomPose.y = odometry->y;
		odomPose.psi = odometry->psi;
	}

private:

	//PoseTrace  odomTrace_;              // trace of odometry for maintaining the offset estimate
	std::vector<pose_xyt_t> targets_;
	pose_xyt_t odomPose;

	lcm::LCM * lcmInstance;

	//int64_t now(){
	//    return utime_now() + time_offset;
	//}

	bool haveReachedTarget(void) {
		const float kPosTolerance = 0.1f;
		const float kFinalPosTolerance = 0.05f;

		//tolerance for intermediate waypoints can be more lenient
		float tolerance = (targets_.size() == 1) ? kFinalPosTolerance : kPosTolerance;

		// There's no target, so we're there by default.
		if(targets_.empty())
		{
				return false;
		}
		// If there's no odometry, then we're nowhere, so we couldn't be at a target
		//if(odomTrace_.empty())
		//{
		//    return false;
		//}

		pose_xyt_t target = targets_.back();
		pose_xyt_t pose = currentPose();

		float xError = std::abs(target.x - pose.x);
		float yError = std::abs(target.y - pose.y);

		return (xError < tolerance) && (yError < tolerance);
	}

	bool assignNextTarget(void) {
		// If there was a target, remove it
		if(!targets_.empty())
		{
				targets_.pop_back();
		}

		return !targets_.empty();
	}

	pose_xyt_t currentPose(void) {
		//assert(!odomTrace_.empty());

		pose_xyt_t pose;

		pose.x = odomPose.x;
		pose.y = odomPose.y;
		pose.psi = odomPose.psi;

		return pose;
	}
};


int main(int argc, char** argv) {
	lcm::LCM lcmInstance;
	MotionController controller(&lcmInstance);
	lcmInstance.subscribe("ODOMETRY", &MotionController::handleOdometry, &controller);
	lcmInstance.subscribe("CONTROLLER_PATH", &MotionController::handlePath, &controller);

	while(true) {
		lcmInstance.handleTimeout(50);  // update at 20Hz minimum

		//if(controller.timesync_initialized()){
		omnibot_speed_command_t cmd = controller.updateCommand();
		lcmInstance.publish("OMNIBOT_SPEED_COMMAND", &cmd);
			
	}

	return 0;
}
