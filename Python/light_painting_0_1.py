#Test Python
print 'Launching script'

import serial

arduino = serial.Serial('/dev/tty.usbmodem1451',115200)

message = arduino.readline() 
while message != "Debut du code" :
	print "On y est pas"
	print message
	message = arduino.readline()


print "On y est"