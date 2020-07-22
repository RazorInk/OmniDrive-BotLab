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
    float x_, y_, psi_;
    int64_t last_time_;

  public:
    /*******************************************************************************
    * initialize_odometry()
    *
    * TODO: initialize odometry
    *
    *******************************************************************************/
    Odometry(lcm::LCM* lcm_instance) : lcm_instance_(lcm_instance), x_(0), y_(0), psi_(0), last_time_(0) {}


    /*******************************************************************************
    * handleEncoders()
    *
    * TODO: calculate odometry from internal variables
    *       publish new odometry to lcm ODOMETRY_CHANNEL
    *
    *******************************************************************************/
    void handleEncoders(const lcm::ReceiveBuffer* buf, const std::string& channel, const omnibot_encoder_t* msg){
        // Skip the first encoder reading
        if (last_time_ == 0)
        {
            last_time_ = msg->utime;
            return;
        }

	float dx, dy, dpsi;

	float enc2meters = (WHEEL_DIAMETER * PI) / (GEAR_RATIO * ENCODER_RES);

	dx = WHEEL_RADIUS*(msg->b_delta/sqrt(3)-msg->c_delta/sqrt(3));
	dy = WHEEL_RADIUS*(-2*msg->a_delta/3 + msg->b_delta/3 + msg->c_delta/3);
	dpsi = WHEEL_RADIUS/ROBOT_RADIUS*(-msg->a_delta/3 - msg->b_delta/3 - msg->c_delta/3);
	
	x_ += dx * cos(clamp_radians(psi_ + dpsi/2.0f)) + dy * cos(clamp_radians(psi_ + dpsi/2.0f + PI/2));
	y_ += dx * sin(clamp_radians(psi_ + dpsi/2.0f + PI/2)) + dy * sin(clamp_radians(psi_ + dpsi/2.0f));
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

        // printf("x: %f\ny: %f\ntheta: %f", x_, y_, theta_);
    }


  private:
    /*******************************************************************************
    * clamp_radians()
    *******************************************************************************/
    float clamp_radians(float angle){

        if(angle < -PI)
        {
            for(; angle < -PI; angle += 2.0*PI);
        }
        else if(angle > PI)
        {
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

    Odometry odom(&lcmInstance);
    lcmInstance.subscribe("OMNIBOT_ENCODERS", &Odometry::handleEncoders, &odom);

    signal(SIGINT, exit);

    while(true)
    {
        lcmInstance.handleTimeout(50);  // update at 20Hz minimum

    }

    return 0;
}
