#include <FastLED.h>

#define PIN1          6
#define NUM_LEDS      40
#define BRIGHTNESS    255

CRGB strip[NUM_LEDS];
CHSV oldPixel;
int oldLocation;

HSVHue ColorWheel[] = {
  HUE_RED,
  HUE_YELLOW,
  HUE_BLUE,
  HUE_GREEN,
  HUE_ORANGE,
  HUE_PURPLE,
  HUE_PINK,
  HUE_AQUA
};

void setup() {
  delay(3000);
  FastLED.addLeds<NEOPIXEL, PIN1>(strip, NUM_LEDS);
  randomSeed(analogRead(1));
  Serial.begin(9600);
  oldLocation = 0;
}

void paint_canvas_black()
{
  for (int i = 0; i < NUM_LEDS; i++) {
    strip[i] = CRGB::Black;
  }
  FastLED.show();
}

void swap_pixels(CHSV &pixel, int location)
{
  CHSV pixels[NUM_LEDS];
  CHSV blackPixel;
  int newPix[4];
  int oldPix[4];
  int pos;
  
  blackPixel.v = 0;
  blackPixel.s = 0;
  blackPixel.h = 0;
  
  paint_canvas_black();
  
  newPix[0] = location;
  newPix[1] = location + 1;
  newPix[2] = location + 8;
  newPix[3] = location + 9;
  oldPix[0] = oldLocation;
  oldPix[1] = oldLocation + 1;
  oldPix[2] = oldLocation + 8;
  oldPix[3] = oldLocation + 9;

  for (int i = 0; i < NUM_LEDS; i++) {
    pixels[i] = blackPixel;
  }

  while (pixel.v < 255) {
    pixel.v++;
    oldPixel.v--;
    
    for (int i = 0; i < 4; i++) {
      pixels[oldPix[i]] = oldPixel;
      pixels[newPix[i]] = pixel;
    }

    hsv2rgb_rainbow(pixels, strip, NUM_LEDS);
    FastLED.show();
    delay(20);
  }
}

void loop() {
  int location = random(0, 31);
  CHSV pixel;
  
  if (location == oldLocation) {
    location = random(0, 31);
  }
  
  if ((location % 8) == 7)
    location++;
    
  pixel.v = 0;
  pixel.h = ColorWheel[random(0, 8)];
  pixel.s = 255;

  swap_pixels(pixel, location);

  oldPixel = pixel;
  oldLocation = location;

  delay(10000);
}
