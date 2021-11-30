#include <FastLED.h>

#define LED_PIN 5
#define NUM_LEDS 58
#define BRIGHTNESS 100
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGB scoreboardColorRight = CRGB::Red;
CRGB alternateColor = CRGB::Black;
#define UPDATES_PER_SECOND 100
byte hourFormat=24;
long numbers[] = {
    0b00111111111111, // [0] 0
    0b00110000000011, // [1] 1
    0b11111100111100, // [2] 2
    0b11111100001111, // [3] 3
    0b11110011000011, // [4] 4
    0b11001111001111, // [5] 5
    0b11001111111111, // [6] 6
    0b00111100000011, // [7] 7
    0b11111111111111, // [8] 8
    0b11111111001111, // [9] 9
    0b00000000000000, // [10] off
    0b11111111000000, // [11] degrees symbol
    0b00001111111100, // [12] C(elsius)
    0b11001111110000, // [13] F(ahrenheit)
};
void displayDots(bool dotsOn)
{
    if (dotsOn)
    {
        leds[28] = CRGB::Red;
        leds[29] = CRGB::Red;
    }
    else
    {
        leds[28] = CRGB::Black;
        leds[29] = CRGB::Black;
    }

    dotsOn = !dotsOn;
}

void displayNumber(byte number, byte segment)
{
    /*
   *
      __ __ __        __ __ __          __ __ __        12 13 14
    __        __    __        __      __        __    11        15
    __        __    __        __      __        __    10        16
    __        __    __        __  42  __        __    _9        17
      __ __ __        __ __ __          __ __ __        20 19 18
    __        65    __        44  43  __        21    _8        _0
    __        __    __        __      __        __    _7        _1
    __        __    __        __      __        __    _6        _2
      __ __ __       __ __ __           __ __ __       _5 _4 _3
   */

    // segment from left to right: 3, 2, 1, 0

    /*
   *
       52 53          38 39            22 23           _8 _9
     51      54     37      40       21      24      _7      10
     50      55     36      41  28   20      25      _6      11
        13 12          43 42            27 26           13 12
     49      44     35      30  29   19      14      _5      _0
     48      45     34      31       18      15      _4      _1
        47 46          33 32            17 16           _3 _2
   */
    byte startindex = 0;
    switch (segment)
    {
    case 0:
        startindex = 0;
        break;
    case 1:
        startindex = 14;
        break;
    case 2:
        startindex = 30;
        break;
    case 3:
        startindex = 44;
        break;
    }

    for (byte i = 0; i < 14; i++)
    {
        leds[i + startindex] = ((numbers[number] & 1 << i) == 1 << i) ? scoreboardColorRight : alternateColor;
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

void updateClock()
{
    DateTime now = rtc.now();
    // printDateTime(now);

    int hour = now.hour();
    int mins = now.minute();
    int secs = now.second();

    if (hourFormat == 12 && hour > 12)
        hour = hour - 12;

    byte h1 = hour / 10;
    byte h2 = hour % 10;
    byte m1 = mins / 10;
    byte m2 = mins % 10;
    byte s1 = secs / 10;
    byte s2 = secs % 10;

    CRGB color = CRGB(r_val, g_val, b_val);
    /*
  if (h1 > 0)
    displayNumber(h1, 3, color);
  else
    displayNumber(10, 3, color); // Blank

  displayNumber(h2, 2, color);
  displayNumber(m1, 1, color);
  displayNumber(m2, 0, color);
*/
    displayNumber(1, 2, color);
    displayNumber(2, 1, color);
    displayNumber(3, 0, color);
    displayDots(color);
}
void setup()
{
    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = CRGB::Green;

    FastLED.show();
    displayDots(1);
    FastLED.show();
    displayNumber(0, 0);
    FastLED.show();
}

void loop()
{
}