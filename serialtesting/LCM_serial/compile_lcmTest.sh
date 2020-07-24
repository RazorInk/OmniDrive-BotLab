cd lcmtypes
make
cd ..
# cd ../../lib/messaging
# g++ -c messaging.cpp
# cd ../../serialtesting/LCM_serial
# g++ `pkg-config --libs lcm` ../../messaging/messaging.o -o  lcmListener customSerial.c lcmListener.cpp
g++ `pkg-config --libs lcm` -o  bin/serialLinkTx \
	-I../../lib/messaging/ -I../../lib/serial/ \
	../../lib/messaging/messaging.cpp \
	../../lib/messaging/nu2pi.hpp \
	../../lib/messaging/pi2nu.hpp \
	../../lib/serial/customSerial.c \
	serialLinkCommon.hpp serialLinkTx.cpp
g++ `pkg-config --libs lcm` -o  bin/serialLinkRx \
	-I../../lib/messaging/ -I../../lib/serial/ \
	../../lib/messaging/messaging.cpp \
	../../lib/messaging/nu2pi.hpp \
	../../lib/messaging/pi2nu.hpp \
	../../lib/serial/customSerial.c \
	serialLinkCommon.hpp serialLinkRx.cpp

g++ `pkg-config --libs lcm` -o  bin/odometry odometry.cpp
g++ `pkg-config --libs lcm` -o  bin/motion_controller motion_controller.cpp
g++ `pkg-config --libs lcm` -o  bin/drive_square drive_square.cpp

g++ `pkg-config --libs lcm` -o  bin/simple_square simple_square.cpp

