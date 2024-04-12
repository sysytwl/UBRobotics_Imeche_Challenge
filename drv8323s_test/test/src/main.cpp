#include <Arduino.h>
#include "DRV8323S.hpp"

DRV8323S Drv8323s(SCK, MISO, MOSI, SS);

void setup() {
  log_v("PINS: \n SCK: %d \n MISO: %d \n MOSI: %d \n SS: %d \n", SCK, MISO, MOSI, SS);
  Serial.println(Drv8323s.SetDriverControl());
  Drv8323s.get_status();
}

void loop() {
  // put your main code here, to run repeatedly:
}
