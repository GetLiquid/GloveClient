#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#define NUMPIXELS 5       //set the number of pixels on each strip
#define BUFFERSIZE 8      //set the size of the data buffer to read from the phone
#define SERIALBAUD 38400  //set the baud rate for serial communication with the bluetooth adapter

//define some codes to look for in the data received
#define START_CODE 0xFF      //each command starts with this code, if the buffer does not start with this ignore the command
#define END_CODE 0x0F        //each command ends with this code, if the buffer does not end with this ignore the command
#define SET_RGB 0x0A         //command to set the raw rgb value of all the pixels in the strip
#define SET_LOOP_DELAY 0x0B  //command to set the delay between callings of the loop() function
#define SET_CYCLE_JUMP 0x0C  //command to set the speed at which the colorWheel() function cycles through colors
#define SET_SINGLE_RGB 0x0D  

//define the pins used
#define LED_PIN 7  //the data pin for the LED strip
#define BT_RX 5    //the RX pin for the bluetooth module
#define BT_TX 6    //the TX pin for the bluetooth module

//stores the red, green, and blue of the pixels
byte r = 0;
byte g = 0;
byte b = 0;


byte buf[BUFFERSIZE]; //this array of bytes stores the data from the bluetooth module

byte loopWait = 0;       //how long to wait at the end of the loop() function before it is called again in milliseconds


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial bt_data = SoftwareSerial(BT_RX,BT_TX);

byte current_mode = 0;

void setup() {
  //Serial.begin(SERIALBAUD);
  bt_data.begin(SERIALBAUD);
  bt_data.setTimeout(10);

  strip.begin();
  //strip.setBrightness(64);
  strip.show();
}

void loop() {
  bt_data.readBytes(buf, BUFFERSIZE);
  
  
  if(buf[0] == START_CODE && buf[BUFFERSIZE-1] == END_CODE)
  {
    switch(buf[1])
    {
      case SET_RGB:
        r = buf[2];
        g = buf[3];
        b = buf[4];
        for(int i=0;i<NUMPIXELS;++i)
        {
          strip.setPixelColor(i, g, r, b);
        }
        break;
        
      case SET_LOOP_DELAY:
        loopWait = buf[2];
        break;
        
      case SET_CYCLE_JUMP:
        current_mode = buf[3];
        //cycleVelocity = buf[2];
        break;

      case SET_SINGLE_RGB:
        //cycle = false;
        /*for(int i=0;i<NUMPIXELS;++i)
        {
          if(i == buf[5])
            strip.setPixelColor(buf[5], buf[3], buf[2], buf[4]);
          else
            strip.setPixelColor(i,0,0,0);
        }*/

        strip.setPixelColor(buf[5], buf[3], buf[2], buf[4]);
      default:
        break;
    }
    buf[0] = 0x0;
  }
  
  strip.show();
  delay(loopWait);
}

/*void rainbowWheel()
{
    for(int i=0; i< NUMPIXELS; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / NUMPIXELS) + cycleOffset) & 255));
    }
    cycleOffset += cycleVelocity;
}

void colorWheel()
{
  for(int i=0;i<NUMPIXELS;++i)
  {
    strip.setPixelColor(i, Wheel(cycleOffset & 255));
  }
  cycleOffset += cycleVelocity;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
   }
  else if(WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  else
  {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}*/
