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
for x in range(1,100):
	filename = "img/25 Spleen/0" + str(x) + ".png"
	if not os.path.exists(filename):
		print "No more file"
		break
		
	myImage = Image.open(filename)
	myPixels = list(myImage.getdata())
	imageX = myImage.size[0]
	imageY = myImage.size[1]
	pix = myImage.load()
	imageSize = imageX
	myImage.show()



################# 


#for x in range(0,imageX):
#	for y in range(0,imageY):
#		print pix[x,y]
		#pix[x,y][3] = i
myImage.show()




