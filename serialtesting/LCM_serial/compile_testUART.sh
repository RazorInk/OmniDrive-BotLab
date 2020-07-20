cd lcmtypes
make
cd ..
g++ `pkg-config --libs lcm` -o  lcmListener customSerial.c lcmtypes/omnibot_speed_command_t.c messaging.cpp lcmListener.cpp
g++ `pkg-config --libs lcm` -o  lcmPublisher lcmtypes/omnibot_speed_command_t.c lcmPublisher.cpp

