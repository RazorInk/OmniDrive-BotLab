#include <mbot/mbot_channels.h>
#include <common/timestamp.h>
#include <lcmtypes/mbot_motor_command_t.hpp>
#include <lcmtypes/odometry_t.hpp>
#include <lcmtypes/pose_xyt_t.hpp>
#include <lcmtypes/robot_path_t.hpp>
#include <lcmtypes/timestamp_t.hpp>
#include <lcmtypes/message_received_t.hpp>
#include <common/angle_functions.hpp>
#include <common/pose_trace.hpp>
#include <common/lcm_config.h>
#include <slam/slam_channels.h>
#include <lcm/lcm-cpp.hpp>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <signal.h>


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


class MotionController
{
public:

    /**
    * Constructor for MotionController.
    */
    MotionController(lcm::LCM * instance) : lcmInstance(instance)
    {
        ////////// TODO: Initialize your controller state //////////////

        // Initially, there's no offset between odometry and the global state
        odomToGlobalFrame_.x = 0.0f;
        odomToGlobalFrame_.y = 0.0f;
        odomToGlobalFrame_.theta = 0.0f;

        time_offset = 0;
        timesync_initialized_ = false;

        confirm.utime = 0;
        confirm.creation_time = 0;
        confirm.channel = "";
    }

    /**
    * updateCommand calculates the new motor command to send to the Mbot. This method is called after each call to
    * lcm.handle. You need to check if you have sufficient data to calculate a new command, or if the previous command
    * should just be used again until for feedback becomes available.
    *
    * \return   The motor command to send to the mbot_driver.
    */
    mbot_motor_command_t updateCommand(void)
    {
        //////////// TODO: Implement your feedback controller here. //////////////////////

        mbot_motor_command_t cmd;
        cmd.trans_v = 0.0f;
        cmd.angular_v = 0.0f;
        cmd.utime = now();

        if(haveReachedTarget())
        {
            std::cout << "TARGET REACHED\n";
            bool haveTarget = assignNextTarget();

            if(!haveTarget)
            {
                std::cout << "COMPLETED PATH!\n";
            }
        }

        // Use feedback based on heading error for line-of-sight vector pointing to the target.
        pose_xyt_t target = targets_.back();
        // Convert odometry to the global coordinates
        pose_xyt_t pose = currentPose();


        return cmd;
    }

    bool timesync_initialized(){ return timesync_initialized_; }

    void handleTimesync(const lcm::ReceiveBuffer* buf, const std::string& channel, const timestamp_t* timesync){
        timesync_initialized_ = true;
        time_offset = timesync->utime-utime_now();
    }

    void handlePath(const lcm::ReceiveBuffer* buf, const std::string& channel, const robot_path_t* path)
    {
        /////// TODO: Implement your handler for new paths here ////////////////////

        // targets_ = ...

        std::cout << "received new path at time: " << path->utime << "\n";
        for(auto pose : targets_){
            std::cout << "(" << pose.x << "," << pose.y << "," << pose.theta << "); ";
        }std::cout << "\n";

        assignNextTarget();

        confirm.utime = now();
        confirm.creation_time = path->utime;
        confirm.channel = channel;

        //confirm that the path was received
        lcmInstance->publish(MESSAGE_CONFIRMATION_CHANNEL, &confirm);
    }

    void handleOdometry(const lcm::ReceiveBuffer* buf, const std::string& channel, const odometry_t* odometry)
    {
        /////// TODO: Implement your handler for new odometry data ////////////////////

        pose_xyt_t pose;
        odomTrace_.addPose(pose);
    }

    void handlePose(const lcm::ReceiveBuffer* buf, const std::string& channel, const pose_xyt_t* pose)
    {
        computeOdometryOffset(*pose);
    }

private:

    enum State
    {
        TURN,
        DRIVE,
    };

    pose_xyt_t odomToGlobalFrame_;      // transform to convert odometry into the global/map coordinates for navigating in a map
    PoseTrace  odomTrace_;              // trace of odometry for maintaining the offset estimate
    std::vector<pose_xyt_t> targets_;

    // State of the motion controller
    State state_;

    int64_t time_offset;

    bool timesync_initialized_;

    message_received_t confirm;
    lcm::LCM * lcmInstance;

    int64_t now(){
        return utime_now() + time_offset;
    }

    bool haveReachedTarget(void)
    {
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
        if(odomTrace_.empty())
        {
            return false;
        }

        pose_xyt_t target = targets_.back();
        pose_xyt_t pose = currentPose();

        float xError = std::abs(target.x - pose.x);
        float yError = std::abs(target.y - pose.y);

        return (xError < tolerance) && (yError < tolerance);
    }

    bool assignNextTarget(void)
    {
        // If there was a target, remove it
        if(!targets_.empty())
        {
            targets_.pop_back();
        }

        // TODO: Reset all error and state terms when switching to a new target

        return !targets_.empty();
    }

    void computeOdometryOffset(const pose_xyt_t& globalPose)
    {
        /////// TODO: Implement your handler for new pose data ////////////////////
        odomToGlobalFrame_.x = 0;
        odomToGlobalFrame_.y = 0;
        odomToGlobalFrame_.theta = 0;
    }

    pose_xyt_t currentPose(void)
    {
        assert(!odomTrace_.empty());

        pose_xyt_t odomPose = odomTrace_.back();
        pose_xyt_t pose;
        // TODO: Implement transform from odom frame to slam frame
        return pose;
    }
};


int main(int argc, char** argv)
{
    lcm::LCM lcmInstance(MULTICAST_URL);

    MotionController controller(&lcmInstance);
    lcmInstance.subscribe(ODOMETRY_CHANNEL, &MotionController::handleOdometry, &controller);
    lcmInstance.subscribe(SLAM_POSE_CHANNEL, &MotionController::handlePose, &controller);
    lcmInstance.subscribe(CONTROLLER_PATH_CHANNEL, &MotionController::handlePath, &controller);
    lcmInstance.subscribe(MBOT_TIMESYNC_CHANNEL, &MotionController::handleTimesync, &controller);

    signal(SIGINT, exit);

    while(true)
    {
        lcmInstance.handleTimeout(50);  // update at 20Hz minimum

        if(controller.timesync_initialized()){
            mbot_motor_command_t cmd = controller.updateCommand();

            lcmInstance.publish(MBOT_MOTOR_COMMAND_CHANNEL, &cmd);
        }
    }

    return 0;
}
