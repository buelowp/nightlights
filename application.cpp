#include "application.h"

#include "FastLED-sparkcore/firmware/FastLED.h"

SYSTEM_MODE(AUTOMATIC);

#define NUM_LEDS	    24
#define NORM_BRIGHT     80
#define PIXEL_PIN       D6
#define NL_COLORS		6
#define HALLO_COLORS	5
#define XMAS_COLORS		4
#define VDAY_COLORS		5
#define IDAY_COLORS		3

#define NO_PROGRAM          0
#define MIGRATE_PROGRAM     1
#define SPARKLE_PROGRAM     2
#define SPIN_PROGRAM        3
#define XMAS_PROGRAM        4
#define HLWN_PROGRAM        5
#define VDAY_PROGRAM		6
#define IDAY_PROGRAM		7
#define METEOR_PROGRAM	8

#define NL_VERSION			"1.2"

using namespace NSFastLED;

static CRGB HalloweenColorWheel[] = {
	CRGB::Red,
	CRGB::OrangeRed,
	CRGB::Purple,
	CRGB::Green,
	CRGB::Blue,
};

static CRGB NightlightColorWheel[] = {
		CRGB::Blue,
		CRGB::Green,
		CRGB::Yellow,
		CRGB::Pink,
		CRGB::Orange,
		CRGB::Aqua,
};

static CRGB ChristmasColorWheel[] = {
		CRGB::Red,
		CRGB::Yellow,
		CRGB::Blue,
		CRGB::Green
};

static CRGB ValentinesColorWheel[] = {
		CRGB::HotPink,
		CRGB::Pink,
		CRGB::FloralWhite,
		CRGB::Lavender,
		CRGB::MistyRose,
};

static CRGB IDayColorWheel[] = {
		CRGB::LightSalmon,
		CRGB::LightBlue,
		CRGB::AntiqueWhite,
};

CRGB strip[NUM_LEDS];

int whichProgram;
CRGB pixelColor;
bool randomColor;
bool progOverride;
int lastColor;
bool bOff;
int lastMinute;

byte ledsX[NUM_LEDS][3];
boolean RAINBOWs = false;
boolean RANDOMpick = false;
uint8_t hue;
uint16_t timeframe;
byte idex = 0;
byte colorTIP = 0;
byte meteorLENGTH;
byte loopCount = 1;    //low number loop counter

//find the adjacent counter clockwise postion of the led
//funkboxing code snippet
byte adjacent_ccw(byte i) {
  byte r;
  if (i > 0) { r = i - 1; }
  else { r = NUM_LEDS - 1; }
  return r;
}

void meteorShower(){
  //hue master
  hue++;

  //populate the strip[] with stored ledsX[] array data
  for(byte i = 0; i < NUM_LEDS; i++ ) {
    ledsX[i][0] = strip[i].r;
    ledsX[i][1] = strip[i].g;
    ledsX[i][2] = strip[i].b;
  }
  //clear the previous counter clockwise position
  byte iCCW;

  //we are keeping track of elapsed time
  timeframe++;    //fx timer

  //meteorLENGTH fx is only shown for this time frame
  if((timeframe >= 1) && (timeframe <= 280)) { meteorLENGTH = 29; }
  if((timeframe > 280) && (timeframe <= 500)) { meteorLENGTH = 45; }

  //RAINBOWs fx add rainbow tails during this time frame only
  if((timeframe > 0) && (timeframe <= 280)) { RAINBOWs = true; }
  else{ RAINBOWs = false; }
  //keep our RAINBOWs within a specific range of hue
  if(RAINBOWs == true){ hue = hue - 20; if(hue <= 0){  hue = 1; } }

  //RANDOMpick fx is only enabled during this timeframe
  if((timeframe > 600) && (timeframe <= 790)) { RANDOMpick = true; }
  else{ RANDOMpick = false; }

  //pick a random spot in the meteor switch case statement below
  if (RANDOMpick == true){ idex = random8(46); }
  else{
    //increment the meteor display frame
    idex++;
    //make sure we don't drift into space
    if (idex > meteorLENGTH) { idex = 0; } }

  //meteorLENGTH is randomized during this timeframe only
  if((timeframe > 790) && (timeframe <= 1090)) { meteorLENGTH = random8(7, 38); }

  //during this point in the animation timeframe
  if(timeframe == 1180) {
    //reset the timeframe
    timeframe = 0;
    //increment the loop counter
    loopCount++;
  }

  //during this part of the loopCount, all meteors have a white colored tip
  if(loopCount == 1) { colorTIP = 0; }
  if(loopCount == 2) { colorTIP = 1; }
  if(loopCount == 3) { colorTIP = random8(11); }
  //end of the desired fx, reset the variable for the next time around
  if(loopCount == 4) {
    colorTIP = 0;
    loopCount = 0;
  }

  //there are two switch case statements nestled into one another
  //we always want to control the color of the meteor tip
  //the other controls the actual meteor animation in 45 frames/case statements
  switch (idex) {
  case 0:
    switch (colorTIP){
    case 0:
      strip[0] = CHSV(hue, 255, 255);
      break;
    case 1:
      strip[0] = CRGB(100,100,100);
      break;
    case 2:
      strip[0] = CRGB::Yellow;
      break;
    case 3:
      strip[0] = CRGB::Violet;
      break;
    case 4:
      strip[0] = CRGB::Green;
      break;
    case 5:
      strip[0] = CRGB::Purple;
      break;
    case 6:
      strip[0] = CRGB::Orange;
      break;
    case 7:
      strip[0] = CRGB::Cyan;
      break;
    case 8:
      strip[0] = CRGB::GreenYellow;
      break;
    case 9:
      strip[0] = CRGB::Magenta;
      break;
    case 10:
      strip[0] = CRGB::SkyBlue;
    }
    break;
  case 1:
    strip[0] = CHSV((hue - 20), 255, 210);
    break;
  case 2:
    strip[0] = CHSV((hue - 22), 255, 180);
    break;
  case 3:
    strip[0] = CHSV((hue - 23), 255, 150);
    break;
  case 4:
    strip[0] = CHSV((hue - 24), 255, 110);
    break;
  case 5:
    strip[0] = CHSV((hue - 25), 255, 90);
    break;
  case 6:
    strip[0] = CHSV((hue - 26), 160, 60);
    break;
  case 7:
    strip[0] = CHSV((hue - 27), 140, 40);
    break;
  case 8:
    strip[0] = CHSV((hue - 28), 120, 20);
    break;
  case 9:
    strip[0] = CHSV((hue - 29), 100, 20);
    break;
  case 10:
    strip[0] = CRGB::Black;
    break;
  case 11:
    strip[0] = CRGB::Black;
    break;
  case 12:
    strip[0] = CRGB::Black;
    break;
  case 13:
    strip[0] = CRGB::Black;
    break;
  case 14:
    strip[0] = CRGB::Black;
    break;
  case 15:
    strip[0] = CRGB::Black;
    break;
  case 16:
    strip[0] = CRGB::Black;
    break;
  case 17:
    strip[0] = CRGB::Black;
    break;
  case 18:
    strip[0] = CRGB::Black;
    break;
  case 19:
    strip[0] = CRGB::Black;
    break;
  case 20:
    strip[0] = CRGB::Black;
    break;
  case 21:
    strip[0] = CRGB::Black;
    break;
  case 22:
    strip[0] = CRGB::Black;
    break;
  case 23:
    strip[0] = CRGB::Black;
    break;
  case 24:
    strip[0] = CRGB::Black;
    break;
  case 25:
    strip[0] = CRGB::Black;
    break;
  case 26:
    strip[0] = CRGB::Black;
    break;
  case 27:
    strip[0] = CRGB::Black;
    break;
  case 28:
    strip[0] = CRGB::Black;
    break;
  case 29:
    strip[0] = CRGB::Black;
    break;
  case 30:
    strip[0] = CRGB::Black;
    break;
  case 31:
    strip[0] = CRGB::Black;
    break;
  case 32:
    strip[0] = CRGB::Black;
    break;
  case 33:
    strip[0] = CRGB::Black;
    break;
  case 34:
    strip[0] = CRGB::Black;
    break;
  case 35:
    strip[0] = CRGB::Black;
    break;
  case 36:
    strip[0] = CRGB::Black;
    break;
  case 37:
    strip[0] = CRGB::Black;
    break;
  case 38:
    strip[0] = CRGB::Black;
    break;
  case 39:
    strip[0] = CRGB::Black;
    break;
  case 40:
    strip[0] = CRGB::Black;
    break;
  case 41:
    strip[0] = CRGB::Black;
    break;
  case 42:
    strip[0] = CRGB::Black;
    break;
  case 43:
    strip[0] = CRGB::Black;
    break;
  case 44:
    strip[0] = CRGB::Black;
    break;
  case 45:
    strip[0] = CRGB::Black;
    break;
  }
  //copy the LED Array
  for(byte i = 1; i < NUM_LEDS; i++ ) {
    iCCW = adjacent_ccw(i);
    strip[i].r = ledsX[iCCW][0];
    strip[i].g = ledsX[iCCW][1];
    strip[i].b = ledsX[iCCW][2];
  }
  //show the blinky
  FastLED.show();
  //control the animation speed/frame rate
  delay(10);
}

int setProgram(String p)
{
	if (p.equalsIgnoreCase("random")) {
		whichProgram = MIGRATE_PROGRAM;
		progOverride = false;
	}
    if (p.equalsIgnoreCase("spin")) {
        whichProgram = SPIN_PROGRAM;
        progOverride = true;
    }
    else if (p.equalsIgnoreCase("random")) {
        whichProgram = MIGRATE_PROGRAM;
        progOverride = true;
    }
    else if (p.equalsIgnoreCase("sparkle")) {
        whichProgram = SPARKLE_PROGRAM;
        progOverride = true;
    }
    else if (p.equalsIgnoreCase("halloween")) {
        whichProgram = HLWN_PROGRAM;
        progOverride = true;
    }
    else if (p.equalsIgnoreCase("christmas")) {
        whichProgram = XMAS_PROGRAM;
        progOverride = true;
    }
    else if (p.equalsIgnoreCase("meteor")) {
    	whichProgram = METEOR_PROGRAM;
    	progOverride = true;
    }
    else if (p.equalsIgnoreCase("off")) {
        whichProgram = NO_PROGRAM;
    }

    if (whichProgram != NO_PROGRAM)
        bOff = false;

    return whichProgram;
}

int setBrightness(String b)
{
    int newBright = b.toInt();

    if (newBright > 0 && newBright < 255) {
    	FastLED.setBrightness(newBright);
        return newBright;
    }
    return -1;
}

int setColor(String c)
{
    int newColor = c.toInt();
    if (newColor == 0)
        return 0;

    if (newColor == 9999) {
        randomColor = true;
        return 9999;
    }

    randomColor = false;
    pixelColor = NightlightColorWheel[newColor];
    return newColor;
}

void pixelShutdown()
{
	for (int j = 0; j < NUM_LEDS; j++) {
		strip[j] = CRGB::Black;
	}
}

int nextRandom(int low, int high)
{
    int next = random(low, high);
    while (next == lastColor)
        next = random(low, high);

    lastColor = next;
    return next;
}

void setPixel(int i, CRGB c)
{
	pixelShutdown();
    strip[i] = c;
}

void spin()
{
    CRGB c;
    static int curPixel = 0;

    if (randomColor) {
        c = NightlightColorWheel[nextRandom(1, NL_COLORS)];
    }
    else {
        c = pixelColor;
    }

    if (curPixel >= NUM_LEDS)
        curPixel = 0;

    setPixel(curPixel++, c);
    FastLED.show();
    delay(1000);
}

/**
 * Light up one pixel with a random color
 */
void migrate()
{
    int index = random(0, NUM_LEDS);
    CRGB c;

    if (randomColor) {
        c = NightlightColorWheel[nextRandom(1, NL_COLORS)];
    }
    else {
        c = pixelColor;
    }

    pixelShutdown();
    strip[index] = c;
    FastLED.show();
    delay(random(4500, 9000));
}

void halloween()
{
    int index = random(0, NUM_LEDS);
    CRGB c;

    if (randomColor) {
        c = HalloweenColorWheel[nextRandom(1, HALLO_COLORS)];
    }
    else {
        c = pixelColor;
    }

    pixelShutdown();
    strip[index] = c;
    FastLED.show();
    delay(random(4500, 9000));
}

void christmas()
{
    int index = random(0, NUM_LEDS);
    CRGB c;

    if (randomColor) {
        c = ChristmasColorWheel[nextRandom(1, XMAS_COLORS)];
    }
    else {
        c = pixelColor;
    }

    pixelShutdown();
    strip[index] = c;
    FastLED.show();
    delay(random(4500, 9000));
}

void vday()
{
    int index = random(0, NUM_LEDS);
    CRGB c;

    if (randomColor) {
        c = ValentinesColorWheel[nextRandom(1, VDAY_COLORS)];
    }
    else {
        c = pixelColor;
    }

    pixelShutdown();
    strip[index] = c;
    FastLED.show();
    delay(random(4500, 9000));
}

void iday()
{
    int index = random(0, NUM_LEDS);
    CRGB c;

    if (randomColor) {
        c = IDayColorWheel[nextRandom(1, IDAY_COLORS)];
    }
    else {
        c = pixelColor;
    }

    pixelShutdown();
    strip[index] = c;
    FastLED.show();
    delay(random(4500, 9000));
}

void shutdown()
{
    if (!bOff) {
        pixelShutdown();
        bOff = true;
    }
}

void printHeartbeat()
{
    if (lastMinute == 59 && Time.minute() >= 0) {
        Particle.publish("Heartbeat", String("System Version: " + System.version() + ", Program Version: " + NL_VERSION));
        lastMinute = Time.minute();
    }

    if (Time.minute() >= lastMinute + 1) {
        Particle.publish("Heartbeat", String("System Version: " + System.version() + ", Program Version: " + NL_VERSION));
        lastMinute = Time.minute();
    }
}

int setProgram()
{
	if (progOverride)
		return whichProgram;

	if (Time.month() == 12) {
		if (Time.day() > 22 && Time.day() < 27)
			return XMAS_PROGRAM;
	}
	if (Time.month() == 2) {
		if (Time.day() == 14)
			return VDAY_PROGRAM;
	}
	if (Time.month() == 7) {
		if (Time.day() == 4)
			return IDAY_PROGRAM;
	}
	if (Time.month() == 10) {
		if (Time.day() > 27)
			return HLWN_PROGRAM;
	}
	return MIGRATE_PROGRAM;
}

void setup() {
    whichProgram = MIGRATE_PROGRAM;
    pixelColor = CRGB::White;
    randomColor = true;
    bOff = false;

    Particle.function("program", setProgram);
    Particle.function("brightness", setBrightness);
    Particle.function("color", setColor);

	FastLED.addLeds<NEOPIXEL, D6>(strip, NUM_LEDS);
	FastLED.setBrightness(NORM_BRIGHT);

    lastMinute = Time.minute();
    Particle.publish("Startup", String("System Version: " + System.version() + ", Program Version: " + NL_VERSION));
}

void loop() {
	whichProgram = setProgram();
    switch (whichProgram) {
        case MIGRATE_PROGRAM:
            migrate();
            break;
        case SPIN_PROGRAM:
            spin();
            break;
        case HLWN_PROGRAM:
            halloween();
            break;
        case XMAS_PROGRAM:
            christmas();
            break;
        case VDAY_PROGRAM:
        	vday();
        	break;
        case IDAY_PROGRAM:
        	vday();
        	break;
        case METEOR_PROGRAM:
        	meteorShower();
        	break;
        case NO_PROGRAM:
            shutdown();
            break;
    }
    printHeartbeat();
}
