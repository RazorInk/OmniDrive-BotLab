# This Makefile was tested with GNU Make
CXX=g++

# Use pkg-config to lookup the proper compiler and linker flags for LCM
CFLAGS=`pkg-config --cflags lcm`
LDFLAGS=`pkg-config --libs lcm`

msg_types=lcmtypes\*.lcm

all:  lcmtypes/omnibot_speed_command_t.hpp \
	lcmListener \
	lcmPublisher

listener: lcmListener.o 
	$(CXX) -o $@ $^ $(LDFLAGS)

send-message: lcmPublisher.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp lcmtypes/omnibot_speed_command_t.hpp
	$(CXX) $(CFLAGS) -I. -o $@ -c $< 

clean:
