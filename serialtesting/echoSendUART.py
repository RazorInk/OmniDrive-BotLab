import serial
import time
import os

ser = serial.Serial("/dev/serial0",baudrate=512000,timeout=0.005) 
if ser.isOpen():
	print(ser.name + ' is open...')
writeData1 = [0xFA,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x0F]
writeData2 = [0x0F,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xFF]
i = 0

f = open("input.txt", "w")

numTests = 1

for i in range(numTests):
	time.sleep(0.002)
	a = os.urandom(64)
	print(a.hex())
	# f.write(a.hex()+"\n")
	for j in range(64):
		f.write(hex(a[j])+" ")
	f.write("\n")
	ser.write(a)

	# if (i % 100) == 0 :
		# time.sleep(0.25)


# while 1:
# 	if (i % 2) == 0:
# 		ser.write(writeData1)
# 	else:
# 		ser.write(writeData2)
# 	i = i+1

# ser.write([0xFA, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11])
print("write complete")

