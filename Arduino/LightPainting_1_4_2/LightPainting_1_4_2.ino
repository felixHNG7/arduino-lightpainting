/*
 This is an example of how simple driving a Neopixel can be
 This code is optimized for understandability and changability rather than raw speed
 More info at http://wp.josh.com/2014/05/11/ws2812-neopixels-made-easy/
*/
/*
  SD card test

 This example shows how use the utility libraries on which the'
 SD library is based in order to get info about your SD card.
 Very useful for testing a card when you're not sure whether its working or not.

 The circuit:
  * SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module.
 		Pin 4 used here for consistency with other Arduino examples


 created  28 Mar 2011
 by Limor Fried
 modified 9 Apr 2012
 by Tom Igoe
 */
// include the SD library:
#include <SPI.h>
#include <SD.h>


// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 4;

// Change this to be at least as long as your pixel string (too long will work fine, just be a little slower)

#define PIXELS 144  // Number of pixels in the string

// These values depend on which pin your string is connected to and what board you are using 
// More info on how to find these at http://www.arduino.cc/en/Reference/PortManipulation

// These values are for digital pin 8 on an Arduino Yun or digital pin 12 on a DueMilinove/UNO
// Note that you could also include the DigitalWriteFast header file to not need to to this lookup.

#define PIXEL_PORT  PORTD  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRD   // Port of the pin the pixels are connected to
#define PIXEL_BIT   6      // Bit of the pin the pixels are connected to

// These are the timing constraints taken mostly from the WS2812 datasheets 
// These are chosen to be conservative and avoid problems rather than for maximum throughput 

#define T1H  900    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns

#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns

#define RES 6000    // Width of the low gap between bits to cause a frame to latch

// Here are some convience defines for using nanoseconds specs to generate actual CPU delays

#define NS_PER_SEC (1000000000L)          // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives

#define CYCLES_PER_SEC (F_CPU)

#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )

#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )

// Actually send a bit to the string. We must to drop to asm to enusre that the complier does
// not reorder things and make it so the delay happens in the wrong place.

void sendBit( bool bitVal ) {
  
    if (  bitVal ) {				// 0 bit
      
		asm volatile (
			"sbi %[port], %[bit] \n\t"				// Set the output bit
			".rept %[onCycles] \n\t"                                // Execute NOPs to delay exactly the specified number of cycles
			"nop \n\t"
			".endr \n\t"
			"cbi %[port], %[bit] \n\t"                              // Clear the output bit
			".rept %[offCycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
			"nop \n\t"
			".endr \n\t"
			::
			[port]		"I" (_SFR_IO_ADDR(PIXEL_PORT)),
			[bit]		"I" (PIXEL_BIT),
			[onCycles]	"I" (NS_TO_CYCLES(T1H) - 2),		// 1-bit width less overhead  for the actual bit setting, note that this delay could be longer and everything would still work
			[offCycles] 	"I" (NS_TO_CYCLES(T1L) - 2)			// Minimum interbit delay. Note that we probably don't need this at all since the loop overhead will be enough, but here for correctness

		);
                                  
    } else {					// 1 bit

		// **************************************************************************
		// This line is really the only tight goldilocks timing in the whole program!
		// **************************************************************************


		asm volatile (
			"sbi %[port], %[bit] \n\t"				// Set the output bit
			".rept %[onCycles] \n\t"				// Now timing actually matters. The 0-bit must be long enough to be detected but not too long or it will be a 1-bit
			"nop \n\t"                                              // Execute NOPs to delay exactly the specified number of cycles
			".endr \n\t"
			"cbi %[port], %[bit] \n\t"                              // Clear the output bit
			".rept %[offCycles] \n\t"                               // Execute NOPs to delay exactly the specified number of cycles
			"nop \n\t"
			".endr \n\t"
			::
			[port]		"I" (_SFR_IO_ADDR(PIXEL_PORT)),
			[bit]		"I" (PIXEL_BIT),
			[onCycles]	"I" (NS_TO_CYCLES(T0H) - 2),
			[offCycles]	"I" (NS_TO_CYCLES(T0L) - 2)

		);
      
    }
    
    // Note that the inter-bit gap can be as long as you want as long as it doesn't exceed the 5us reset timeout (which is A long time)
    // Here I have been generous and not tried to squeeze the gap tight but instead erred on the side of lots of extra time.
    // This has thenice side effect of avoid glitches on very long strings becuase 

    
}  

  
void sendByte( unsigned char byte ) {
    
    for( unsigned char bit = 0 ; bit < 8 ; bit++ ) {
      
      sendBit( bitRead( byte , 7 ) );                // Neopixel wants bit in highest-to-lowest order
                                                     // so send highest bit (bit #7 in an 8-bit byte since they start at 0)
      byte <<= 1;                                    // and then shift left so bit 6 moves into 7, 5 moves into 6, etc
      
    }           
} 

/*
  The following three functions are the public API:
  
  ledSetup() - set up the pin that is connected to the string. Call once at the begining of the program.  
  sendPixel( r g , b ) - send a single pixel to the string. Call this once for each pixel in a frame.
  show() - show the recently sent pixel on the LEDs . Call once per frame. 
  
*/


// Set the specified pin up as digital out

void ledsetup() {
  
  bitSet( PIXEL_DDR , PIXEL_BIT );
  
}

void sendPixel( unsigned char r, unsigned char g , unsigned char b )  {  
  
  sendByte(g);          // Neopixel wants colors in green then red then blue order
  sendByte(r);
  sendByte(b);
  
}


// Just wait long enough without sending any bots to cause the pixels to latch and display the last sent frame

void show() {
	_delay_us( (RES / 1000UL) + 1);				// Round up since the delay must be _at_least_ this long (too short might not work, too long not a problem)
}


/*
  That is the whole API. What follows are some demo functions rewriten from the AdaFruit strandtest code...
  
  https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest/strandtest.ino
  
  Note that we always turn off interrupts while we are sending pixels becuase an interupt
  could happen just when we were in the middle of somehting time sensitive.
  
  If we wanted to minimize the time interrupts were off, we could instead 
  could get away with only turning off interrupts just for the very brief moment 
  when we are actually sending a 0 bit (~1us), as long as we were sure that the total time 
  taken by any interrupts + the time in our pixel generation code never exceeded the reset time (5us).
  
*/

    byte rArray[PIXELS];
byte gArray[PIXELS];
byte bArray[PIXELS];

void setup() {
    ledsetup();

    cli();    

   for(int i=0;i<PIXELS;i++){
  
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
     // pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
 // Serial.println("LANDED, STOPED");
  
  sendPixel( 0 , 120 , 0 );
    }
    sei();
    show();
    delay(1000);
     cli();    

   for(int i=0;i<PIXELS;i++){
  
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
     // pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
 // Serial.println("LANDED, STOPED");
  
  sendPixel( 0 , 0 , 0 );
    }
    sei();
    show();
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
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


}


void loop() {
  for (int x = 0; x <4; x++)
  {


String stringAcces = String("ARDUINO/002/000") + String(x) +  String(".dat");
char charAcces[stringAcces.length()+1];
stringAcces.toCharArray(charAcces,stringAcces.length()+1);    
Serial.println(stringAcces);   

//File dataFile = SD.open("ARDUINO/002/0003.dat");
File dataFile = SD.open(charAcces);

  // if the file is available, write to it:
  if (dataFile) {
          
      for (int i = 0; i < PIXELS; i++)
   {
      rArray[i]= dataFile.read();
      gArray[i]=dataFile.read();
      bArray[i]=dataFile.read();
 }
  }
 
  cli();    

   for (int i = 0; i < PIXELS; i++)
   {
      
  sendPixel( rArray[i],gArray[i],bArray[i] );
 }
   
    sei();
    show();
    delay(100);
    dataFile.close();
   
    Serial.print("X = ");
    Serial.print(x);
    Serial.println(" Done");
    
}

 cli();
  for(int i=0;i<PIXELS;i++){
  
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
     // pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
 // Serial.println("LANDED, STOPED");
  
  sendPixel( 0 , 0 , 0 );
    }
    sei();
    show();
int i =5;
while (i<10)
{;}
}
/**
    
      File dataFile = SD.open("ARDUINO/002/0001.dat");
int r = 100;
int g = 0;
int b = 0;
  // if the file is available, write to it:
for 
  if (dataFile) {
          
      for (int i = 0; i < PIXELS; i++)
   {
      rArray[i]= dataFile.read();
      gArray[i]=dataFile.read();
      bArray[i]=dataFile.read();
 }
 
  cli();    

   for (int i = 0; i < PIXELS; i++)
   {
      
  sendPixel( rArray[i],gArray[i],bArray[i] );
 }
   
    sei();
    show();
    delay(1000);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
  }
    
    
     cli();    

   for(int i=0;i<PIXELS;i++){
  
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
     // pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
 // Serial.println("LANDED, STOPED");
  
  sendPixel( 120 , 120 , 120 );
    }
    sei();
    show();
    delay(1000);
     cli();    

   for(int i=0;i<PIXELS;i++){
  
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
     // pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
 // Serial.println("LANDED, STOPED");
  
  sendPixel( 0 , 0 , 0 );
    }
    sei();
    show();
int i =5;
while (i<10)
{;}
return;
  
}
**/

