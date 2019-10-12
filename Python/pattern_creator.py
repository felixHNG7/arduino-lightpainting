import Image
import math

img = Image.new( 'RGB', (1024,1024), "black") # create a new black image
pixels = img.load() # create the pixel map

periodR = 13;
periodG = 17;
periodB = 31;

deltaR = 0;
deltaG = 1;
deltaB = 4;



for i in range(img.size[0]):    # for every pixel:
    for j in range(img.size[1]):
        #pixels[i,j] = (i, j, 100) # set the colour accordingly
        pixels[i,j] = (int((math.cos(math.pi*float(i)/float(periodR)+deltaR)+1)/2*255), int((math.cos(math.pi*float(i)/float(periodG)+deltaG)+1)/2*255), int((math.cos(math.pi*float(i)/float(periodB)+deltaB)+1)/2*255), 255)
    	#pixels[i,j] = (int((math.cos(math.pi*float(i)/float(periodR)+deltaR)+1)/2*255), 0, 0, 255)

    	#pixels[i,j] = (int((math.cos(math.pi*float(i)/float(51))+1)/2*255), 0, 0, 255)

      #  print(int((math.cos(math.pi*float(i)/float(20))+1)/2*255))

img.show()