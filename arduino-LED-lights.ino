#include <Adafruit_NeoPixel.h>    //https://github.com/adafruit/Adafruit_NeoPixel
#include <RTClib.h>               //https://github.com/adafruit/RTClib

#define DIN 3     //Neopixel DIN pin to Arduino Pin
#define PIXELS 48 //Number of LED pixels

RTC_DS3231 timer;
Adafruit_NeoPixel led_strip = Adafruit_NeoPixel(PIXELS, DIN, NEO_GRB + NEO_KHZ800);

// RGB Color
struct RGB
{   unsigned char r, g, b;
};

//Details of a specific RGB color at a second in the day
struct ColorFrame
{   RGB color;
    int time_point;
};

//Returns the number of seconds in the day
int getSecondsOfDay(const DateTime &date)
{   return date.hour() * 3600 + date.minute() * 60 + date.second();
}

//Transitions from one value to another at some progress %
int tween(int from, int to, float progress)
{   return (progress * (float)to) + ((1 - progress) * (float)from);
}

//Calculates the progress rate of curr between start and end
float ratio(float start, float curr, float end)
{   return (curr - start) / end;
}

//Derives a RGB LED_Color by tweening one color frame to the other
void getColor(const int time, const ColorFrame &c1, const ColorFrame &c2, RGB &res)
{   
    float rate = ratio(c1.time_point, time, c2.time_point);
    res = RGB{
        (unsigned char)tween(c1.color.r, c2.color.r, rate),
        (unsigned char)tween(c1.color.g, c2.color.g, rate),
        (unsigned char)tween(c1.color.b, c2.color.b, rate),
    };
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
    led_strip.setPixelColor(0, Adafruit_NeoPixel::Color(0,0,0));
    led_strip.show();
}

const int color_count = 3;
ColorFrame colors[color_count] = {
    { 255, 0, 0, 0  },
    { 0, 255, 0, 20 },
    { 0, 0, 255, 40 },
};

int counter = 0;
RGB col;

void loop() 
{
    char date[10] = "hh:mm:ss";
    timer.now().toString(date);
    Serial.println(date);

    getColor(counter, colors[0], colors[1], col);
    led_strip.setPixelColor(0, Adafruit_NeoPixel::Color(col.r, col.g, col.b));
    led_strip.show();
    
    delay(1000);
    counter+=1;
}
