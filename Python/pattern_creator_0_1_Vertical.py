import Image
import math
import os

img = Image.new( 'RGB', (1024,10), "black") # create a new black image
pixels = img.load() # create the pixel map

periodR = 13;
periodG = 17;
periodB = 31;

deltaR = 0;
deltaG = 1;
deltaB = 4;

r = 11
g = 3
b = 255

for i in range(img.size[0]):    # for every pixel:
    for j in range(img.size[1]):
        #pixels[i,j] = (i, j, 100) # set the colour accordingly
        #pixels[i,j] = (int((math.cos(math.pi*float(i)/float(periodR)+deltaR)+1)/2*255), int((math.cos(math.pi*float(i)/float(periodG)+deltaG)+1)/2*255), int((math.cos(math.pi*float(i)/float(periodB)+deltaB)+1)/2*255), 255)
    	pixels[i,j] = (r,g,b,255)

    	#pixels[i,j] = (int((math.cos(math.pi*float(i)/float(periodR)+deltaR)+1)/2*255), 0, 0, 255)

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
	

	