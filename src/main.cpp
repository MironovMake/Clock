#include "led.h"
#include "temp.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define S_RX 4 // Define software serial RX pin
#define S_TX 3
byte h1;
byte h2;
byte m1;
byte m2;
byte s1;
byte s2;
SoftwareSerial SoftSerial(S_RX, S_TX);
TinyGPSPlus gps;
int displayflag = 0;
int start_positon;
bool clockFlag = 0;
bool timeGPSclock = 0;
bool sing;
struct MyTime
{
    int hour;
    int minute;
    int day;
    int month;
    int year;
    int second;
};
struct MyTime GPSclock;
struct MyTime clock;
#include "RTC.h"
#include "GPS.h"

void setup()
{
    Serial.begin(57600);
    SoftSerial.begin(9600);
    Serial.print("Temperature was setup  ");

    setupLed();
    setupRTC();
    delay(1000);
    setupTemp();

    sensors.requestTemperatures(); // Send the command to get temperature readings
    temp = sensors.getTempCByIndex(0);
    Serial.println(temp);
    DateTime now = rtc.now();
    Serial.print("hour   ");
    Serial.print(now.hour());
    Serial.print(" : ");
    Serial.println(now.minute());
}

void loop()
{
    // first time synchronizatin with GPS
    if (!timeGPSclock)
    {
        GPSTimeUpdate();
    }
    // When synchronizatin with GPS done, show time/temperature/date
    else
    {
        // get time from RTC
        DateTime now = rtc.now();
        // display time
        if (displayflag == 0)
        {
            displayflag = 1;
            prev_sec = now.second();
            prev_min = now.minute();
            h1 = now.hour() / 10;
            h2 = now.hour() % 10;
            m1 = now.minute() / 10;
            m2 = now.minute() % 10;
            s1 = now.second() / 10;
            s2 = now.second() % 10;
            if (h1 > 0)
                displayNumber(h1, 0);
            else
                displayNumber(10, 0); // Blank
            displayNumber(h2, 1);
            displayNumber(m1, 2);
            displayNumber(m2, 3);
            FastLED.show();
            for (int i = 0; i < 7; i++)
            {
                displayDots(1);
                FastLED.show();
                delay(500);
                displayDots(0);
                FastLED.show();
                delay(500);
            }
        }
        // display temperature
        else if (displayflag == 1)
        {
            displayflag = 2;
            allBlank();
            prev_sec = now.second();
            h1 = abs(temp / 10);
            h2 = abs(temp % 10);
            if (h1 > 0)
                displayNumber(h1, 1);
            else
                displayNumber(10, 1); // Blank
            if (temp < 0 && h1 <= 0)
            {
                displayNumber(14, 1);
            }
            else if (temp < 0 && h1 > 0)
                displayNumber(14, 0);
            displayNumber(h2, 2);
            displayNumber(12, 3);
            displayDots(0);
            FastLED.show();
            sensors.requestTemperatures(); // Send the command to get temperature readings
            temp = sensors.getTempCByIndex(0) - 6;
            delay(7000);
        }
        // display date
        else if (displayflag == 2)
        {
            allBlank();
            leds[65] = CRGB::Green;
            displayflag = 0;
            prev_sec = now.second();
            h1 = now.month() / 10;
            h2 = now.month() % 10;
            m1 = now.day() / 10;
            m2 = now.day() % 10;
            if (m1 == 0)
            {
                displayNumber(10, 0);
                displayNumber(m2, 1);
            }
            else
            {
                displayNumber(m1, 0);
                displayNumber(m2, 1);
            }
            if (h1 == 0)
            {
                displayNumber(10, 3);
                displayNumber(h2, 2);
            }
            else
            {
                displayNumber(h1, 2);
                displayNumber(h2, 3);
            }

            FastLED.show();
            delay(7000);
            leds[65] = CRGB::Black;
        }

        // synchronization a 2 a.m.
        if (now.hour() == 2 && now.minute() == 0 && now.second() < 30)
        {
            Serial.println("gonna off");
            timeGPSclock = 0;
        }
    }
}