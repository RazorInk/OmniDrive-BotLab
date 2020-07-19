import serial
import time
import os

# ser = serial.Serial("/dev/serial0",baudrate=115200,timeout=1) 
ser = serial.Serial("/dev/serial0",baudrate=512000,timeout=0.02) 
if ser.isOpen():
	print(ser.name + ' is open...')
fails = 0
numTests = 10000

for i in range(numTests):
	print(i)
	a = os.urandom(64)
	ser.write(a)
	# time.sleep(0.3)
	b = ser.read(64)
	# print(a.hex())
	# print(b.hex())
	if a != b:
		fails = fails + 1

print("failure rate: " + str(fails) + "/" + str(numTests))



# while 1:
# 	if (i % 2) == 0:
# 		ser.write(writeData1)
# 	else:
# 		ser.write(writeData2)
# 	i = i+1

# ser.write([0xFA, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11])
print("write complete")

