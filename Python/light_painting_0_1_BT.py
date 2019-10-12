#Test Python
print 'Launching script'

import serial

arduino = serial.Serial('/dev/tty.HC-05-DevB',9600, timeout=1)
arduino.write("Salut, il y a quelqu'un?")


message = arduino.readline() 
while message != "Debut du code" :
	print "Pas de retour"
	print message
	message = arduino.readline()


