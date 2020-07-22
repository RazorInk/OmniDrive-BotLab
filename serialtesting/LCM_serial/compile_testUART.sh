cd lcmtypes
make
cd ..
g++ `pkg-config --libs lcm` -o  lcmListener customSerial.c messaging.cpp lcmListener.cpp
g++ `pkg-config --libs lcm` -o  lcmPublisher lcmPublisher.cpp

