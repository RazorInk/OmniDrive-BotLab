cd lcmtypes
make
cd ..
# cd ../../lib/messaging
# g++ -c messaging.cpp
# cd ../../serialtesting/LCM_serial
# g++ `pkg-config --libs lcm` ../../messaging/messaging.o -o  lcmListener customSerial.c lcmListener.cpp
g++ `pkg-config --libs lcm` -o  serialOUT -I../../lib/messaging/ \
	../../lib/messaging/messaging.cpp \
	../../lib/messaging/nu2pi.h \
	../../lib/messaging/pi2nu.h \
	customSerial.c serialOUT.cpp
g++ `pkg-config --libs lcm` -o  serialIN -I../../lib/messaging/ \
	../../lib/messaging/messaging.cpp \
	../../lib/messaging/nu2pi.h \
	../../lib/messaging/pi2nu.h \
	customSerial.c serialIN.cpp


