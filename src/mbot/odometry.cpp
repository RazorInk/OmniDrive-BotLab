/*******************************************************************************
* odometry.cpp
*
* TODO: Implement these functions to add odometry and dead rekoning
*
*******************************************************************************/

#include <mbot/mbot_channels.h>
#include <mbot/mbot_defs.h>
#include <lcmtypes/odometry_t.hpp>
#include <lcmtypes/mbot_encoder_t.hpp>
#include <lcmtypes/reset_odometry_t.hpp>
#include <common/lcm_config.h>
#include <lcm/lcm-cpp.hpp>
#include <math.h>
#include <signal.h>
#include <iostream>

#define PI 3.14159265358979323846
class Odometry
{
  private:
    lcm::LCM* lcm_instance_;
    float x_, y_, theta_;
    int64_t last_time_;

  public:
    /*******************************************************************************
    * initialize_odometry()
    *
    * TODO: initialize odometry
    *
    *******************************************************************************/
    Odometry(lcm::LCM* lcm_instance) : lcm_instance_(lcm_instance), x_(0), y_(0), theta_(0), last_time_(0) {}


    /*******************************************************************************
    * handleEncoders()
    *
    * TODO: calculate odometry from internal variables
    *       publish new odometry to lcm ODOMETRY_CHANNEL
    *
    *******************************************************************************/
    void handleEncoders(const lcm::ReceiveBuffer* buf, const std::string& channel, const mbot_encoder_t* msg){
        // Skip the first encoder reading
        if (last_time_ == 0)
        {
            last_time_ = msg->utime;
            return;
        }

        // Publish odometry msg
        odometry_t odom_msg;
        odom_msg.utime = msg->utime;
        lcm_instance_->publish(ODOMETRY_CHANNEL, &odom_msg);

        // printf("x: %f\ny: %f\ntheta: %f", x_, y_, theta_);
    }


    /*******************************************************************************
    * handleEncoders()
    *
    * TODO: calculate odometry from internal variables
    *       publish new odometry to lcm ODOMETRY_CHANNEL
    *
    *******************************************************************************/
    void handleOdometryReset(const lcm::ReceiveBuffer* buf, const std::string& channel, const reset_odometry_t* msg){
        reset(msg->x, msg->y, msg->theta);
    }


    void reset(float x = 0, float y = 0, float theta = 0) {
        x_ = x;
        y_ = y;
        theta_ = clamp_radians(theta);
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
    lcm::LCM lcmInstance(MULTICAST_URL);

    Odometry odom(&lcmInstance);
    lcmInstance.subscribe(MBOT_ENCODERS_CHANNEL, &Odometry::handleEncoders, &odom);
    lcmInstance.subscribe(RESET_ODOMETRY_CHANNEL, &Odometry::handleOdometryReset, &odom);

    signal(SIGINT, exit);

    while(true)
    {
        lcmInstance.handleTimeout(50);  // update at 20Hz minimum


    }

    return 0;
}
