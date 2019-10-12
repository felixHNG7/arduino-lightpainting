// Test d'instructions via Serial pour Neo Pixel
/*

Instruction send is OK
Implementing LIGHT Instruction

*/
#include <SoftwareSerial.h>


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
SoftwareSerial bluetoothSerial(10, 11); // RX, TX

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
//Serial.begin(115200);	// opens serial port, sets data rate to 9600 bps
  //      Serial.flush();
 bluetoothSerial.begin(115200);        
bluetoothSerial.println("Debut du code");
//Serial.println("Debut du code");
  pixels.begin(); // This initializes the NeoPixel library.

//delay(2000);
}

void loop() {
   // bluetoothSerial.flush();
   //   delay(100);

bluetoothSerial.println("Shoot!");
//Serial.println("Shoot!");

//On attend qu'un message arrive
while (!bluetoothSerial.available())
{;}
//On a recu un message
//Debut de boucle de lecture

m = bluetoothSerial.read();

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
while (!bluetoothSerial.available())
        {;}
//Serial.print("Starting at pixel: ");
pixelStart = 0;
        m = bluetoothSerial.read();
while (m != '&')
  {
  while (!bluetoothSerial.available())
      {;}
      pixelStart = pixelStart * 10 + (m - 48); 
//  Serial.print(m);
  m = bluetoothSerial.read();
  
  }
  //Serial.print("pixelStart = ");
 // Serial.println(pixelStart);
//Serial.println();  

if (m == '&')
  {
 // Serial.println("I have received the pixel, now here are the colours");
  }
while (!bluetoothSerial.available())
        {;}
    m = bluetoothSerial.read();
    rgbCount = 0;
 int watchDog = 0;
while ( m != 'X' && watchDog < 250)
  {
watchDog = watchDog +1;
  if (m != '|')
    {  
      
    //  Serial.print(m);
      codeReceived[rgbCount] = m;
          rgbCount = rgbCount +1;

      }
    
  else 
    {
        //    Serial.print("J'ai recu :");
          //  for (int i =0; i <rgbCount +1; i++)
            //  {Serial.print(codeReceived[i]);}
           
           
    // Serial.println();
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
          
          
          
        pixels.setPixelColor(pixelStart, pixels.Color(colorToSend[0],colorToSend[1],colorToSend[2])); 
    pixelStart = pixelStart +1;} 
    
     while (!bluetoothSerial.available())
        {;}
    m = bluetoothSerial.read();
  }
//  Serial.println("Out of While not X");
  
  
    bluetoothSerial.flush();

  
}

void incomingTimer()
{
while (!bluetoothSerial.available())
        {;}
}

void incomingGo()
{
  delay(100);
  bluetoothSerial.println("FIRE UP THE ROCKET!");
   // Serial.println("FIRE UP THE ROCKET!");

    pixels.show(); // This sends the updated pixel color to the hardware.
    //delay(1000);

     bluetoothSerial.println("ROCKET FIRED");
    // Serial.println("ROCKET FIRED");

    bluetoothSerial.flush();
//delay(1000);
}

void incomingStop()
{
   for(int i=0;i<NUMPIXELS;i++){
  
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
  bluetoothSerial.println("LANDED, STOPED");
      bluetoothSerial.flush();

  //Serial.println("LANDED, STOPED");
  
    }
          pixels.show(); // This sends the updated pixel color to the hardware.


}

