#include "src/indicate.h"
#include <ACAN_ESP32.h>
#include <Adafruit_NeoPixel.h>

uint8_t canEnPin = 14;

#define CAN_ID_LED_MODE      (0x10)

uint8_t ledMode = 0xFF;
void setup() {
  // 모니터 프로그램용 시리얼
  Serial.begin(115200);
  
  indicate.begin();

  pinMode(canEnPin, OUTPUT);
  digitalWrite(canEnPin, LOW);

  ACAN_ESP32_Settings settings(250000UL); //125Kbps

  settings.mRxPin = GPIO_NUM_18;
  settings.mTxPin = GPIO_NUM_19;
  // CAN 시작
  const uint32_t ret = ACAN_ESP32::can.begin(settings) ;
  
  if (ret == 0) {    
#if 0    
    // 성공했으면 관련 정보 출력
    Serial.print("Bit Rate prescaler: ") ;
    Serial.println(settings.mBitRatePrescaler) ;
    Serial.print("Time Segment 1:     ") ;
    Serial.println(settings.mTimeSegment1) ;
    Serial.print("Time Segment 2:     ") ;
    Serial.println(settings.mTimeSegment2) ;
    Serial.print("RJW:                ") ;
    Serial.println(settings.mRJW) ;
    Serial.print("Triple Sampling:    ") ;
    Serial.println(settings.mTripleSampling ? "yes" : "no") ;
    Serial.print("Actual bit rate:    ") ;
    Serial.print(settings.actualBitRate ()) ;
    Serial.println(" bit/s") ;
    Serial.print("Exact bit rate ?    ") ;
    Serial.println(settings.exactBitRate () ? "yes" : "no") ;
    Serial.print("Distance            ") ;
    Serial.print(settings.ppmFromDesiredBitRate ()) ;
    Serial.println(" ppm") ;
    Serial.print("Sample point:       ") ;
    Serial.print(settings.samplePointFromBitStart ()) ;
    Serial.println("%") ;
#endif    
    Serial.println("CAN Configuration OK!");
  } else {
    // 실패시 에러 코드 출력
    Serial.print("Configuration error 0x") ;
    Serial.println(ret, HEX) ;    
  }
  ledMode = 0;
  indicate.setLedMode(ledMode);
}

void loop() {
  // CAN 메시지 프레임 변수
  CANMessage frame;  
  // 받은 데이터가 있다면
  if(ACAN_ESP32::can.receive(frame)) {
    switch(frame.id){
      case CAN_ID_LED_MODE:      
        if(frame.len > 0 && ledMode != frame.data[0]) {
          ledMode = frame.data[0];
          Serial.print("Received led mode: ");
          Serial.println(ledMode);  
          
          indicate.setLedMode(ledMode);        
        }
        break;
      default:      
#if 0      
        Serial.print("**** Received ");
        // ID가 일반(Std)인지 확장(ext)인지 표시
        Serial.print(frame.ext ? "extended " : "standard ");
        // 데이터가 요청(remote)인지 일반 데이터인지 표시
        Serial.print(frame.rtr ? "remote " : "data ");
        // ID 표시
        Serial.print("frame, id 0x ");
        Serial.print(frame.id, HEX);
        // 데이터 길이 표시
        Serial.print("len 0x ");
        Serial.println(frame.len);
        
        // 데이터가 있다면 출력
        if(frame.len > 0) {
          Serial.println("  data: ");
          // 데이터 길이 만큼
          for(uint8_t i=0;i<frame.len;i++) {
            Serial.print(frame.data[i], HEX);
            Serial.print(" ");
          }
          Serial.println("");
        }
#endif        
      break;    
    }    
  }
}
