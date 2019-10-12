#Test Python
print 'Launching script'

import serial
import time
import random

messageSize = 16
stripSize = 300
imageSize = 100

# 300 Leds/5M
# Image 300*533: 133s, 0.24s/line
# Distance between 2 leds: 1,66cm
# Maximum speed: ~6.9cm/s, ~0.25km/h 

# 144 Leds/m
# Image 144*256: 32s, 0.11s/line
# Distance between 2 leds: 0.7mm
# Maximum speed: ~6.36cm/s, ~0.22km/h


arduino = serial.Serial('/dev/tty.usbmodem1451',115200)

message = arduino.readline() 
while not "Debut du code" in message :
	print "Arduino starting"
	print message
	message = arduino.readline()


print "Arduino Started"
cycleCount = 0
general_time = time.time()

while cycleCount < imageSize:
	message = arduino.readline() 
	while not "Shoot" in message :
		print message
		message = arduino.readline()

	if "Shoot" in message:
		print "Sending a picture"
		start_time = time.time()
		x = 0
		while (x < stripSize):

			#arduino.write("X|" + str(x) + "&00001|" + str(x+1) + "&00002|" + str(x+2) + "&00003|" + str(x+3) + "&00004|" + str(x+4) + "&00005|" + str(x+5) + "&00006|" + str(x+6) + "&00007|" + str(x+7) + "&00008|" + "X")
			#arduino.write("X|" + str(x) + "&00001|" + "&00002|" + "&00003|" + "&00004|" + "&00005|" + "&00006|" + "&00007|" + "&00008|" + "X")
			toSend = "P" + str(x) + "&"
			c = 0
			while c < messageSize:
				#toSend = toSend + "A00ABF"  + "|"
				toSend = toSend + hex(random.randint(0,255))[2:].upper()  + hex(random.randint(0,255))[2:].upper()  + hex(random.randint(0,255))[2:].upper()  + "|"
				c = c +1
			toSend = toSend + "X"
			x = x+messageSize
			print toSend
			arduino.write(toSend)
			#print "Arduino says"
			message = arduino.readline()
			while not "Shoot" in message :
				print message
				message = arduino.readline()
		elapsed_time = time.time() - start_time
		print elapsed_time
		arduino.write("G")
		while not "ROCKET" in message :
				message = arduino.readline()
				print message
		while not "FIRED" in message :
				message = arduino.readline()
				print message		
	cycleCount = cycleCount + 1
general_time_elapsed = time.time() - general_time
print general_time_elapsed
