/* This is the client-side (glove-side) code to run on a set of neopixel-based gloves, powered by
 *  an Adafruit Gemma or similar ATTiny85-based device in conjunction with an HC-06 bluetooth module.
 *  The bluetooth module communicates with the ATTiny via a SoftwareSerial. Data is read in 
 */

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

//Some general configurations for the gloves
#define NUMPIXELS 5       //set the number of pixels on each strip
#define BUFFERSIZE 15     //set the size of the data buffer to read from the phone
#define SERIALBAUD 38400  //set the baud rate for serial communication with the bluetooth adapter

//define the pins used
#define LED_PIN 1  //the data pin for the LED strip
#define BT_RX 0    //Connect bluetooth TX to this pin (green)
#define BT_TX 2    //Connect bluetooth RX to this pin (yellow)

#define SET_RGB_ALL 0x0A

//Some important variables
byte buf[BUFFERSIZE]; //this array of bytes stores the data from the bluetooth module each time data is read
struct light
{
  uint8_t r, g, b;
} fingers[NUMPIXELS];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial bt_data = SoftwareSerial(BT_RX,BT_TX);

void setup() {
  bt_data.begin(SERIALBAUD); //begin the serial connection
  bt_data.setTimeout(5);

  strip.begin(); //initialize the strip of neopixels
  for(int i=0;i<BUFFERSIZE;++i)
  {
    buf[i] = 0;
  }
  for(int i=0;i<NUMPIXELS;++i)
  {
    fingers[i].r = 0;
    fingers[i].g = 0;
    fingers[i].b = 0;
  }

  for(uint8_t i = 0;i<NUMPIXELS;++i)
  {
    strip.setPixelColor(i, 200, 100, 100);
  }
  strip.show();
  
}

void loop() {
  bt_data.write(0xFF);
  bt_data.write(0xCC);
  delay(1);
  if(bt_data.available())
  {
    bt_data.readBytes(buf, BUFFERSIZE); //read BUFFERSIZE bytes from the serial and store them into the buffer
  }
    
  /*switch(buf[BUFFERSIZE-1])
  {
    case SET_RGB_ALL:
      for(int i=0;i<NUMPIXELS;++i)
      {
        fingers[i].r = buf[i*3];
        fingers[i].g = buf[(i*3) + 1];
        fingers[i].b = buf[(i*3) + 2];
      }
      break;
    default:
      /*fingers[0].r = 255;
      fingers[0].g = 0;
      fingers[0].b = 0;
      break;
  }*/
  
  for(uint8_t i=0;i<NUMPIXELS;++i)
  {
    strip.setPixelColor(i, fingers[i].r, fingers[i].g, fingers[i].b);
  }
  strip.show();
}

/*void rainbowWheel()
{
    for(int i=0; i< NUMPIXELS; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / NUMPIXELS)) & 255));
    }
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

light Wheel(byte pos)
{
  light out;
  pos = 255 - pos;
  if(pos < 85)
  {
    out.r = 255 - pos * 3;
    out.g = 0;
    out.b = pos * 3;
    return out;
  } else if(pos < 170)
  {
    pos -= 85;
    out.r = 0;
    out.g = pos * 3;
    out.b = 255 - pos * 3;
    return out;
  } else
  {
    pos -= 170;
    out.r = pos * 3;
    out.g = 255 - pos * 3;
    out.b = 0;
  }
  
}
