/*******************************************************************************
* odometry.cpp
*
* TODO: Implement these functions to add odometry and dead rekoning
*
*******************************************************************************/

#include "lcmtypes/odometry_t.hpp"
#include "lcmtypes/omnibot_encoder_t.hpp"
#include <lcm/lcm-cpp.hpp>
#include <math.h>
#include <signal.h>
#include <iostream>
#include "kinematics.hpp"

#define PI 3.14159265358979323846
#define WHEEL_DIAMETER 0.1
#define WHEEL_RADIUS 0.05
#define ROBOT_RADIUS 0.134
#define GEAR_RATIO 20.4
#define ENCODER_RES 48
#define DT 0.0125

class Odometry
{
  private:
    lcm::LCM* lcm_instance_;
		Kinematics kin_;
    float x_, y_, psi_;
    int64_t last_time_;

  public:
    /*******************************************************************************
    * initialize_odometry()
    *
    * TODO: initialize odometry
    *
    *******************************************************************************/
    Odometry(lcm::LCM* lcm_instance, Kinematics kin) :
			lcm_instance_(lcm_instance), kin_(kin),
			x_(0), y_(0), psi_(0), last_time_(0) {}


    /*******************************************************************************
    * handleEncoders()
    *
    * TODO: calculate odometry from internal variables
    *       publish new odometry to lcm ODOMETRY_CHANNEL
    *
    *******************************************************************************/
    void handleEncoders(const lcm::ReceiveBuffer* buf,
			const std::string& channel, const omnibot_encoder_t* msg){
			// Skip the first encoder reading
			if (last_time_ == 0) {
					last_time_ = msg->utime;
					return;
			}

			float dx, dy, dpsi;

			float enc2meters = (WHEEL_DIAMETER * PI) / (GEAR_RATIO * ENCODER_RES);

			float va = enc2meters*msg->a_delta;
			float vb = enc2meters*msg->b_delta;
			float vc = enc2meters*msg->c_delta;

			Kinematics::CartesianVels cart_vel = kin_.inverseKinematics(va, vb, vc);

			dx = cart_vel.vx;
			dy = cart_vel.vy;
			dpsi = cart_vel.wz;

			float angle1 = clamp_radians(psi_ + dpsi/2.0f);
			float angle2 = clamp_radians(psi_ + dpsi/2.0f + PI/2);

			x_ += dx * cos(angle1) + dy * cos(angle2);
			y_ += dx * sin(angle2) + dy * sin(angle1);
			psi_ =  clamp_radians(psi_ + dpsi);

			// Publish odometry msg
			odometry_t odom_msg;
			odom_msg.utime = msg->utime;
			odom_msg.v_x = dx / DT;
			odom_msg.v_y = dy / DT;
			odom_msg.w_z = dpsi / DT;
			
			odom_msg.x = x_;
			odom_msg.y = y_;
			odom_msg.psi = psi_;
			lcm_instance_->publish("ODOMETRY", &odom_msg);
    }


  private:
    /*******************************************************************************
    * clamp_radians()
    *******************************************************************************/
    float clamp_radians(float angle){

			if(angle < -PI) {
					for(; angle < -PI; angle += 2.0*PI);
			}
			else if(angle > PI) {
					for(; angle > PI; angle -= 2.0*PI);
			}

			return angle;
    }

    float angle_diff_radians(float angle1, float angle2){
			float diff = angle2 - angle1;
			while(diff < -PI) diff+=2.0*PI;
			while(diff > PI) diff-=2.0*PI;
			return diff;
    }
};

int main(int argc, char** argv)
{
	lcm::LCM lcmInstance;
	Kinematics kin(WHEEL_RADIUS, ROBOT_RADIUS);

	Odometry odom(&lcmInstance, kin);
	lcmInstance.subscribe("OMNIBOT_ENCODERS", &Odometry::handleEncoders, &odom);

	signal(SIGINT, exit);

	while(true)
	{
			lcmInstance.handleTimeout(50);  // update at 20Hz minimum

	}

	return 0;
}
