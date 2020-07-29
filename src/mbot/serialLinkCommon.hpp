#ifndef INC_SERIALLINKCOMMON_HPP_
#define INC_SERIALLINKCOMMON_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#include <common/customSerial.h>
#include <messaging/messaging.hpp>

#include <lcm/lcm-cpp.hpp>
#include <lcmtypes/omnibot_encoder_t.hpp>
#include <lcmtypes/omnibot_imu_t.hpp>
#include <lcmtypes/omnibot_speed_command_t.hpp>
#include <lcmtypes/omnibot_kiwi_command_t.hpp>
#include <lcmtypes/omnibot_global_pid_t.hpp>
#include <lcmtypes/odometry_t.hpp>

#define UARTBAUD 115200

void sendMessageUART(Messaging::Message* msg_buf);

void rxMsgCallback(Messaging::Message &msg);

#endif
