#Test Python
print 'Launching script'

import serial

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
	print "Sending a message"
	arduino.write("X|01&ABCD|")
	print "Arduino says"
	while True:
		print arduino.readline()