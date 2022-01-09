#include <FastLED.h>
#define LED_PIN 5
#define NUM_LEDS 130
#define BRIGHTNESS 255
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGB scoreboardColorRight = CRGB::Green;
CRGB alternateColor = CRGB::Black;
#define UPDATES_PER_SECOND 100
long numbersDig[] = {
    0b111111111110000, // [0] 0
    0b111111111111111, // [1] 0
    0b111100000111111, // [2] 0
    0b000000111110000, // [3] 1
    0b111000000000000, // [4] 1
    0b000000000000111, // [5] 1
    0b111111111111111, // [6] 2
    0b000111111111000, // [7] 2
    0b111111111111100, // [8] 2
    0b001111111111111, // [9] 3
    0b111111100000000, // [10] 3
    0b111111110000011, // [11] 3
    0b110000111111111, // [12] 4
    0b111000000000111, // [13] 4
    0b001111111111111, // [14] 4
    0b111111000001111, // [15] 5
    0b111111100000111, // [16] 5
    0b111111111111111, // [17] 5
    0b111111000001111, // [18] 6
    0b111111111111111, // [19] 6
    0b000000000000011, // [20] 6
    0b001111111110000, // [21] 7
    0b111000000000000, // [22] 7
    0b111111110011111, // [23] 7
    
    0b111111111111111, // [24] 8
    0b111111111111111, // [25] 8
    0b111111111111111, // [26] 8

    0b111111111111111, // [24] 9
    0b111111100000111, // [25] 9
    0b111111111111111, // [26] 9

    0b00000000000000, // [24] 10
    0b00000000000000, // [25] 10
    0b00000000000000, // [26] 10

    0b111111111111111, // [25] 11 //! о
    0b000000000000111, // [26] 11
    0b000000000000000, // [27] 11

    0b111111000000000, // [25] 12  //!С
    0b000111111111111, // [26] 12
    0b110011111100000, // [27] 12

    0b111111111111111, // [25] 13
    0b111111111111111, // [26] 13
    0b110011111100000, // [27] 13

    0b000000000001111, // [25] 14  //! -
    0b000000000000000, // [26] 14
    0b110011111100000, // [27] 14
};

void displayDots(bool dotsOn)
{
    if (dotsOn)
    {
        leds[64] = CRGB::Green;
        leds[65] = CRGB::Green;
    }
    else
    {
        leds[64] = CRGB::Black;
        leds[65] = CRGB::Black;
    }

    dotsOn = !dotsOn;
}

void displayNumber(byte number, byte segment)
{

    byte startindex = 0;
    switch (segment)
    {
    case 0:
        startindex = 0;
        break;
    case 1:
        startindex = 32;
        break;
    case 2:
        startindex = 66;
        break;
    case 3:
        startindex = 98;
        break;
    }

    for (int i = 0; i < 15; i++)
    {
        leds[i + startindex] = ((numbersDig[number * 3] & 1 << i) == 1 << i) ? scoreboardColorRight : alternateColor;
    }

    for (int i = 0; i < 15; i++)
    {
        leds[i + startindex + 15] = ((numbersDig[number * 3 + 1] & 1 << i) == 1 << i) ? scoreboardColorRight : alternateColor;
    }

    for (int i = 0; i < 2; i++)
    {
        leds[i + startindex + 30] = ((numbersDig[number * 3 + 2] & 1 << i) == 1 << i) ? scoreboardColorRight : alternateColor;
    }
}

void allBlank()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void setupLed()
{
    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CRGB::Green;

    FastLED.show();
    // Serial.println();
    // for (int j = 0; j < 20; j++)
    // {
    //     for (int i = 0; i < 15; i++)
    //     {
    //         Serial.print(((numbersDig[j] & 1 << i) == 1 << i));
    //     }
    //     Serial.println();
    // }
}