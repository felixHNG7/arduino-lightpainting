#include <FastLED.h>
#define LED_PIN     3
#define NUM_LEDS    50

//SPI to talk with the Shield, SD to manipulate the card
#include <SPI.h>

#include <SD.h>


// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 4;
int num = 0; // general variable containing the sequence number

const int button_incr = 5; //setting incrementation button pin D5
const int button_decr = 6; //setting decrementation button pin D6

const int buttonPin = 7;//switch between setting and operating
const int potentionmetrePin = A7;

// Change this to be at least as long as your pixel string (too long will work fine, just be a little slower)

boolean button1_state = LOW; //deboucing variable bouton +
boolean button2_state = LOW; //deboucing variable bouton -

#define PIXELS 144  // Number of pixels in the string
int myProgram = 1; // Number of the first picture to display

// These values depend on which pin your string is connected to and what board you are using
// More info on how to find these at http://www.arduino.cc/en/Reference/PortManipulation

// These values are for digital pin 8 on an Arduino Yun or digital pin 12 on a DueMilinove/UNO
// Note that you could also include the DigitalWriteFast header file to not need to to this lookup.

CRGB leds[NUM_LEDS];

/*

  These settings are for MEGA pin 5:
  #define PIXEL_PORT  PORTE  // Port of the pin the pixels are connected to
  #define PIXEL_DDR   DDRE  // Port of the pin the pixels are connected to
  #define PIXEL_BIT   3      // Bit of the pin the pixels are connected to

  These settings are for UNO pin 6:
  #define PIXEL_PORT  PORTD  // Port of the pin the pixels are connected to
  #define PIXEL_DDR   DDRD   // Port of the pin the pixels are connected to
  #define PIXEL_BIT   6      // Bit of the pin the pixels are connected to

  // Carte Doumo / Light painting
  //#define PIXEL_PORT  PORTD  // Port of the pin the pixels are connected to
  //#define PIXEL_DDR   DDRD  // Port of the pin the pixels are connected to
  //#define PIXEL_BIT   3      // Bit of the pin the pixels are connected to



*/
// Carte Doumo / Light painting
#define PIXEL_PORT  PORTD  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRD  // Port of the pin the pixels are connected to
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

void setup() {
  ledsetup();
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  Serial.begin(9600);
  pinMode(potentionmetrePin , INPUT); // potentiometer
  pinMode(button_incr, INPUT);
  pinMode(button_decr, INPUT);

  pinMode(buttonPin, INPUT);
  pinMode(potentionmetrePin, INPUT);

  /* Initialisation of led strip, first bit is sparked green for half a second then the whole strip is shut */
  cli();
  sendPixel( 0 , 50 , 0 );
  sei();
  show();
  delay(500);
  cli();
  sendPixel( 0 , 50 , 0 );

  for (int i = 0; i < PIXELS; i++) {
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
  //  pinMode(10, OUTPUT);
  //digitalWrite(10, HIGH); // Add this line

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");


}


void loop() {

  if (digitalRead(buttonPin) == 0) {
    Serial.println("Bienvenue");
    /*  while (!Serial.available()) {
        ; // wait for serial port to connect. Needed for Leonardo only
      }*/

    /*
      Starting script.
      2 sec pause
    */

    cli();

    for (int i = 0; i < PIXELS; i++)
    {
      sendPixel( 0 , 0 , 0 );
    }

    sei();
    show();

    delay(2000);



    Serial.println("C'est parti");
    // intensity = 0.4;
    intensity = float(analogRead(potentionmetrePin)) / 1023;
    //intensity = 0.1;
    Serial.print("Intensity: ");
    Serial.println(intensity);
    for (double x = 0; x < PIXELS * 4; x++)
    {

      /*
        First step is to get the filePath
      */

      String stringAcces = String("ARDUINO/0");
      if (myProgram < 10)
      {
        stringAcces = stringAcces + '0';
      }
      stringAcces = stringAcces + String(myProgram) + '/';

      if ((x / 100) < 10)
      {
        stringAcces = stringAcces + '0';
      }

      float f = x / 100;

      if (x == 0)
        stringAcces = stringAcces + "0.0";
      else
        stringAcces = stringAcces + String(f, 2);

      stringAcces = stringAcces + '/';

      Serial.println(String(f, 2));

      stringAcces = stringAcces  + "0.dat";
      char charAcces[stringAcces.length() + 1];
      stringAcces.toCharArray(charAcces, stringAcces.length() + 1);
      File dataFile = SD.open(charAcces);
      Serial.println(stringAcces);

      // if the file is available, open it and read the values:
      if (dataFile) {

        for (int i = 0; i < PIXELS; i++)
        {
          rArray[i] = int(dataFile.read() * intensity);
          gArray[i] = int(dataFile.read() * intensity);
          bArray[i] = int(dataFile.read() * intensity);
          /*  Serial.print("On a recu les couleurs ");
            Serial.print(rArray[i]);
            Serial.print(" ");
            Serial.print(gArray[i]);
            Serial.print(" ");
            Serial.print(bArray[i]);
            Serial.println();*/
        }
      }
      else
      {
        Serial.println("No more file");
        break;
      }
      dataFile.close();

      cli();
      for (int i = 0; i < PIXELS; i++)
      {

        sendPixel( rArray[i], gArray[i], bArray[i] );
      }

      sei();
      show();
      //delay(10);

      /*  Serial.print("X = ");
        Serial.print(x);
        Serial.println(" Done");*/

    }
    Serial.println("Image has been painted");

    /*

      Clean the strip

    */

    cli();
    for (int i = 0; i < PIXELS; i++) {
      sendPixel( 0 , 0 , 0 );
    }
    sei();
    show();

    Serial.println("Strip is off");
    delay(2000);


    /*
      Incrementing the image number
    */

    myProgram = myProgram + 1;

    Serial.flush();
  }
  else {
    mode();
  }
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

  for ( unsigned char bit = 0 ; bit < 8 ; bit++ ) {

    sendBit( bitRead( byte , 7 ) );                // Neopixel wants bit in highest-to-lowest order
    // so send highest bit (bit #7 in an 8-bit byte since they start at 0)
    byte <<= 1;                                    // and then shift left so bit 6 moves into 7, 5 moves into 6, etc

  }
}


void mode() {
  Serial.println("setting");
  //+
  while (digitalRead(buttonPin) == 1) {
    if (debounceButton(button1_state, button_incr) == HIGH && button1_state == LOW) {
      num++;
      button1_state = HIGH;
      /* convert presses to binary and store it as a string */
      String binNumber = String(num, BIN);
      /* get the length of the string */
      int binLength = binNumber.length();
      if (num <= 64) {
        for (int i = 0, x = 1; i < binLength; i++, x += 2) {
          if (binNumber[i] == '0') leds[i + binLength - x] = CRGB(0, 0, 0);
          if (binNumber[i] == '1') leds[i + binLength - x] = CRGB(255, 0, 0);
          FastLED.show();
        }
      }
      Serial.print("seq : ");
      Serial.println(num);
    }
    else if (debounceButton(button1_state, button_incr) == LOW && button1_state == HIGH)
    {
      button1_state = LOW;
    }


    if (debounceButton(button2_state, button_decr) == HIGH && button2_state == LOW) {
      num--;
      button2_state = HIGH;
      /* convert presses to binary and store it as a string */
      String binNumber = String(num, BIN);
      /* get the length of the string */
      int binLength = binNumber.length();
      if (num <= 64) {

        FastLED.clear();
        FastLED.show();
        for (int i = 0, x = 1; i < binLength; i++, x += 2) {

          if (binNumber[i] == '0') leds[i + binLength - x] = CRGB(0, 0, 0);
          if (binNumber[i] == '1') leds[i + binLength - x] = CRGB(255, 0, 0);
          FastLED.show();
        }
      }
      Serial.print("seq : ");
      Serial.println(num);
    }
    else if (debounceButton(button2_state, button_decr) == LOW && button2_state == HIGH)
    {
      button2_state = LOW;
    }

    myProgram = num;
    //   Serial.println(analogRead(potentionmetrePin));
    //Serial.println(myMedium);
    ; // wait for serial port to connect. Needed for Leonardo only
    float myIntensity = 255 * float(analogRead(potentionmetrePin)) / 1023;
    //     float myIntensity = 5;
    cli();

    for (int i = 0; i < 1; i++)
    {
      sendPixel( 0 , myIntensity , 0 );
    }

    sei();
    show();
  }

}

boolean debounceButton(boolean state, int button)
{
  boolean stateNow = digitalRead(button);
  if (state != stateNow)
  {
    delay(10);
    stateNow = digitalRead(button);
  }
  return stateNow;
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
