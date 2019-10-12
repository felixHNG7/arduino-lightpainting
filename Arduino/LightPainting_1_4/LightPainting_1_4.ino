// Test d'instructions via Serial pour Neo Pixel
/*

Instruction send is OK
Implementing LIGHT Instruction

07/09/15
# Light Paiting, second iteration
#Objectives:
#- Reduce exposition time. 
#- Reduce light
#Leads:
#- Optimize transmission
#- Reduce palet of colors
#- Add a dimmer on arduino

Test: Transmitting RGB codes in 3 bytes
*/

/*
 This is an example of how simple driving a Neopixel can be
 This code is optimized for understandability and changability rather than raw speed
 More info at http://wp.josh.com/2014/05/11/ws2812-neopixels-made-easy/
*/

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

static byte arrayR[PIXELS];
static byte arrayG[PIXELS];
static byte arrayB[PIXELS];

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


// Char to store the current character
char m = 'Z';
// Int to store pixel
int pixelStart = 0;
// Char to store color code
byte codeReceived[] = {0,0,0};
// Int to store colours to implement
int colorToSend[3] = {0 ,0 ,0};
//Int to count colors
int rgbCount = 0;


void setup() {
    ledsetup();

  cli();    

   for(int i=0;i<PIXELS;i++){
  sendPixel( 0 , 0 , 0 );
    }
    sei();
    
    show();
  // put your setup code here, to run once:
Serial.begin(9600);	// opens serial port, sets data rate to 9600 bps
        Serial.flush();
Serial.println("Debut du code");

}

void loop() {

/*while (analogRead(A0) != 0)
{delay(1);}
//Serial.println(analogRead(A0));
//Serial.println("Shoot !");
Serial.println("S");*/

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
  Serial.println("I have received the pixel, now here are the colours");
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
         // Serial.print(m);
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
                          colorToSend[i] = int(codeReceived[i])/5;
                          
                /*       Serial.print("Iteration ");
                        Serial.print(i);
                        Serial.print(" Color set is ");
                        Serial.println(colorToSend[i], DEC);*/
                        
                        }
          
         //   cli();     
      
 // sendPixel( colorToSend[0] , colorToSend[1] , colorToSend[2] );
    
   // sei();
    
          arrayR[pixelStart] = colorToSend[0];
          arrayG[pixelStart] = colorToSend[1];
          arrayB[pixelStart] = colorToSend[2];
       // pixels.setPixelColor(pixelStart, pixels.Color(colorToSend[0],colorToSend[1],colorToSend[2])); 
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
  //Serial.println("FIRE UP THE ROCKET!");
  Serial.println("F");
   cli();     
      
    for(int i=0;i<PIXELS;i++){
 sendPixel( arrayR[i] , arrayG[i] , arrayB[i] );
    }
    sei();
    show();
   // pixels.show(); // This sends the updated pixel color to the hardware.
    //delay(1000);
    delay(100);
  Serial.println("R");

    // Serial.println("ROCKET FIRED");

    Serial.flush();
//delay(1000);
}

void incomingStop()
{
      cli();    

   for(int i=0;i<PIXELS;i++){
  
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
     // pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
 // Serial.println("LANDED, STOPED");
  
  sendPixel( 0 , 0 , 0 );
    }
    sei();
    
    show();
         // pixels.show(); // This sends the updated pixel color to the hardware.
 Serial.println("LANDED, STOPED");


}

