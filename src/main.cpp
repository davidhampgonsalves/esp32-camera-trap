#include <Arduino.h>

#define BAUD_RATE 115200
#define GPIO_PIN_WAKEUP GPIO_NUM_12
#define EXT_WAKEUP_PIN_BITMASK 0x1000  //  2^12
#define MINIMUM_WAKE_PERIOD_MILLIS 60e3

unsigned long lastWakeupPinHigh = 0;

void gotoSleep(){
  Serial.println("Deep sleep enabled");
  esp_sleep_enable_ext0_wakeup(GPIO_PIN_WAKEUP, HIGH);
  esp_deep_sleep_start();
}

unsigned long secs() {
  return millis() / 1e3L;
}

void loop() {
  unsigned long now=millis();
  int wakeupPinState=digitalRead(GPIO_PIN_WAKEUP);
  if(wakeupPinState==HIGH){
    lastWakeupPinHigh=now;
  } else {
    if(now-lastWakeupPinHigh >= MINIMUM_WAKE_PERIOD_MILLIS){
      gotoSleep();
    }
  }
  if(now % 1000 ==0){
    Serial.printf("%u wakeupPinState %u \n", secs(), wakeupPinState);
  }
}

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.setDebugOutput(false);
  Serial.println();
  Serial.print(F("SensorTest compiled "));
  Serial.print(__DATE__ " " __TIME__);
  Serial.println();

  pinMode(GPIO_PIN_WAKEUP, INPUT);
  lastWakeupPinHigh = millis();
}
