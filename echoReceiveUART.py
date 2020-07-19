import serial
import time

ser = serial.Serial("/dev/serial0",baudrate=512000,timeout=0.005) 
ser.flush()

f = open("output.txt", "w")

if ser.isOpen():
	print(ser.name + ' is open...')
while 1:
	time.sleep(0.01)
	a = ser.read(64)
	if len(a.hex()) > 0:
		print(a.hex())
		f.write(a.hex()+"\n")

