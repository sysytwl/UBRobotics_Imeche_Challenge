#include <Arduino.h>
#include "DRV8323S.hpp"

DRV8323S Drv8323s(SCK, MISO, MOSI, SS);

void setup() {
  Serial.begin(115200);
  log_v("PINS: \n SCK: %d \n MISO: %d \n MOSI: %d \n SS: %d \n", SCK, MISO, MOSI, SS);
  log_i("SetDriverControl: %u", Drv8323s.SetDriverControl());
  Drv8323s.get_status();
}

void loop() {

}
