#include "application.h"

#include "FastLED-sparkcore/firmware/FastLED.h"

SYSTEM_MODE(AUTOMATIC);

#define NUM_LEDS	    24
#define NORM_BRIGHT     80
#define PIXEL_PIN       D6
#define NL_COLORS		6
#define HALLO_COLORS	5
#define XMAS_COLORS		4


#define NO_PROGRAM          0
#define MIGRATE_PROGRAM     1
#define SPARKLE_PROGRAM     2
#define SPIN_PROGRAM        3
#define XMAS_PROGRAM        4
#define HLWN_PROGRAM        5

#define NL_VERSION			"1.0"

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

CRGB strip[NUM_LEDS];

int whichProgram;
int pixelBrightness;
CRGB pixelColor;
bool randomColor;
int lastColor;
bool bOff;
int lastMinute;

int setProgram(String p)
{
    if (p.equalsIgnoreCase("spin"))
        whichProgram = SPIN_PROGRAM;
    else if (p.equalsIgnoreCase("random"))
        whichProgram = MIGRATE_PROGRAM;
    else if (p.equalsIgnoreCase("sparkle"))
        whichProgram = SPARKLE_PROGRAM;
    else if (p.equalsIgnoreCase("halloween"))
        whichProgram = HLWN_PROGRAM;
    else if (p.equalsIgnoreCase("christmas"))
        whichProgram = XMAS_PROGRAM;
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

    if (newBright > 0) {
        pixelBrightness = newBright;
    }
    return pixelBrightness;
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
	FastLED.show();
}

int nextRandom(int low, int high)
{
    int next = random(low, high);
    while (next == lastColor)
        next = random(low, high);

    lastColor = next;
    return next;
}

void setPixel(int i, CRGB c, uint8_t b)
{
	pixelShutdown();
    FastLED.setBrightness(b);
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

    setPixel(curPixel++, c, pixelBrightness);
    FastLED.show();
    delay(1000);
}

/*
 * Light one pixel up white
 */
void sparkle()
{
    int pixel = random(0, 24);
    CRGB c;

    if (randomColor) {
        c = NightlightColorWheel[nextRandom(1, NL_COLORS)];
    }
    else {
        c = pixelColor;
    }

    for (int i = 0; i < pixelBrightness; i++) {
        setPixel(pixel, c, i);
        FastLED.show();
        delay(25);
    }
    for (int i = pixelBrightness; i >= 0; i--) {
        setPixel(pixel, c, i);
        FastLED.show();
        delay(25);
    }
}

/**
 * Light up one pixel with a random color
 */
void migrate()
{
    int index = random(0, NUM_LEDS);
    CRGB c;

    if (randomColor) {
        c = NightlightColorWheel[nextRandom(1, NUM_LEDS)];
    }
    else {
        c = pixelColor;
    }

    setPixel(index, c, pixelBrightness);
    FastLED.show();
    delay(random(4500, 9000));
}

void halloween()
{
    int pixel = random(0, NUM_LEDS);
    CRGB c;

    c = HalloweenColorWheel[nextRandom(0, HALLO_COLORS)];
    for (int i = 0; i < pixelBrightness; i++) {
        setPixel(pixel, c, i);
        FastLED.show();
        delay(25);
    }
    delay(random(5000, 8500));
    for (int i = pixelBrightness; i >= 0; i--) {
        setPixel(pixel, c, i);
        FastLED.show();
        delay(25);
    }
}

void christmas()
{
    int pixel = random(0, NUM_LEDS);
    CRGB c;

    c = ChristmasColorWheel[nextRandom(0, XMAS_COLORS)];
    for (int i = 0; i < pixelBrightness; i++) {
        setPixel(pixel, c, i);
        FastLED.show();
        delay(25);
    }
    delay(random(5000, 8500));
    for (int i = pixelBrightness; i >= 0; i--) {
        setPixel(pixel, c, i);
        FastLED.show();
        delay(25);
    }
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

void setup() {
    whichProgram = MIGRATE_PROGRAM;
    pixelBrightness = NORM_BRIGHT;
    pixelColor = CRGB::White;
    randomColor = true;
    bOff = false;

    Particle.function("program", setProgram);
    Particle.function("brightness", setBrightness);
    Particle.function("color", setColor);

	FastLED.addLeds<NEOPIXEL, D6>(strip, NUM_LEDS);

    lastMinute = Time.minute();
    Particle.publish("Startup", String("System Version: " + System.version() + ", Program Version: " + NL_VERSION));
}

void loop() {
    switch (whichProgram) {
        case MIGRATE_PROGRAM:
            migrate();
            break;
        case SPARKLE_PROGRAM:
            sparkle();
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
        case NO_PROGRAM:
            shutdown();
            break;
    }
    printHeartbeat();
}
