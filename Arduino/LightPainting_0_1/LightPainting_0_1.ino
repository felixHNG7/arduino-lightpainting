// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <SPI.h>
#include <SD.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;


// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      300

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 1000; // delay for half a second
int fadecount = 0;


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
    Serial.begin(9600);
 Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");


  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  
  
  
  
    if (fadecount == 0 )
    {
   for(int i=0;i<NUMPIXELS;i++){
 
  
    pixels.setPixelColor(i, pixels.Color(150,150,0)); // Moderately bright green color.
   }
       fadecount = 1;  
    }
    else if (fadecount == 1 )
    {
         for(int i=0;i<NUMPIXELS;i++){

    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
         }
    fadecount = 0;  
    }
  


  

      pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).

  
}
