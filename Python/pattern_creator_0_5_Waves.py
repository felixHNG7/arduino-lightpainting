import Image
import math
import os

img = Image.new( 'RGB', (720,144), "black") # create a new black image
pixels = img.load() # create the pixel map

periodRVert = 144/2;
periodGVert = 144/2;
periodBVert = 144/2;

periodRHori = 720/4;
periodGHori = 720/4;
periodBHori = 720/4;

deltaR = 0;
deltaG = math.pi/2;
deltaB = math.pi;



for i in range(img.size[0]):    # for every pixel:
    for j in range(img.size[1]):
        #pixels[i,j] = (i, j, 100) # set the colour accordingly
        #pixels[i,j] = (int((math.cos(math.pi*float(i+j)/float(periodR)+deltaR)+1)/2*255), int((math.cos(math.pi*float(i)/float(periodG)+deltaG)+1)/2*255), int((math.cos(math.pi*float(i)/float(periodB)+deltaB)+1)/2*255), 255)
    	#pixels[i,j] = (int((math.cos(math.pi*float(i+j)/float(periodR)+deltaR)+1)/2*255), int((math.cos(math.pi*float(i-j)/float(periodG)+deltaG)+1)/2*255), 0, 255)

    	#pixels[i,j] = (int(((math.cos(math.pi*float(j)/float(periodR)+math.cos(math.pi*float(i)/periodR))+1)/2)*255), 0, 0, 255)

    	#pixels[i,j] = (int(((math.cos(math.pi*float(i)/float(periodR)+deltaR)+1)/2)*((math.cos(math.pi*float(j)/float(periodR)+deltaR)+1)/2)*255), int(((math.cos(math.pi*float(i)/float(periodG)+deltaG)+1)/2)*((math.cos(math.pi*float(j)/float(periodG)+deltaG)+1)/2)*255), int(((math.cos(math.pi*float(i)/float(periodB)+deltaB)+1)/2)*((math.cos(math.pi*float(j)/float(periodB)+deltaB)+1)/2)*255), 255)

    	#pixels[i,j] = (int((math.cos(math.pi*float(i)/float(51))+1)/2*255), 0, 0, 255)

    	pixels[i,j] = (int(((math.cos(math.pi*float(j)/float(periodRVert)+math.cos(math.pi*float(i)/periodRHori))+1)/2)*255), int(((math.cos(math.pi*float(j)/float(periodGVert)+math.cos(math.pi*float(i)/periodGHori))+1)/2)*255), int(((math.cos(math.pi*float(j)/float(periodBVert)+math.cos(math.pi*float(i)/periodBHori))+1)/2)*255), 255)
    	#pixels[i,j] = (int(((math.cos(math.pi*float(j)/float(periodRVert)+math.cos(math.pi*float(i)/periodRHori))+1)/2)*255), 0, 0, 255)


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
	

	