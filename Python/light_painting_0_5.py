#Test Python
print 'Launching script'

import serial
import time

messageSize = 16;

arduino = serial.Serial('/dev/tty.usbmodem1451',115200)

message = arduino.readline() 
while not "Debut du code" in message :
	print "Arduino starting"
	print message
	message = arduino.readline()


print "Arduino Started"

message = arduino.readline() 
while not "Shoot" in message :
	print message
	message = arduino.readline()

if "Shoot" in message:
	print "Sending a picture"
	start_time = time.time()
	x = 0
	while (x < 255):

		#arduino.write("X|" + str(x) + "&00001|" + str(x+1) + "&00002|" + str(x+2) + "&00003|" + str(x+3) + "&00004|" + str(x+4) + "&00005|" + str(x+5) + "&00006|" + str(x+6) + "&00007|" + str(x+7) + "&00008|" + "X")
		#arduino.write("X|" + str(x) + "&00001|" + "&00002|" + "&00003|" + "&00004|" + "&00005|" + "&00006|" + "&00007|" + "&00008|" + "X")
		toSend = "P" + str(x) + "&"
		c = 0
		while c < messageSize:
			toSend = toSend + "ABCDE" + str(c) + "|"
			c = c +1
		toSend = toSend + "X"
		x = x+messageSize
		print toSend
		arduino.write(toSend)
		print "Arduino says"
		message = arduino.readline()
		while not "Shoot" in message :
			print message
			message = arduino.readline()
	elapsed_time = time.time() - start_time
	print elapsed_time
	arduino.write("G")
	while not "ROCKET" in message :
			print message
			arduino.write("G")
			message = arduino.readline()

