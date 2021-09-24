#include <Adafruit_NeoPixel.h>    //https://github.com/adafruit/Adafruit_NeoPixel
#include <RTClib.h>               //https://github.com/adafruit/RTClib

#define DIN 3     //Neopixel DIN pin to Arduino Pin
#define PIXELS 48 //Number of LED pixels

#define LED_Color Adafruit_NeoPixel::Color

RTC_DS3231 timer;
Adafruit_NeoPixel led_strip = Adafruit_NeoPixel(PIXELS, DIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
    //Initializing objects
    Serial.begin(9600);
    led_strip.begin();
    timer.begin();

    //Setting up timer
    timer.adjust(DateTime(F(__DATE__), F(__TIME__)));

    //Lighting up one pixel
    led_strip.setPixelColor(0, LED_Color(255,255,0));
    led_strip.show();
}

void loop() 
{
    char date[10] = "hh:mm:ss";
    
    timer.now().toString(date);
    Serial.println(date);
    
    delay(1000);
}
