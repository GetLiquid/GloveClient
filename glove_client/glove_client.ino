/* This is the client-side (glove-side) code to run on a set of neopixel-based gloves, powered by
 *  an Adafruit Gemma or similar ATTiny85-based device in conjunction with an HC-06 bluetooth module.
 *  The bluetooth module communicates with the ATTiny via a SoftwareSerial. Data is read in 
 */

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

//Some general configurations for the gloves
#define NUMPIXELS 5        //set the number of pixels on each strip
#define BUFFERSIZE 4       //set the size of the data buffer to read from the phone
#define SERIALBAUD 115200  //set the baud rate for serial communication with the bluetooth adapter

//define the pins used
#define GEMMA
#ifdef GEMMA

#define LED_PIN 1  //the data pin for the LED strip
#define BT_RX 0    //Connect bluetooth TX to this pin (green)
#define BT_TX 2    //Connect bluetooth RX to this pin (yellow)

#else

#define LED_PIN 4
#define BT_RX   3
#define BT_TX   5

#endif

#define START_CODE  0xCF
#define END_CODE    0xFC
#define SET_RGB_ALL 0x0A

//Some important variables
byte buf[BUFFERSIZE]; //this array of bytes stores the data from the bluetooth module each time data is read
byte new_data = 0;
byte flash = 0;
boolean rainbow = false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial bt_data = SoftwareSerial(BT_RX,BT_TX);

void setup() {

#ifndef GEMMA
  Serial.begin(SERIALBAUD);
  //Serial.setTimeout(10);
#endif
  
  bt_data.begin(SERIALBAUD); //begin the serial connection
  //bt_data.setTimeout(10);

  strip.begin();
  for(int i=0;i<3;++i)
  {
    for(uint8_t i = 0;i<NUMPIXELS;++i)
    {
      strip.setPixelColor(i, 255, 255, 255);
    }
    strip.show();
    delay(500);
    for(uint8_t i = 0;i<NUMPIXELS;++i)
    {
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    delay(500);
  }
  strip.show();
}

void loop() {
  //recv_data();
  //display_lights();
  /*if(bt_data.available() > 0)
  {
    switch(bt_data.read())
    {
      case 0xFF:
        rainbow = true;
        break;
      case 0x00:
        rainbow = false;
        break;
      default:
        break;
    }
  }
  if(rainbow)
    rainbowCycle(3);
  else
  {
    for(int i=0;i<255;++i)
    {
      uint32_t color = Wheel(i);
      for(int j=0;j<NUMPIXELS;++j)
      {
        strip.setPixelColor(j, color);
      }
      strip.show();
      delay(3);
    }
    //delay(3);
  }*/
  rainbowCycle(3);
}
/*
void recv_data() {
    static byte inProgress = 0;
    static byte index = 0;
    byte incoming;
 
    while (bt_data.available() > 0 && !new_data) {
        incoming = bt_data.read();

        if (inProgress) {
            if (incoming != END_CODE) {
                buf[index] = incoming;
                ++index;
                if (index >= BUFFERSIZE) {
                    index = BUFFERSIZE - 1;
                }
            }
            else {
                inProgress = false;
                index = 0;
                new_data = true;
            }
        }

        else if (incoming == START_CODE) {
            inProgress = true;
        }
    }
}*/

/*void print_data()
{
  if(new_data)
  {
//    Serial.print("Received... ");
    for(int i=0;i<BUFFERSIZE;++i)
    {
//      Serial.print(buf[i], HEX);
//      Serial.print(' ');
    }
//    Serial.print('\n');
  }
}*/

/*void display_lights()
{
  if(new_data)
  {
    for(int i=0;i<NUMPIXELS;++i)
    {
      strip.setPixelColor(i, buf[(i*3)], buf[(i*3)+1], buf[(i*3)+2]);
    }
    //strip.setPixelColor(buf[3], buf[1], buf[0], buf[2]);
    new_data = false;
    strip.show();
  }
}*/

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUMPIXELS; ++i) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}



