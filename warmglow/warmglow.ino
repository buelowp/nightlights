#include <FastLED.h>

#define PIN1          6
#define NUM_LEDS      40
#define BRIGHTNESS    90

CRGB strip[NUM_LEDS];

HSVHue ColorWheel[] = {
  HUE_RED,
  HUE_YELLOW,
  HUE_BLUE,
  HUE_GREEN,
  HUE_ORANGE,
  HUE_PURPLE,
  HUE_PINK,
  HUE_AQUA,
};

void setup() {
  // put your setup code here, to run once:
  delay(3000);
  FastLED.addLeds<NEOPIXEL, PIN1>(strip, NUM_LEDS);
  randomSeed(analogRead(1));
}

void scale_pixel_down(CHSV &pixel)
{
  while (pixel.v) {
    if ((pixel.v - 2) < 0) {
      pixel.v = 0;
    }
    else {
      pixel.v -= 2;
    }
    delay(40);
    see_the_rainbow(pixel);
  }
}

void scale_pixel_up(CHSV &pixel)
{
  while (pixel.v < BRIGHTNESS) {
    if ((pixel.v + 2) > BRIGHTNESS) {
      pixel.v = BRIGHTNESS;
    }
    else {
      pixel.v += 2;
    }
    see_the_rainbow(pixel);;
    delay(40);
  }
}

void scale_pixel_home(CHSV &pixel)
{
  while (pixel.v != 100) {
    if ((pixel.v + 2) > BRIGHTNESS) {
      pixel.v = 100;
    }
    else {
      pixel.v += 2;
    }
    delay(20);
    see_the_rainbow(pixel);
  }
}

void glow()
{
  CHSV pixel;
  
  pixel.v = 0;
  pixel.h = ColorWheel[random(0, 7)];
  pixel.s = 255;
  
  scale_pixel_up(pixel);
  delay(5000);
  scale_pixel_down(pixel);
}

void transition_colors(CHSV &pixel)
{
  HSVHue hue = ColorWheel[random(0, 7)];
  
  while (pixel.h != hue) {
    if (pixel.h < hue) {
      pixel.h++;
    }
    else {
      pixel.h--;
    }
    see_the_rainbow(pixel);
    delay(20);
  }
}

void set_color(CHSV &pixel)
{
  pixel.v = BRIGHTNESS;
  pixel.h = ColorWheel[random(0,7)];
  pixel.s = 255;
}

void see_the_rainbow(CHSV &pixel)
{
  CHSV pixels[NUM_LEDS];
  
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels[i] = pixel;
  }
  hsv2rgb_rainbow(pixels, strip, NUM_LEDS);
  FastLED.show();
}

void loop() {
  CHSV pixel;
  
  set_color(pixel);
  
  while (1) {
    transition_colors(pixel);
    delay(5000);
  }
//  glow();
}
