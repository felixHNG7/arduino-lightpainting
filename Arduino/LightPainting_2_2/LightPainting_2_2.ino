/*
 
 Second stable version of my light painting tool.
 
 Designed to work on an Arduino Mega board with an ethernet shield attached.
 Also 1 button for command and a potentiometer.
 
 The board wakes up and waits for interaction on the button.
 When the button is triggered, the board will read in the SD card the files of the picture.
 In the card, there is a repertory called "ARDUINO". Inside should be the graphic files.
 
 Since the board can not handle graphic files, we converted the files in .dat files. Each column of pixel is transformed in a serie of bytes, in order of display.
 The file system in the SD card is ARDUINO/Picture number/xx/yy.dat . The column number is xxyy. 
 
 Enjoy!
 
 04/10 - Test with cosinus
 
 */

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


/* 

These settings are for MEGA pin 5:
#define PIXEL_PORT  PORTE  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRE  // Port of the pin the pixels are connected to
#define PIXEL_BIT   3      // Bit of the pin the pixels are connected to

These settings are for UNO pin 6:
#define PIXEL_PORT  PORTD  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRD   // Port of the pin the pixels are connected to
#define PIXEL_BIT   6      // Bit of the pin the pixels are connected to

*/
#define PIXEL_PORT  PORTE  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRE  // Port of the pin the pixels are connected to
#define PIXEL_BIT   3      // Bit of the pin the pixels are connected to


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


/*
For each column, the pixel values are stored in these arrays for R,G,B.
We allocate them in the beginning of the code to optimize space.
*/
byte rArray[PIXELS];
byte gArray[PIXELS];
byte bArray[PIXELS];

/* Intensity of the strip */
float intensity = 0;
int myProgram = 0;

void setup() {
    ledsetup();
      pinMode(A0, INPUT);
      pinMode(A1, INPUT);

/* Initialisation of led strip, first bit is sparked green for half a second then the whole strip is shut */
    cli();   
  sendPixel( 0 , 50 , 0 );
    sei();
    show();
    delay(500);
     cli();    
  sendPixel( 0 , 50 , 0 );

   for(int i=0;i<PIXELS;i++){
      sendPixel( 0 , 0 , 0 );
        }
    sei();
    show();
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


}


void loop() {
  
  Serial.println("Bienvenue");
/*  while (!Serial.available()) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }*/
  
  /*
  Waiting for the button to be hit before lauching program. Adjusting intensity in the process
  */
  
 while (analogRead(A0) > 1000 ) {
    ; // wait for serial port to connect. Needed for Leonardo only
    float myIntensity = 50*2*float(analogRead(A1))/1023;
     cli();    
  sendPixel( 0 , myIntensity , 0 );
    sei();
    show();
  }
  
  /*
  Starting script.
  */
  
Serial.println("C'est parti");
         // intensity = 0.4;
intensity = 2*float(analogRead(A1))/1023;
Serial.print("Intensity: ");
Serial.println(intensity);
int roof_intensity = intensity*255;
Serial.print("Roof intensity: ");
Serial.println(roof_intensity);
  for (int x = 0; x <1024; x++)
  {

  
  // if the file is available, open it and read the values:
  Serial.print("x");
  Serial.println(x);

 /* 
rArray[i]= int(abs(float(cos(M_PI*i/72)+cos(M_PI*x/32))/2*roof_intensity));
      gArray[i]= int(abs(float(cos(M_PI*i/36)+cos(M_PI*x/64))/2*roof_intensity));
      bArray[i]= int(abs(float(cos(M_PI*i/18)+cos(M_PI*x/128))/2*roof_intensity));
     */
volatile unsigned int i = 0;
volatile unsigned int jobi = 0;

cli();
   while (i < PIXELS)
   {
     
           uint8_t oldSREG = SREG; // Save the status
           jobi = cos(i)*30;
  SREG = oldSREG;

    sendPixel( jobi,0,0 );

  // Do stuff, as little as possible.

    i++;

 }
 sei();
 //sendPixel( int(abs(float(cos(M_PI*i/72)+cos(M_PI*x/32))/2*roof_intensity)),int(abs(float(cos(M_PI*i/36)+cos(M_PI*x/64))/2*roof_intensity)),int(abs(float(cos(M_PI*i/18)+cos(M_PI*x/128))/2*roof_intensity)) );

    show();
    delay(100);
   
  /*  Serial.print("X = ");
    Serial.print(x);
    Serial.println(" Done");*/
    
}
Serial.println("Image has been painted");

     myProgram = myProgram+1;
  /*
  
Clean the strip

*/
 
 cli();
  for(int i=0;i<PIXELS;i++){
  sendPixel( 0 , 0 , 0 );
   rArray[i]= 0;
      gArray[i]= 0;
      bArray[i]= 0;
    }
    sei();
    show();
    
    Serial.println("Strip is off");


  /*
  Incrementing the image number 
  */
  

Serial.flush();
}

/*
  Following is the whole API. What follows are some demo functions rewriten from the AdaFruit strandtest code...
  
  https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest/strandtest.ino
  
  Note that we always turn off interrupts while we are sending pixels becuase an interupt
  could happen just when we were in the middle of somehting time sensitive.
  
  If we wanted to minimize the time interrupts were off, we could instead 
  could get away with only turning off interrupts just for the very brief moment 
  when we are actually sending a 0 bit (~1us), as long as we were sure that the total time 
  taken by any interrupts + the time in our pixel generation code never exceeded the reset time (5us).
  
*/


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




