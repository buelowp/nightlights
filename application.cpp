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

#define NL_VERSION			"1.1"

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
        case NO_PROGRAM:
            shutdown();
            break;
    }
    printHeartbeat();
}
