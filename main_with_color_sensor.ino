#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>



static int status = 0;
static bool ex_color_status = 0;
static bool color_status = 0;
static int is_red = 0;


class color_sensor {
  public:
    color_sensor(uint8_t s2, uint8_t s3, uint8_t out) {
      _s2 = s2;
      _s3 = s3;
      _out = out;

      pinMode(_s2, OUTPUT);  
      pinMode(_s3, OUTPUT);  
      pinMode(_out, INPUT); 
    }

    bool color() {    
      //red
      digitalWrite(_s2, LOW);  
      digitalWrite(_s3, LOW);   
      //long red = map(1000.0 / pulseIn(_out, HIGH), 95, 4, 255, 0);
      long red = 5100.0 / (pulseIn(_out, HIGH));

      //green
      digitalWrite(_s2, HIGH);
      digitalWrite(_s3, HIGH);   
      //long green = map(1000.0 / pulseIn(_out, HIGH), 88, 3, 255, 0);
      long green = 5604.4 / (pulseIn(_out, HIGH));

      //blue
      digitalWrite(_s2, LOW);
      digitalWrite(_s3, HIGH); 
      //long blue = map(1000.0 / pulseIn(_out, HIGH), 116, 4, 255, 0);
      long blue = 4594.6 / (pulseIn(_out, HIGH));

      //white
      //digitalWrite(s2, HIGH);
      //digitalWrite(s3, LOW);  
      //white = pulseIn(out_,LOW);

      Serial.print("RGB:"); Serial.print(red); Serial.print(","); Serial.print(green); Serial.print(","); Serial.println(blue);

      float maxVal = max(red, max(green, blue));
      float minVal = min(red, min(green, blue));
      float delta = maxVal - minVal;

      // Calculate hue
      if (delta == 0) {
          _h = 0;
      } else if (maxVal == (float) red) {
          _h = 60 * ((green - blue) / delta);
      } else if (maxVal == (float) green) {
          _h = 60 * ((blue - red) / delta + 2);
      } else if (maxVal == (float) blue){
          _h = 60 * ((red - green) / delta + 4);
      }
      if (_h < 0) {
          _h += 360;
      }

      // Calculate saturation
      //if (maxVal == 0) {
      //    _s = 0;
      //} else {
      //    _s = delta / maxVal;
      //}

      // Calculate value
      //_v = maxVal;
      //Serial.print("HSV:"); Serial.print(_h); Serial.print(","); Serial.print(_s); Serial.print(","); Serial.println(maxVal);

      if ( _h > 340 && _h < 361) {
        return true;
      } else {
        return false;
      }
    }

  private:
    uint8_t _s2, _s3, _out;
    float _h, _s;
};

color_sensor colorsensor(33, 32, 25);

class position_control {
  public:
    position_control(float kp, float ki, float kd) { // kp, ki, kd
      _kp = kp;
      _ki = ki;
      _kd = kd;
      _maxoutput = 255;
      _minoutput = -255;
    }

    float Compute(float input, float target) {
      _err = target - input;

      _outputp = _kp * _err;
      if (_err < 100 && _err > -100){ //Integral separation
        _outputi += _ki * _err;
      } else {
        _outputi = 0;
      }
      _outputd = _kd * (_err - _exerr);

      if (_err < 2 && _err > -2) { // dead zone
        _err = 0;
        _outputp = 0;
        _outputi = 0;
        _outputd = 0;
      }

      _output = _outputp + _outputi + _outputd;

      if (_output > _maxoutput) {
        _output = _maxoutput;
      } else if (_output < _minoutput) {
        _output = _minoutput;
      }

      //Serial.print(_err); Serial.print("    "); Serial.print(_outputp); Serial.print("    "); Serial.print(_outputi); Serial.print("    "); Serial.println(_outputd);

      _exerr = _err;
    
    return _output;
    }

    int get_target(int target) { // get the position of each line
      switch(target) {
        //case 0: return _offset;
        case 1: return _datum;
        case 2: return _red_line;
        default: return 0;
      }
    }

    //void position_record(int position) {
    //  if (_excolor != color) {
    //    //_exposition
    //  }
    //}

  private:
    float _maxoutput, _minoutput, _outputp, _outputi = 0, _outputd, _output;
    float _err, _exerr;

    float _resolution = 126.0 / 836.0; // mmm
    int _datum = -2.2 * 1000.0 / _resolution, _red_line = -1.1 * 1000.0 / _resolution; // _offset = 36.0 / _resolution;

    float real_resolution;
    int real_datum = , real_resolution = , real_red_line;
    //int _exposition, _maxposition;

    float _kp, _ki, _kd;
};

float kp = 0.72, ki = 0.02, kd = 0.15;             // modify for optimal performance
position_control control1(kp, ki, kd);
position_control control2(kp, ki, kd);

class Motor {
  public:
    Motor( uint8_t pinIN1, uint8_t pinIN2, uint8_t ledCH1, uint8_t ledCH2, uint8_t pinIN3, uint8_t pinIN4) { // motor pin1, motor pin2, pwm channel 1, pwm channel 2, encoder pin1, encoder pin2
      _ledCH1 = ledCH1;         // ESP32 LED Channel for PWM to Pin  
      _ledCH2 = ledCH2;         // 0 - 15 are availible by default
      pinA = pinIN3;
      pinB = pinIN4;

      pinMode(pinA, INPUT_PULLDOWN); //INPUT_PULLUP ?
      pinMode(pinB, INPUT_PULLDOWN);

      pinMode(pinIN1, OUTPUT);
      ledcSetup(_ledCH1, 2500, 8);         // Setup channel at 2500Hz with 8 bit (0-255) resolution
      ledcAttachPin(pinIN1, _ledCH1);
      pinMode(pinIN2, OUTPUT);
      ledcSetup(_ledCH2, 2500, 8);         // Setup channel at 2500Hz with 8 bit (0-255) resolution
      ledcAttachPin(pinIN2, _ledCH2);
      _maxpwm = 255;                       // Sets a flag on the motor so the object knows the max pwm value
    }

    void motorGo(long pwmVal) {
      if(pwmVal > 0){
        ledcWrite(_ledCH2, _maxpwm - pwmVal);
        ledcWrite(_ledCH1, _maxpwm);
      } else if(pwmVal < 0) {
        ledcWrite(_ledCH1, _maxpwm + pwmVal);
        ledcWrite(_ledCH2, _maxpwm);
      } else {
        ledcWrite(_ledCH1, _maxpwm);
        ledcWrite(_ledCH2, _maxpwm);
      }
    } 
    
    void motorBrake() {
      ledcWrite(_ledCH1, _maxpwm);
      ledcWrite(_ledCH2, _maxpwm);
    }

    void motorStop() {
      ledcWrite(_ledCH1, 0);
      ledcWrite(_ledCH2, 0);
    }
    
    uint8_t pinA;
    uint8_t pinB;
    int position = 0;

  private:
    uint8_t _ledCH1;        // ESP32 ledc Channel for PWM   
    uint8_t _ledCH2;
    long _maxpwm;           // Max PWM Value of the Motor

};

Motor motor1(22, 23, 0, 1, 35, 34);
Motor motor2(19, 21, 2, 3, 36, 39);

void IRAM_ATTR encoder_A1() {
  if (digitalRead(motor1.pinA) ^ digitalRead(motor1.pinB)) {
    motor1.position--;
    //motor1.lastPulsePeriod = -(micros() - motor1.lastPulseTime);
  } else {
    motor1.position++;
    //motor1.lastPulsePeriod = (micros() - motor1.lastPulseTime);
  }
  //motor1.lastPulseTime = micros();
}

void IRAM_ATTR encoder_A2() {
  if (digitalRead(motor1.pinB) ^ digitalRead(motor1.pinA)) {
    motor1.position++;
    //motor1.lastPulsePeriod = (micros() - motor1.lastPulseTime);
  } else {
    motor1.position--;
    //motor1.lastPulsePeriod = -(micros() - motor1.lastPulseTime);
  }
  //motor1.lastPulseTime = micros();
}

void IRAM_ATTR encoder_B1() {
  if (digitalRead(motor2.pinA) ^ digitalRead(motor2.pinB)) {
    motor2.position--;
    //motor2.lastPulsePeriod = -(micros() - motor2.lastPulseTime);
  } else {
    motor2.position++;
    //motor2.lastPulsePeriod = (micros() - motor2.lastPulseTime);
  }
  //motor2.lastPulseTime = micros();
}

void IRAM_ATTR encoder_B2() {
  if (digitalRead(motor2.pinB) ^ digitalRead(motor2.pinA)) {
    motor2.position++;
    //motor2.lastPulsePeriod = (micros() - motor2.lastPulseTime);
  } else {
    motor2.position--;
    //motor2.lastPulsePeriod = -(micros() - motor2.lastPulseTime);
  }
  //motor2.lastPulseTime = micros();
}

void IRAM_ATTR down_to_datum() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 500) {
    if (status == 2){
      status = 3;
      motor1.position = 0;
      motor2.position = 0;
    } else {
      status = 0;
    }
  }
  last_interrupt_time = interrupt_time;
}

void IRAM_ATTR bottom_switch() {
  static unsigned long last_interrupt_time1 = 0;
  unsigned long interrupt_time1 = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time1 - last_interrupt_time1 > 500) {
    if (status != 2 && status != 3){
      status ++;
    }
  }
  last_interrupt_time1 = interrupt_time1;
}

void setup() {
  Serial.begin(115200); //initialize serial comunication
  //Serial.print("setup running on core "); Serial.println(xPortGetCoreID());

  //encoder counter
  attachInterrupt(motor1.pinA, encoder_A1, CHANGE);
  attachInterrupt(motor1.pinB, encoder_A2, CHANGE);
  attachInterrupt(motor2.pinA, encoder_B1, CHANGE);
  attachInterrupt(motor2.pinB, encoder_B2, CHANGE);

  //top switch
  pinMode(27, INPUT_PULLUP);
  attachInterrupt(27, down_to_datum, FALLING);

  //bottom switch
  pinMode(26, INPUT_PULLUP);
  attachInterrupt(26, bottom_switch, RISING);

  //on board LED
  pinMode(2,OUTPUT);
}

void loop(){
  //Serial.print("PID running on core "); Serial.println(xPortGetCoreID());
  switch (status){
    case 0: { // idle can be moved
      motor1.motorStop();
      motor2.motorStop();
      motor1.position = 0;
      motor2.position = 0;

      //debug only
      colorsensor.color();
      //Serial.print("status: "); Serial.print(status); Serial.print("    color: "); Serial.println( colorsensor.color());
    }
    break;

    case 1: { // ready to start
      motor1.motorBrake();
      motor2.motorBrake();
      motor1.position = 0;
      motor2.position = 0;

      //debug only
      //Serial.print("status: "); Serial.println(status); 
    }
    break;

    case 2: { // up to top
      motor1.motorGo(255);
      motor2.motorGo(255);
      
      color_status = colorsensor.color();
      if (color_status ^ ex_color_status) {
        is_red++;
        control1.real_red_line += (float) motor1.position / (float) is_red;
        control2.real_red_line += (float) motor2.position / (float) is_red;
        if (is_red == 2) {
          
        }
      }
      //debug only
      //Serial.print("status: "); Serial.print(status); Serial.print("    encoderValue: "); Serial.println(motor1.position);
    }
    break;

    case 3: { // down to DATUM line
      long output1 = (long) control1.Compute(motor1.position, control1.get_target(1));                 // calculate new output
      long output2 = (long) control2.Compute(motor2.position, control2.get_target(1));                 // calculate new output
      
      if (output1 == 0 && output2 == 0) {
        status++;
      }

      //debug only
      //Serial.print("status: "); Serial.print(status); Serial.print("    encoderValue: "); Serial.print(motor1.position); Serial.print("    target: "); Serial.print(control1.get_target(1)); Serial.print("    Output: "); Serial.println(output1);

      motor1.motorGo(output1);
      motor2.motorGo(output2);
    }
    break;

    case 4: { // go to target
      long output1 = (long) control1.Compute(motor1.position, control1.get_target(2));                 // calculate new output
      long output2 = (long) control2.Compute(motor2.position, control2.get_target(2));                 // calculate new output    

      //debug only
      //Serial.print("status: "); Serial.print(status); Serial.print("    encoderValue: "); Serial.print(motor1.position); Serial.print("    target: "); Serial.print(control1.get_target(2)); Serial.print("    Output: "); Serial.println(output1);

      motor1.motorGo(output1);
      motor2.motorGo(output2);
    }
    break;

    case 5: {
      WiFi.mode(WIFI_STA);
      WiFi.begin("UBrobotics", "ubrobotics1");
      //Serial.print("wait for connection");
      while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        //Serial.print(".");
        delay(5000);
      }

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

      //Serial.println("Ready");
      //Serial.print("IP address: ");
      //Serial.println(WiFi.localIP());
      status ++;
      digitalWrite(2,HIGH);
      //Serial.println("ready to Update");
    }
    break;

    case 6: { // OTA
      ArduinoOTA.handle();
      //Serial.print(".");
    }
    break;

    default:
      status = 0;
    break;
  }


}
