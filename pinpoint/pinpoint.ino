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

void swap_pixels(CHSV &pixel, int newLocation)
{
  CHSV pixels[NUM_LEDS];
  CHSV blackPixel;
  int pos;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    memset(&pixels[i], 0, sizeof(CHSV));
  }
  
  while (pixel.v < 255) {
    pixel.v++;
    oldPixel.v--;
    
    pixels[oldLocation] = oldPixel;
    pixels[newLocation] = pixel;

    hsv2rgb_rainbow(pixels, strip, NUM_LEDS);
    FastLED.show();
    delay(20);
  }
}

void loop() {
  int location = random(0, 31);
  CHSV pixel;
  
  if (location == oldLocation) {
    location = random(0, 39);
  }
  
  pixel.v = 0;
  pixel.h = ColorWheel[random(0, 8)];
  pixel.s = 255;

  swap_pixels(pixel, location);

  oldPixel = pixel;
  oldLocation = location;

  delay(10000);
}
