#include "main.h"

void IRAM_ATTR down_to_datum() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 600) {
    if (status == 2){
      status = 3;
      motor.motorBreak();
      position_.current_positon = 0;
    } else if (status == 3) {

    } else {
      status = 0;
    }
  }
  last_interrupt_time = interrupt_time;
}

void IRAM_ATTR bottom_switch() {
  static unsigned long last_interrupt_time1 = 0;
  unsigned long interrupt_time1 = millis();
  if (interrupt_time1 - last_interrupt_time1 > 600) {
    if (status != 2 && status != 3){
      status ++;
    }
  }
  last_interrupt_time1 = interrupt_time1;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  sensor.setTimeout(500);
  while (!sensor.init()){
    Serial.println("Failed to detect and initialize sensor!");
  }
  sensor.setDistanceMode(VL53L1X::Medium);
  sensor.setMeasurementTimingBudget(50000);
  sensor.setROISize(4, 4);
  sensor.startContinuous(50);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("UBrobotics", "ubrobotics1");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
  ArduinoOTA.begin();

  //top switch
  pinMode(26, INPUT_PULLUP);
  attachInterrupt(26, down_to_datum, FALLING);

  //bottom switch
  pinMode(27, INPUT_PULLUP);
  attachInterrupt(27, bottom_switch, RISING);
}

void loop() {
  Serial.print(sensor.read());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.println();

  switch (status){
    case 0: { // idle can be moved
      Serial.println(Drv8323s.SetDriverControl());
      Drv8323s.get_status();
      ArduinoOTA.handle();
    }
    break;

    case 1: { // ready to start
      motor.motorBreak(); // TODO: any torque when motor break?
      position_.datum_line = sensor.read(); // TODO: blocking? Average?
    }
    break;

    case 2: { // up to top
      position_.current_positon = sensor.read(); // TODO: if the reading is not abailable or valid, keep the old one. Switch!
      long output = control.Compute(position_.current_positon, position_.datum_line);
      motor.motorGo(output * direction);
    }
    break;

    case 3: { // down to DATUM line
      position_.current_positon = sensor.read(); // TODO: if the reading is not abailable or valid, keep the old one.
      long output = control.Compute(position_.current_positon, position_.datum_line);
      if (output == 0) { // TODO: <10?
        status++;
      }
      motor.motorGo(-output * direction);
    }
    break;

    case 4: { // go to target
    position_.current_positon = sensor.read(); // TODO: if the reading is not abailable or valid, keep the old one.
      long output = (long) control.Compute(position_.current_positon, position_.red_line);  
      motor.motorGo(output * direction);
    }
    break;

    default:
      status = 0;
    break;
  }
}
