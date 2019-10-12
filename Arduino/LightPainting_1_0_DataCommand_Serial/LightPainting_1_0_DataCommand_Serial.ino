// Test d'instructions via Serial pour Neo Pixel
/*

Ce script recoit des instructions en serial.
Le script a utiliser est lightPainting_1_0.


*/

/* Neopixel variables */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      300

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Char to store the current character
char m = 'Z';
// Int to store pixel
int pixelStart = 0;
// Char to store color code
char codeReceived[] = "zzzzzz";
// Int to store colours to implement
int colorToSend[3] = {0 ,0 ,0};
//Int to count colors
int rgbCount = 0;


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);	// opens serial port, sets data rate to 9600 bps
        Serial.flush();
Serial.println("Debut du code");
  pixels.begin(); // This initializes the NeoPixel library.

//delay(2000);
}
float intensity = 0;

void loop() {

/*while (analogRead(A0) != 0)
{delay(1);}*/
//Serial.println(analogRead(A0));
Serial.println("Shoot!");

//On attend qu'un message arrive
while (!Serial.available())
{;}
//On a recu un message
//Debut de boucle de lecture

m = Serial.read();

if (m == 'P')
  {
  // We are receiving a pixel message. 
  incomingPixel();
  }
if (m == 'T')
  {
  incomingTimer();
  }
if (m == 'G')
  {
  incomingGo();
  }
if (m == 'S')
  {
  incomingStop();
  }  

}

void incomingPixel()
{
  //Serial.println("It is a Pixel!");
while (!Serial.available())
        {;}
//Serial.print("Starting at pixel: ");
pixelStart = 0;
        m = Serial.read();
while (m != '&')
  {
  while (!Serial.available())
      {;}
      pixelStart = pixelStart * 10 + (m - 48); 
//  Serial.print(m);
  m = Serial.read();
  
  }
 // Serial.print("pixelStart = ");
  //Serial.println(pixelStart);
//Serial.println();  

if (m == '&')
  {
 // Serial.println("I have received the pixel, now here are the colours");
  }
while (!Serial.available())
        {;}
    m = Serial.read();
    rgbCount = 0;
    
while ( m != 'X')
  {

  if (m != '|')
    {  
      
      //Serial.print(m);
      codeReceived[rgbCount] = m;
          rgbCount = rgbCount +1;

      }
    
  else 
    {
            //Serial.print("J'ai recu :");
            //for (int i =0; i <rgbCount +1; i++)
              //{Serial.print(codeReceived[i]);}
           
           
   //   Serial.println();
      rgbCount = 0;
          // Converting the HEX array to RGB
                                for (int i=0; i<3; i++)
                        {
                          colorToSend[i] = 0;
                           for (int j=0; j<2; j++)
                              {
                                if (codeReceived[2*i+j]>47 && codeReceived[2*i+j]<58 )
                                {
                                //  Serial.println("It's a number");
                                  colorToSend[i] = colorToSend[i]*16 + codeReceived[2*i+j] - 48;
                                }
                                else if (codeReceived[2*i+j]>64 && codeReceived[2*i+j]<71 )
                                {
                                  //Serial.println("It's a capital letter");
                                  colorToSend[i] = colorToSend[i]*16 + codeReceived[2*i+j] - 65+10;
                                }
                                 else if (codeReceived[2*i+j]>96 && codeReceived[2*i+j]<103 )
                                {
                                //  Serial.println("It's a minuscul letter");
                                  colorToSend[i] = colorToSend[i]*16 + codeReceived[2*i+j] - 97+10;
                                }
                                else
                                {
                               // Serial.println("It's out of bound");
                                colorToSend[i] = colorToSend[i]*16;
                                }
                              }
                       /* Serial.print("Iteration ");
                        Serial.print(i);
                        Serial.print(" Color set is ");
                        Serial.println(colorToSend[i]);*/
                        }
          
          
          intensity = 0.15;
        pixels.setPixelColor(pixelStart, pixels.Color(colorToSend[0]*intensity,colorToSend[1]*intensity,colorToSend[2]*intensity)); 
    pixelStart = pixelStart +1;} 
    
     while (!Serial.available())
        {;}
    m = Serial.read();
  }
  
  

  
}

void incomingTimer()
{
while (!Serial.available())
        {;}
}

void incomingGo()
{
  Serial.println("FIRE UP THE ROCKET!");
    pixels.show(); // This sends the updated pixel color to the hardware.
    //delay(1000);

     Serial.println("ROCKET FIRED");

    Serial.flush();
//delay(1000);
}

void incomingStop()
{
   for(int i=0;i<NUMPIXELS;i++){
  
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
  Serial.println("LANDED, STOPED");
  
  
    }
          pixels.show(); // This sends the updated pixel color to the hardware.


}

