import Image
import math
import os

img = Image.new( 'RGB', (720,144), "black") # create a new black image
pixels = img.load() # create the pixel map

periodRVert = 144;
periodGVert = 144/2;
periodBVert = 144/8;

periodRHori = 144;
periodGHori = 144/2;
periodBHori = 128/4;

deltaR = 0;
deltaG = math.pi;
deltaB = math.pi;



for i in range(img.size[0]):    # for every pixel:
    for j in range(img.size[1]):
         #J Is vertical (Pixel) (i in arduino) and I is x in arduino
    	

        formulaR = int(((math.cos(math.pi*float(i*i)/float(periodRVert))+1)/2)*((math.cos(math.pi*float(j*j)/float(periodRHori))+1)/2)*255)
    	#formulaR = 0
    	formulaG = int(((math.cos(math.pi*float(j)/float(periodGVert))+math.cos(math.pi*float(i)/periodGHori)+1)/2)*255)
    	#formulaG = 0
    	#formulaB = int((math.cos(math.pi*float(i-j)/float(periodBHori)+deltaB)+1)/2*255)
    	formulaB = int(((math.cos(math.pi*float(i-j)/float(periodBHori))+1)/2+(math.cos(math.pi*float(i+j)/float(periodBHori))+1)/2)*255)

    	#formulaB = int((math.cos(math.pi*float(i+j)/float(periodBHori)+deltaB)+1)+(math.cos(math.pi*float(i-j)/float(periodBHori)+deltaB)+1)/2*255)

    	#pixels[i,j] = (formulaR , formulaG , formulaB , 255)
    	pixels[i,j] = (formulaR , formulaG , formulaB, 255)

    	#pixels[i,j] = (int((math.cos(math.pi*float(i)/float(51))+1)/2*255), 0, 0, 255)

      #  print(int((math.cos(math.pi*float(i)/float(20))+1)/2*255))

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
	

	