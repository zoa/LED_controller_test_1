#include <Adafruit_WS2801.h>
#include <Audio_monitor.h>
#include <MsTimer2.h>
#include <SPI.h>

int dataPin  = 2;
int clockPin = 3;

Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);
void setup() {

strip.begin();
strip.show();

}

void loop(){
  /*
colorWipe(Color(255, 0, 0), 50);  // red fill
colorWipe(Color(0, 255, 0), 50);  // green fill
colorWipe(Color(0, 0, 255), 50);  // blue fill*/


rainbowCycle(20);

}

void rainbowCycle(uint8_t wait) {
  int i, j;

  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j)% 256) );
    }
    strip.show();   // write all the pixels out
    
    // calculate delay based on current audio level
    Audio_monitor& monitor = Audio_monitor::instance();
    wait = (1023 - monitor.get_amplitude()) / 10;
    
    if ( monitor.is_anomolously_loud() )
    {
      colorWipe( Color(255, 0, 0), 50 );
    }
    
    delay(wait);
  }
}

//helper functions

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
