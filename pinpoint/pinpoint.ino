#include <FastLED.h>

#define PIN1          6
#define NUM_LEDS      40
#define BRIGHTNESS    255

CRGB strip[NUM_LEDS];

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
  // put your setup code here, to run once:
  delay(3000);
  FastLED.addLeds<NEOPIXEL, PIN1>(strip, NUM_LEDS);
  randomSeed(analogRead(1));
  Serial.begin(9600);
}

void see_the_rainbow(CHSV &pixel, int index)
{
  CHSV pixels[NUM_LEDS];
  CHSV black;
  
  if ((index % 8) == 7) {
    index = index - 1;
  }
  
  black.v = 0;
  black.h = 0;
  black.s = 0;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels[i] = black;
  }
  pixels[index] = pixel;
  pixels[index + 1] = pixel;
  pixels[index + 8] = pixel;
  pixels[index + 9] = pixel;
  
  hsv2rgb_rainbow(pixels, strip, NUM_LEDS);
  FastLED.show();
}

void loop() {
  int location = random(0, 31);
  CHSV pixel;
  
  Serial.print("location is ");
  Serial.println(location);
  
  pixel.v = BRIGHTNESS;
  pixel.h = ColorWheel[random(0, 8)];
  pixel.s = 255;
  
  see_the_rainbow(pixel, location);
  delay(5000);
}
