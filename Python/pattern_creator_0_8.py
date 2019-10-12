import Image
import math
import os

img = Image.new( 'RGB', (1024,144), "black") # create a new black image
pixels = img.load() # create the pixel map

periodRVert = 144/2;
periodGVert = 144/4;
periodBVert = 144/8;

periodRHori = 144/2;
periodGHori = 64;
periodBHori = 144/4;

deltaR = 0;
deltaG = math.pi;
deltaB = math.pi;



for i in range(img.size[0]):    # for every pixel:
    for j in range(img.size[1]):
    	formulaR = int(((math.cos(math.pi*float(i*i)/float(periodRVert)+deltaR)+1)/2)*((math.cos(math.pi*float(j*j)/float(periodRHori)+deltaG)+1)/2)*255)
    	#formulaR = 0
    	formulaG = int(((math.cos(math.pi*float(j)/float(periodGVert))+math.cos(math.pi*float(i)/periodGHori)+1)/2)*255)

    	formulaB = int(((math.cos(math.pi*float(i-j)/float(periodBHori)+deltaB)+1)/2+(math.cos(math.pi*float(i+j)/float(periodBHori)+deltaB)+1)/2)*255)

    	pixels[i,j] = (formulaR , formulaG , formulaB , 255)

    

img.show()

# Saving the image for futur use!

isSaved = False
x = 0

while not isSaved:
	fileName = "img/00_Pattern_Creator/img"


	if x < 1000:
		fileName = fileName +'0'
	if x < 100:
		fileName = fileName +'0'
	if x < 10:
		fileName = fileName +'0'
	fileName = fileName + str(x) + ".bmp"
		
	if not os.path.isfile(fileName):
		print("New file is created")
		img.save(fileName)
		isSaved = True
	x = x + 1
	

	