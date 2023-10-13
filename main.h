#include "DRV8323s.h"
#include <Wire.h>
#include "VL53L1X.h"
#include "pwmmotordrive.h"
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include "position_control.h"

#define direction (long)1

VL53L1X sensor;
DRV8323S Drv8323s(SCK, MISO, MOSI, SS);
pwmmotordrive motor(34, 35, 32);
position_control control(0.7, 0.01, 0.5);

/*
MOSI - 23
MISO - 19
SCK - 18
SS - 5

SDA	- 21
SCL	- 22
*/

uint8_t status = 0;

struct position {
  double current_positon;
  double datum_line;
  double datum = 2250.0, red_line = 1050.0;
} position_;
