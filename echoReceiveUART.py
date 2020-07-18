import serial
import time

ser = serial.Serial("/dev/serial0",baudrate=921600,timeout=1) 
if ser.isOpen():
	print(ser.name + ' is open...')
while 1:
	time.sleep(0.5)
	print(ser.read(11).hex())

