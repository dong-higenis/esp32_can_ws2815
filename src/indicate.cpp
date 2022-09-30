/*

    https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects
*/

#include "./indicate.h"
#include "./pixels.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

Indicate indicate;


void indicateTaskUpdate( void *pvParameters );

Indicate::Indicate() {
}

Indicate::~Indicate() {
}

void indicateTaskUpdate(void *pvParameters) {
  Indicate *p_class =  (Indicate *)pvParameters;
  
  for (;;) {
    p_class->loop();
    vTaskDelay(1);
  }
}

bool Indicate::begin(void) {
    xTaskCreatePinnedToCore(
        indicateTaskUpdate,
        "indicateTaskUpdate",
        4*1024,       // Stack size
        (void *)this,
        1,            // Priority
        NULL,
        1);

    pixels.begin();

    return true;
}

void Indicate::ledMode1() {    
  static uint32_t tm = 0;   
  if(millis() - tm > 500) {
    tm = millis();    
    switch(indicateStep) {
        case 0:
            pixels.fadeInOut(100, 100, 100, 20);
            indicateStep++;
            break;
        default:
            break;
    } 
  }
}
void Indicate::ledMode2() {    
  static uint32_t tm = 0;  
  if(millis() - tm > 500) {
    tm = millis();    
    switch(indicateStep) {
        case 0:
            pixels.fadeIn(100, 100, 100, 20);
            indicateStep++;
            break;
        default:
            break;
    } 
  }
}
void Indicate::ledMode3() {    
  static uint32_t tm = 0;  
  if(millis() - tm > 500) {
    tm = millis();    
    switch(indicateStep) {
        case 0:
            pixels.fadeOut(100, 100, 100, 20);
            indicateStep++;
            break;
        default:
            break;
    } 
  }
}

void Indicate::ledMode4() {    
  static uint32_t tm = 0;  
  if(millis() - tm > 1000) {
    tm = millis();
    if(indicateStep) {
        pixels.fadeOut(255, 0, 0, 20);
        indicateStep = 0;
    } else {
        pixels.fadeIn(255, 0, 0, 20);
        indicateStep = 1;
    }    
  }
}

bool Indicate::loop(void) {
  switch(ledMode){
    case 0:
      ledMode1();
      break;
    case 1:
      ledMode2();
      break;
    case 2:
      ledMode3();
      break;
    case 3:
      ledMode4();
      break;
  }
  return true;
}

void Indicate::setLedMode(uint8_t mode) {  
  ledMode = mode;
  indicateStep = 0;
}
