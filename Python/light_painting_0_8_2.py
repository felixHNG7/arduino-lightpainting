#Test Python
#Supporting Bluetooth
print 'Launching script'

import serial
import time
import random
from PIL import Image
messageSize = 16
stripSize = 300
imageSize = 10

### CABLE
# 300 Leds/5M
# Image 300*533: 133s, 0.24s/line
# Distance between 2 leds: 1,66cm
# Maximum speed: ~6.9cm/s, ~0.25km/h 

# 144 Leds/m
# Image 144*256: 32s, 0.11s/line
# Distance between 2 leds: 0.7mm
# Maximum speed: ~6.36cm/s, ~0.22km/h

### Bluetooth
# 300 Leds/5M


# 144 Leds/m

################# Loading image
myImage = Image.open("img/light_painting_60x159.png")
imageX = myImage.size[0]
imageY = myImage.size[1]
pix = myImage.load()
imageSize = imageX

print "Printing image in " + str(imageSize) + " iterations"
if (imageY > stripSize):
	print "Image is too big for strip"



################# Starting Arduino


#arduino = serial.Serial('/dev/tty.usbmodem1451',115200)
arduino = serial.Serial('/dev/tty.H-C-2010-06-01-DevB',115200, timeout=1)

#arduino = serial.Serial('/dev/tty.usbmodem1411',115200, timeout=1)
message = arduino.readline() 
print "Arduino starting"
while not "Debut du code" in message :
	print message
	message = arduino.readline()


print "Arduino Started"
columnCount = 0
general_time = time.time()

################# Starting program


################# For each column of pixel

while columnCount < imageSize:
	print "Printing column " + str(columnCount)
	message = arduino.readline() 
	while not "Shoot" in message :
		print message
		message = arduino.readline()

################# Sending a pixel column
################# Pixel columns are sent as series of pixels in a message. Each message has messageSize pixel in order to not overload the arduino

	if "Shoot" in message:
		print "Sending a pixel column"
		start_time = time.time()
		y = 0
################# Y is the height of each pixel, 


		while (y < imageY):
			#arduino.write("X|" + str(x) + "&00001|" + str(x+1) + "&00002|" + str(x+2) + "&00003|" + str(x+3) + "&00004|" + str(x+4) + "&00005|" + str(x+5) + "&00006|" + str(x+6) + "&00007|" + str(x+7) + "&00008|" + "X")
			#arduino.write("X|" + str(x) + "&00001|" + "&00002|" + "&00003|" + "&00004|" + "&00005|" + "&00006|" + "&00007|" + "&00008|" + "X")
			toSend = "P" + str(y) + "&"
			yDelta = 0
################# X is a variable to increment the counter in the message that is begin sent

			while (yDelta < messageSize) & (yDelta + y < imageY):
				#toSend = toSend + "A00ABF"  + "|"
				#print yDelta+y
				#print columnCount
				#print hex(pix[columnCount,yDelta+y][0])[2:].upper()
				#toSend = toSend + hex(random.randint(0,255))[2:].upper()  + hex(random.randint(0,255))[2:].upper()  + hex(random.randint(0,255))[2:].upper()  + "|"
				#print "On lit " + str(hex(pix[columnCount,yDelta+y][0])[2:].upper() ) + str(hex(pix[columnCount,yDelta+y][1])[2:].upper() ) + str(hex(pix[columnCount,yDelta+y][2])[2:].upper() )
				stringR =str(hex(pix[columnCount,yDelta+y][0])[2:].upper() )
				if (pix[columnCount,yDelta+y][0] < 16 ):
					stringR = "0" + stringR
				stringG = str(hex(pix[columnCount,yDelta+y][1])[2:].upper() )
				if (pix[columnCount,yDelta+y][1] < 16):
					stringG = "0"  + stringG
				stringB = str(hex(pix[columnCount,yDelta+y][2])[2:].upper() )
				if (pix[columnCount,yDelta+y][2] < 16):
					stringB = "0"  + stringB
				#myString =  +  + 
				#print "On lit " + stringR + stringG + stringB
				#toSend = toSend + hex(pix[columnCount,yDelta+y][0])[2:].upper()  + hex(pix[columnCount,yDelta+y][0])[2:].upper()  + hex(pix[columnCount,yDelta+y][0])[2:].upper()  + "|"
				toSend = toSend + stringR + stringG + stringB  + "|"

				yDelta = yDelta +1
			toSend = toSend + "X"
			y = y+messageSize
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
	columnCount = columnCount + 1
arduino.write("S")
while not "STOPED" in message :
	message = arduino.readline()
	print message	
general_time_elapsed = time.time() - general_time
print general_time_elapsed
