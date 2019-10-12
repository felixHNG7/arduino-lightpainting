# Light Paiting, new generation
#Objectives:
#- Reduce exposition time. 
#- Reduce light
#Leads:
#- Optimize transmission
#- Reduce palet of colors
#- Add a dimmer on arduino

#Trying Python optimization - DONE
# Using Getdata method

print 'Launching script'

import serial
import time
import os
# import random
from PIL import Image
messageSize = 64
stripSize = 300
imageSize = 10

# 300 Leds/5M
# Image 300*533: 133s, 0.24s/line
# Distance between 2 leds: 1,66cm
# Maximum speed: ~6.9cm/s, ~0.25km/h 

# 144 Leds/m
# Image 144*256: 32s, 0.11s/line
# Distance between 2 leds: 0.7mm
# Maximum speed: ~6.36cm/s, ~0.22km/h

################# Loading image
myImage = Image.open("img/16 Franklin Avenue/02.png")
myPixels = list(myImage.getdata())
imageX = myImage.size[0]
imageY = myImage.size[1]
pix = myImage.load()
imageSize = imageX

################# 



################# For each column of pixel, creating a file
columnCount = 0;
while columnCount < imageSize:
	fileName = "img/16 Franklin Avenue/Formatted/009/"


	if columnCount/100 < 10:
		fileName = fileName +'0'
	fileName = fileName + str(columnCount/100) + "/"
	
	if not os.path.exists(fileName):
		os.makedirs(fileName)
	if (columnCount - (columnCount/100)*100) < 10:
		fileName = fileName +'0'


	fileName = fileName + str(columnCount - (columnCount/100)*100) + ".dat"			
	
	print "Creating file " + str(fileName)

	myFile = open(fileName, 'w')

################# Sending a pixel column
################# Pixel columns are sent as series of pixels in a message. Each message has messageSize pixel in order to not overload the arduino

	print "writing a pixel column in file " + str(fileName)
	y = 0
	while y < imageY:
		myFile.write(str(chr(pix[columnCount,y][0]/2)))
		myFile.write(str(chr(pix[columnCount,y][1]/2)))
		myFile.write(str(chr(pix[columnCount,y][2]/2)))
		y = y +1
################# Y is the height of each pixel, 
		#while (y < imageY):
		#		arduino.write(str(chr(pix[columnCount,yDelta+y][0])))
		#		arduino.write(str(chr(pix[columnCount,yDelta+y][1])))
		#		arduino.write(str(chr(pix[columnCount,yDelta+y][2])))
		#		arduino.write("|") 
				
				
			
		#	arduino.write("X")
		#	y = y+messageSize
		
		
	columnCount = columnCount + 1

print "Estimated image size:" + str(imageSize/float(144)) + "m"
print "Estimated exposition time:" + str(imageSize/float(144)*9) + "s"

