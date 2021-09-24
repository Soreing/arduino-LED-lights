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

// Compares two RGB colors to test if they are the same
bool operator==(const RGB &l, const RGB &r)
{   return l.r == r.r && l.g == r.g && l.b == r.b;
}

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
{   return (curr - start) / (end - start);
}

//Derives a RGB LED_Color by tweening one color frame to the other
void getColor(const int time, const ColorFrame &c1, const ColorFrame &c2, RGB &res)
{   
    float rate;
    
    if( c2.time_point > c1.time_point )
    {   rate = ratio(c1.time_point, time, c2.time_point );
    }
    else if(time < c1.time_point)
    {   rate = ratio(c1.time_point, time+30, c2.time_point+30);
    }
    else
    {   rate = ratio(c1.time_point, time, c2.time_point+30);
    }

    res = RGB{
        (unsigned char)tween(c1.color.r, c2.color.r, rate),
        (unsigned char)tween(c1.color.g, c2.color.g, rate),
        (unsigned char)tween(c1.color.b, c2.color.b, rate),
    };
}

//Checks if the time is between ColorFrame A and B's time 
bool isValidFrame(const int time, const int a, const int b)
{   return !((a<=b && time>b) || (a>b && time>b && time<a));
}

int counter = 0;
int now_idx = 0;
int nxt_idx = 0;
RGB col;

const int color_count = 3;
ColorFrame colors[color_count] = {
    { 255, 0, 0, 0  },
    { 0, 255, 0, 10 },
    { 0, 0, 255, 20 },
};

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

    //Setup next index
    nxt_idx = (now_idx + 1) % color_count;
}

void loop() 
{
    //char date[10] = "hh:mm:ss";
    //timer.now().toString(date);
    //Serial.println(date);

    int time = counter;//getSecondsOfDay(timer.now());
    while(!isValidFrame(time, colors[now_idx].time_point, colors[nxt_idx].time_point))
    {   now_idx = nxt_idx;
        nxt_idx = (nxt_idx + 1) % color_count;
    }

    getColor(counter, colors[now_idx], colors[nxt_idx], col);
    led_strip.setPixelColor(0, Adafruit_NeoPixel::Color(col.r, col.g, col.b));
    led_strip.show();
    
    Serial.print(now_idx, DEC);
    Serial.print("-");
    Serial.print(nxt_idx, DEC);
    Serial.print(" ");
    Serial.println(time, DEC);

    delay(1000);

    if(++counter == 30)
        counter = 0;
}
