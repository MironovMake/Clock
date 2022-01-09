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

int segmentsixe[] = {4, 5, 4, 5, 5, 4, 5};
int k = 0;
// int SegmentsOfDigit[15][7] = {
int SegmentsOfDigit[15][7] = {
    {0, 1, 1, 1, 1, 1, 1}, // [0] 0
    {0, 1, 0, 0, 0, 0, 1}, // [1] 1
    {1, 1, 1, 0, 1, 1, 0}, // [2] 2
    {1, 1, 1, 0, 0, 1, 1}, // [3] 3
    {1, 1, 0, 1, 0, 0, 1}, // [4] 4
    {1, 0, 1, 1, 0, 1, 1}, // [5] 5
    {1, 0, 1, 1, 1, 1, 1}, // [6] 6
    {0, 1, 1, 0, 0, 0, 1}, // [7] 7
    {1, 1, 1, 1, 1, 1, 1}, // [8] 8
    {1, 1, 1, 1, 0, 1, 1}, // [9] 9
    {0, 0, 0, 0, 0, 0, 0}, // [10] off
    {0, 0, 1, 1, 1, 1, 0}, // [11] degrees symbol
    {1, 1, 1, 1, 0, 0, 0}, // [12] C(elsius)
    {1, 0, 1, 1, 1, 0, 0}, // [13] F(ahrenheit)
    {1, 0, 0, 0, 0, 0, 0}  // [14] minus
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

void printNumber(byte number, byte segment)
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

    k = 0;
    for (byte i = 0; i < 7; i++) // перебор сегментов
    {
        for (int j = 0; j < segmentsixe[i]; j++) // перебор диодов в сегменте
        {

            (SegmentsOfDigit[number][i]) ? leds[startindex + k] = scoreboardColorRight : leds[startindex + k] = alternateColor;
            //! Serial.println(i);
            k++;
            //   delay(50);
        }
    }

    /*
        switch (number)
        {
        case 0:
            for (byte i = 0; i < 4; i++)
            {
                leds[i + startindex] = alternateColor;
            }
            for (byte i = 4; i < 32; i++)
            {
                leds[i + startindex] = scoreboardColorRight;
            }
            break;
        case 1:
            for (byte i = 0; i < 4; i++)
            {
                leds[i + startindex] = alternateColor;
            }
            for (byte i = 4; i < 9; i++)
            {
                leds[i + startindex] = scoreboardColorRight;
            }
            for (byte i = 9; i < 28; i++)
            {
                leds[i + startindex] = alternateColor;
            }
            for (byte i = 27; i < 31; i++)
            {
                leds[i + startindex] = scoreboardColorRight;
            }
            break;
        }
        */
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
    //
    //    printNumber(1, 0);
    //    printNumber(2, 1);
    //    printNumber(3, 2);
    //    printNumber(4, 3);
    //    displayDots(1);
    //    FastLED.show();
    //  delay(1000);
}