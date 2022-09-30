#ifndef __PIXELS_H__
#define __PIXELS_H__

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define MAX_LEDS          500

#define LED_COUNT     20
#define LED_PIN       4


class Pixels
{
  public:
    Pixels();
    ~Pixels();

    bool begin();
    bool loop(void);    
    void meteorRain(uint8_t r, uint8_t g, uint8_t b, int totalLeds, uint8_t meteorSize, uint8_t meteorTrailDecay, boolean meteorRandomDecay, int speedDelay);
    void fadeToBlack(int ledNo, uint8_t fadeValue);
    void fire(int totalLeds, int cooling, int sparking, int speedDelay);
    void setPixelHeatColor(int pixel, uint8_t temperature);
    void fadeIn(uint8_t r, uint8_t g, uint8_t b, int totalLeds);
    void fadeOut(uint8_t r, uint8_t g, uint8_t b, int totalLeds);
    void fadeInOut(uint8_t r, uint8_t g, uint8_t b, int totalLeds);
    void runningLights(uint8_t r, uint8_t g, uint8_t b, int totalLeds, int waveDelay);
    void centerToOutside(uint8_t r, uint8_t g, uint8_t b, int totalLeds, int unitSize, int speedDelay, int returnDelay);    
    
  private:
    uint32_t getPixel(int index);
    void setPixel(int index, uint8_t r, uint8_t g, uint8_t b);    
    void setAll(int totalLeds, uint8_t r, uint8_t g, uint8_t b);
    void showStrip();

    Adafruit_NeoPixel strip;
};
  
extern Pixels pixels;


#endif /* __PIXELS_H__ */