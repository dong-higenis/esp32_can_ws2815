/*

    https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects
*/

#include "./pixels.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

Pixels pixels;

void pixelsTaskUpdate( void *pvParameters );

Pixels::Pixels()
    :strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800) {
}

Pixels::~Pixels() {
}

void pixelsTaskUpdate(void *pvParameters) {
  Pixels *p_class =  (Pixels *)pvParameters;
  
  for (;;) {
    p_class->loop();
    vTaskDelay(1);
  }
}

bool Pixels::begin() {
    xTaskCreatePinnedToCore(
        pixelsTaskUpdate,
        "pixelsTaskUpdate",
        4*1024,       // Stack size
        (void *)this,
        1,            // Priority
        NULL,
        1);
        
    return true;
}

bool Pixels::loop(void) {
  return true;
}


uint32_t Pixels::getPixel(int index) {
    return strip.getPixelColor(index);
}
void Pixels::setPixel(int index, uint8_t r, uint8_t g, uint8_t b) {
    strip.setPixelColor(index, strip.Color(r,g,b));
}

void Pixels::showStrip() {
    strip.show();
}

void Pixels::setAll(int totalLeds, uint8_t r, uint8_t g, uint8_t b) {
    for(int i=0; i<totalLeds; i++) {
        strip.setPixelColor(i, strip.Color(r,g,b));
    }
}

void Pixels::centerToOutside(uint8_t r, uint8_t g, uint8_t b, int totalLeds, int unitSize, int speedDelay, int returnDelay) {
  for(int i =((totalLeds-unitSize)/2); i>=0; i--) {
    setAll(totalLeds, 0,0,0);
   
    setPixel(i, r/10, g/10, b/10);
    for(int j = 1; j <= unitSize; j++) {
      setPixel(i+j, r, g, b);
    }
    setPixel(i+unitSize+1, r/10, g/10, b/10);
   
    setPixel(totalLeds-i, r/10, g/10, b/10);
    for(int j = 1; j <= unitSize; j++) {
      setPixel(totalLeds-i-j, r, g, b);
    }
    setPixel(totalLeds-i-unitSize-1, r/10, g/10, b/10);
   
    showStrip();
    delay(speedDelay);
  }
  delay(returnDelay);
}

void Pixels::fadeIn(uint8_t r, uint8_t g, uint8_t b, int totalLeds) {
    float tr, tg, tb;
        
    for(int k = 0; k < 256; k=k+1) {
        tr = (k/256.0)*r;
        tg = (k/256.0)*g;
        tb = (k/256.0)*b;
        setAll(totalLeds, tr, tg, tb);
        showStrip();
    }
}

void Pixels::fadeOut(uint8_t r, uint8_t g, uint8_t b, int totalLeds) {
    float tr, tg, tb;   
        
    for(int k = 255; k >= 0; k=k-2) {
        tr = (k/256.0)*r;
        tg = (k/256.0)*g;
        tb = (k/256.0)*b;
        setAll(totalLeds, tr, tg, tb);
        showStrip();
    }
}  

void Pixels::fadeInOut(uint8_t r, uint8_t g, uint8_t b, int totalLeds) {
    fadeIn(r, g, b, totalLeds);
    fadeOut(r, g, b, totalLeds);
}  


void Pixels::runningLights(uint8_t r, uint8_t g, uint8_t b, int totalLeds, int waveDelay) {
  int pos=0;
 
  for(int j=0; j<totalLeds*2; j++) {
      pos++; // = 0; //pos + Rate;
      for(int i=0; i<totalLeds; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+pos) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+pos) * 127 + 128;
        setPixel(i,((sin(i+pos) * 127 + 128)/255)*r,
                   ((sin(i+pos) * 127 + 128)/255)*g,
                   ((sin(i+pos) * 127 + 128)/255)*b);
      }     
      showStrip();
      delay(waveDelay);
  }
}

void Pixels::fire(int totalLeds, int cooling, int sparking, int speedDelay) {
  static uint8_t heat[MAX_LEDS];
  int cooldown;
 
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < totalLeds; i++) {
    cooldown = random(0, ((cooling * 10) / totalLeds) + 2);
   
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
 
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= totalLeds - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
   
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < totalLeds; j++) {
    setPixelHeatColor(j, heat[j]);
  }

  showStrip();
  delay(speedDelay);
}

void Pixels::setPixelHeatColor (int pixel, uint8_t temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  uint8_t t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  uint8_t heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(pixel, heatramp, 0, 0);
  }
}

void Pixels::meteorRain(uint8_t r, uint8_t g, uint8_t b, int totalLeds, uint8_t meteorSize, uint8_t meteorTrailDecay, boolean meteorRandomDecay, int speedDelay) {  
  setAll(totalLeds, 0,0,0);
 
  for(int i = 0; i < totalLeds+totalLeds; i++) {    
    // fade brightness all LEDs one step
    for(int j=0; j<totalLeds; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <totalLeds) && (i-j>=0) ) {
        setPixel(i-j, r, g, b);
      }
    }   
    showStrip();
    delay(speedDelay);
  }
}

void Pixels::fadeToBlack(int ledNo, uint8_t fadeValue) {
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = getPixel(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    setPixel(ledNo, r,g,b);    
}
