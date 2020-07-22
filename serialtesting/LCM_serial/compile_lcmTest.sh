cd lcmtypes
make
cd ..
# cd ../../lib/messaging
# g++ -c messaging.cpp
# cd ../../serialtesting/LCM_serial
# g++ `pkg-config --libs lcm` ../../messaging/messaging.o -o  lcmListener customSerial.c lcmListener.cpp
g++ `pkg-config --libs lcm` -o  lcmListener -I../../lib/messaging/ \
	../../lib/messaging/messaging.cpp \
	../../lib/messaging/nu2pi.h \
	../../lib/messaging/pi2nu.h \
	customSerial.c lcmListener.cpp
g++ `pkg-config --libs lcm` -o  lcmPublisher lcmPublisher.cpp

