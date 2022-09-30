#ifndef INDICATE_H_
#define INDICATE_H_

#include <Arduino.h>

class Indicate
{
  public:
    Indicate();
    ~Indicate();

    bool begin(void);
    bool loop(void);

    void setLedMode(uint8_t mode);
    void ledMode1();
    void ledMode2();
    void ledMode3();
  private:
    uint8_t ledMode;
    uint8_t indicateStep = 0;

};
  
extern Indicate indicate;

#endif  /* INDICATE_H_ */