# Light Paiting, second iteration
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
# import random
from PIL import Image
messageSize = 8
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
myImage = Image.open("img/light_painting_144x257.png").convert("RGB")
myPixels = list(myImage.getdata())
width = myImage.size[0]
height = myImage.size[1]
pix = myImage.load()
imageSize = width



gamma = bytearray(256)
for i in range(256):
	gamma[i] = 0x80 | int(pow(float(i) / 255.0, 2.5) * 127.0 + 0.5)
# Create list of bytearrays, one for each column of image.
# R, G, B byte per pixel, plus extra '0' byte at end for latch.
print "Allocating..."
column = [0 for x in range(width)]
for x in range(width):
	column[x] = bytearray(height * 3 + 1)

print "Converting..."
for x in range(width):
	for y in range(height):
		value = pixels[x, y]
		y3 = y * 3
		column[x][y3] = gamma[value[1]]
		column[x][y3 + 1] = gamma[value[0]]
		column[x][y3 + 2] = gamma[value[2]]


################# Starting Arduino

