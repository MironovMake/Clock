#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h> // Include RTC library by Makuna: https://github.com/Makuna/Rtc
RTC_DS1307 rtc;

#include <FastLED.h>
#define countof(a) (sizeof(a) / sizeof(a[0]))
#define NUM_LEDS 58 // Total of 86 LED's
#define DATA_PIN 5  // Change this if you are using another type of ESP board than a WeMos D1 Mini
#define MILLI_AMPS 2400
#define COUNTDOWN_OUTPUT 9

CRGB LEDs[NUM_LEDS];

// Settings
unsigned long prevTime = 0;
byte r_val = 255;
byte g_val = 0;
byte b_val = 0;
bool dotsOn = true;
byte brightness = 255;
float temperatureCorrection = -3.0;
byte temperatureSymbol = 12; // 12=Celcius, 13=Fahrenheit check 'numbers'
byte clockMode = 0;          // Clock modes: 0=Clock, 1=Countdown, 2=Temperature, 3=Scoreboard
unsigned long countdownMilliSeconds;
unsigned long endCountDownMillis;
byte hourFormat = 24; // Change this to 12 if you want default 12 hours format instead of 24
CRGB countdownColor = CRGB::Green;
byte scoreboardLeft = 0;
byte scoreboardRight = 0;
CRGB scoreboardColorLeft = CRGB::Green;
CRGB scoreboardColorRight = CRGB::Red;
CRGB alternateColor = CRGB::Black;
long numbers[] = {
    0b000111111111111111111, // [0] 0
    0b000111000000000000111, // [1] 1
    0b111111111000111111000, // [2] 2
    0b111111111000000111111, // [3] 3
    0b111111000111000000111, // [4] 4
    0b111000111111000111111, // [5] 5
    0b111000111111111111111, // [6] 6
    0b000111111000000000111, // [7] 7
    0b111111111111111111111, // [8] 8
    0b111111111111000111111, // [9] 9
    0b000000000000000000000, // [10] off
    0b111111111111000000000, // [11] degrees symbol
    0b000000111111111111000, // [12] C(elsius)
    0b111000111111111000000, // [13] F(ahrenheit)
};
void displayDots(CRGB color)
{
    if (dotsOn)
    {
        LEDs[28] = color;
        LEDs[29] = color;
    }
    else
    {
        LEDs[28] = CRGB::Black;
        LEDs[29] = CRGB::Black;
    }

    dotsOn = !dotsOn;
}
void displayNumber(byte number, byte segment, CRGB color)
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
        yield();
        LEDs[i + startindex] = ((numbers[number] & 1 << i) == 1 << i) ? color : alternateColor;
    }
}

void allBlank()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        LEDs[i] = CRGB::Black;
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

void updateCountdown()
{

    if (countdownMilliSeconds == 0 && endCountDownMillis == 0)
        return;

    unsigned long restMillis = endCountDownMillis - millis();
    unsigned long hours = ((restMillis / 1000) / 60) / 60;
    unsigned long minutes = (restMillis / 1000) / 60;
    unsigned long seconds = restMillis / 1000;
    int remSeconds = seconds - (minutes * 60);
    int remMinutes = minutes - (hours * 60);

    Serial.print(restMillis);
    Serial.print(" ");
    Serial.print(hours);
    Serial.print(" ");
    Serial.print(minutes);
    Serial.print(" ");
    Serial.print(seconds);
    Serial.print(" | ");
    Serial.print(remMinutes);
    Serial.print(" ");
    Serial.println(remSeconds);

    byte h1 = hours / 10;
    byte h2 = hours % 10;
    byte m1 = remMinutes / 10;
    byte m2 = remMinutes % 10;
    byte s1 = remSeconds / 10;
    byte s2 = remSeconds % 10;

    CRGB color = countdownColor;
    if (restMillis <= 60000)
    {
        color = CRGB::Red;
    }

    if (hours > 0)
    {
        // hh:mm
        displayNumber(h1, 3, color);
        displayNumber(h2, 2, color);
        displayNumber(m1, 1, color);
        displayNumber(m2, 0, color);
    }
    else
    {
        // mm:ss
        displayNumber(m1, 3, color);
        displayNumber(m2, 2, color);
        displayNumber(s1, 1, color);
        displayNumber(s2, 0, color);
    }

    displayDots(color);

    if (hours <= 0 && remMinutes <= 0 && remSeconds <= 0)
    {
        Serial.println("Countdown timer ended.");
        //endCountdown();
        countdownMilliSeconds = 0;
        endCountDownMillis = 0;
        digitalWrite(COUNTDOWN_OUTPUT, HIGH);
        return;
    }
}

void endCountdown()
{
    allBlank();
    for (int i = 0; i < NUM_LEDS; i++)
    {
        if (i > 0)
            LEDs[i - 1] = CRGB::Black;

        LEDs[i] = CRGB::Red;
        FastLED.show();
        delay(25);
    }
}

void hideDots()
{
    LEDs[28] = CRGB::Black;
    LEDs[29] = CRGB::Black;
}

void updateTemperature()
{
    float ftemp = 99.99;
    float ctemp = ftemp + temperatureCorrection;
    Serial.print("Sensor temp: ");
    Serial.print(ftemp);
    Serial.print(" Corrected: ");
    Serial.println(ctemp);

    if (temperatureSymbol == 13)
        ctemp = (ctemp * 1.8000) + 32;

    byte t1 = int(ctemp) / 10;
    byte t2 = int(ctemp) % 10;
    CRGB color = CRGB(r_val, g_val, b_val);
    displayNumber(t1, 3, color);
    displayNumber(t2, 2, color);
    displayNumber(11, 1, color);
    displayNumber(temperatureSymbol, 0, color);
    hideDots();
}

void updateScoreboard()
{
    byte sl1 = scoreboardLeft / 10;
    byte sl2 = scoreboardLeft % 10;
    byte sr1 = scoreboardRight / 10;
    byte sr2 = scoreboardRight % 10;

    displayNumber(sl1, 3, scoreboardColorLeft);
    displayNumber(sl2, 2, scoreboardColorLeft);
    displayNumber(sr1, 1, scoreboardColorRight);
    displayNumber(sr2, 0, scoreboardColorRight);
    hideDots();
}

void printDateTime(const DateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.month(),
               dt.day(),
               dt.year(),
               dt.hour(),
               dt.minute(),
               dt.second());
    Serial.println(datestring);
}

void setup()
{
    pinMode(COUNTDOWN_OUTPUT, OUTPUT);
    Serial.begin(115200);
    delay(200);
    Serial.println("Uno Start");
    // RTC DS3231 Setup

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }

    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running, let's set the time!");

        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    //Serial.setDebugOutput(true);

    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(LEDs, NUM_LEDS);
    FastLED.setDither(false);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
    fill_solid(LEDs, NUM_LEDS, CRGB::Black);
    FastLED.show();

    // WiFi - AP Mode or both
#if defined(WIFIMODE) && (WIFIMODE == 0 || WIFIMODE == 2)
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(APssid, APpassword); // IP is usually 192.168.4.1
    Serial.println();
    Serial.print("SoftAP IP: ");
    Serial.println(WiFi.softAPIP());
#endif

    // WiFi - Local network Mode or both
#if defined(WIFIMODE) && (WIFIMODE == 1 || WIFIMODE == 2)
    byte count = 0;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        // Stop if cannot connect
        if (count >= 60)
        {
            Serial.println("Could not connect to local WiFi.");
            return;
        }

        delay(500);
        Serial.print(".");
        LEDs[count] = CRGB::Green;
        FastLED.show();
        count++;
    }
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());

    IPAddress ip = WiFi.localIP();
    Serial.println(ip[3]);
#endif

    digitalWrite(COUNTDOWN_OUTPUT, LOW);
    updateClock();
}

void loop()
{
    updateClock();
    FastLED.setBrightness(brightness);
    FastLED.show();
    /*
  unsigned long currentMillis = millis();
  if (currentMillis - prevTime >= 1000)
  {
    prevTime = currentMillis;

    if (clockMode == 0)
    {
      updateClock();
    }
    else if (clockMode == 1)
    {
      updateCountdown();
    }
    else if (clockMode == 2)
    {
      updateTemperature();
    }
    else if (clockMode == 3)
    {
      updateScoreboard();
    }

    FastLED.setBrightness(brightness);
    FastLED.show();
  }
  */
}