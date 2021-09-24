#include <Adafruit_NeoPixel.h>    //https://github.com/adafruit/Adafruit_NeoPixel
#include <RTClib.h>               //https://github.com/adafruit/RTClib

#define DIN 3     //Neopixel DIN pin to Arduino Pin
#define PIXELS 48 //Number of LED pixels

#define LED_Color Adafruit_NeoPixel::Color

RTC_DS3231 timer;
Adafruit_NeoPixel led_strip = Adafruit_NeoPixel(PIXELS, DIN, NEO_GRB + NEO_KHZ800);

//Transitions from one value to another at some progress %
int tween(int from, int to, float progress)
{   return (progress * (float)to) + ((1 - progress) * (float)from);
}

//Calculates the progress rate of curr between start and end
float ratio(float start, float curr, float end)
{   return (curr - start) / end;
}

void setup() 
{
    //Initializing objects
    Serial.begin(9600);
    led_strip.begin();
    timer.begin();

    //Setting up timer
    timer.adjust(DateTime(F(__DATE__), F(__TIME__)));

    //Lighting up one pixel
    led_strip.setPixelColor(0, LED_Color(0,0,0));
    led_strip.show();
}

int counter = 0;

void loop() 
{
    char date[10] = "hh:mm:ss";

    int r = tween(0,255, ratio(0, counter, 60));
    led_strip.setPixelColor(0, LED_Color(r,0,0));
    led_strip.show();
    
    timer.now().toString(date);
    Serial.println(r);
    Serial.println(date);
    
    delay(1000);
    counter+=1;
}
