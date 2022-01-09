#include "led.h"
#include "temp.h"
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define S_RX 4 // Define software serial RX pin
#define S_TX 3

SoftwareSerial SoftSerial(S_RX, S_TX);
TinyGPSPlus gps;

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
    setupTemp();
    Serial.print("Temperature was setup  ");
    sensors.requestTemperatures(); // Send the command to get temperature readings
    temp = sensors.getTempCByIndex(0);
    Serial.println(temp);
    DateTime now = rtc.now();
    Serial.print("hour   ");
    Serial.print(now.hour());
    Serial.print(" : ");
    Serial.println(now.minute());

    displayNumber(5, 0); // Blank
    displayNumber(2, 1);
    displayNumber(3, 2);
    displayNumber(4, 3);
}

void loop()
{
    if (!timeGPSclock)
    {
        GPSTimeUpdate();
    }
    else
    {
        DateTime now = rtc.now();

        if (!(now.second() % 5) && !clockFlag && prev_sec != now.second())
        {
            Serial.print("current time  ");
            Serial.print(now.hour());
            Serial.print(":");
            Serial.print(now.minute());
            Serial.print(":");
            Serial.println(now.second());
            prev_sec = now.second();
            clockFlag = true;
            prev_min = now.minute();
            byte h1 = now.hour() / 10;
            byte h2 = now.hour() % 10;
            byte m1 = now.minute() / 10;
            byte m2 = now.minute() % 10;
            byte s1 = now.second() / 10;
            byte s2 = now.second() % 10;
            if (h1 > 0)
                displayNumber(h1, 0);
            else
                displayNumber(10, 0); // Blank
            displayNumber(h2, 1);
            displayNumber(m1, 2);
            displayNumber(m2, 3);
            displayDots(1);
            FastLED.show();
        }
        else if (!(now.second() % 5) && clockFlag && prev_sec != now.second())
        {
            allBlank();
            Serial.print("temp  ");
            Serial.println(temp);
            prev_sec = now.second();
            clockFlag = false;
            byte h1 = abs(temp / 10);
            byte h2 = abs(temp % 10);

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
            temp = sensors.getTempCByIndex(0);
        }

        if (now.hour() == 2 && now.minute() == 0 && now.second() == 0)
        {
            Serial.println("gonna off");
            timeGPSclock = 0;
        }
    }
}