class position_control {
  public:
    position_control(float kp, float ki, float kd) { // kp, ki, kd
      _kp = kp;
      _ki = ki;
      _kd = kd;
      _maxoutput = 255;
      _minoutput = -255;
      _home = -10;
    }

    float Compute(float input, float target) {
      _err = target - input;

      _outputp = _kp * _err;
      if (_err < 100 && _err > -100){ //Integral separation
        _outputi += _ki * _err;
      }
      _outputd = _kd * (_err - _exerr);

      if (_err < 3 && _err > -3) { // dead zone
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

      _exerr = _err;
    
    return _output;
    }

    int get_target(int input) {
      if (input < -3){
        cases = 2;
      }

      switch(cases) {
        case 0: return _positive_inf;
        case 1: return _home;
        case 2: return _red_line;
      }
    }

    void position_record(int color, int position) {
      if (_excolor != color) {
        _exposition
      }
    }

    int cases = 0;

  private:
    float _maxoutput, _minoutput, _outputp, _outputi = 0, _outputd, _output;
    float _err, _exerr;

    int _home, _positive_inf = 999999, _red_line = 999, _offset, _exposition, _excolor;

    float _kp;
    float _ki;
    float _kd;
};

float kp = 0.72 , ki = 0.004 , kd = 0.15;             // modify for optimal performance
position_control control1(kp, ki, kd);
position_control control2(kp, ki, kd);

class Motor {
  public:
    Motor( uint8_t pinIN1, uint8_t pinIN2, uint8_t ledCH1, uint8_t ledCH2, uint8_t pinIN3, uint8_t pinIN4) { // motor pin1, motor pin2, pwm channel 1, pwm channel 2, encoder pin1, encoder pin2
      _ledCH1 = ledCH1;         // ESP32 LED Channel for PWM to Pin  
      _ledCH2 = ledCH2;         // 0 - 15 are availible by default
      pinA = pinIN3; 
      pinB = pinIN4; 
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
        //motorBrake();
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
    int distance = 0;

  private:
    uint8_t _ledCH1;        // ESP32 ledc Channel for PWM   
    uint8_t _ledCH2;
	  long _pwmVal;           // PWM Value (speed)
    long _maxpwm;           // Max PWM Value of the Motor

};

Motor motor1(22, 23, 0, 1, 35, 34);
Motor motor2(19, 21, 2, 3, 32, 33);

void IRAM_ATTR encoder_A1() {
  if (digitalRead(motor1.pinA) ^ digitalRead(motor1.pinB)) {
    motor1.distance--;
    //motor1.lastPulsePeriod = -(micros() - motor1.lastPulseTime);
  } else {
    motor1.distance++;
    //motor1.lastPulsePeriod = (micros() - motor1.lastPulseTime);
  }
  //motor1.lastPulseTime = micros();
}

void IRAM_ATTR encoder_A2() {
  if (digitalRead(motor1.pinB) ^ digitalRead(motor1.pinA)) {
    motor1.distance++;
    //motor1.lastPulsePeriod = (micros() - motor1.lastPulseTime);
  } else {
    motor1.distance--;
    //motor1.lastPulsePeriod = -(micros() - motor1.lastPulseTime);
  }
  //motor1.lastPulseTime = micros();
}

void IRAM_ATTR encoder_B1() {
  if (digitalRead(motor2.pinA) ^ digitalRead(motor2.pinB)) {
    motor2.distance--;
    //motor2.lastPulsePeriod = -(micros() - motor2.lastPulseTime);
  } else {
    motor2.distance++;
    //motor2.lastPulsePeriod = (micros() - motor2.lastPulseTime);
  }
  //motor2.lastPulseTime = micros();
}

void IRAM_ATTR encoder_B2() {
  if (digitalRead(motor2.pinB) ^ digitalRead(motor2.pinA)) {
    motor2.distance++;
    //motor2.lastPulsePeriod = (micros() - motor2.lastPulseTime);
  } else {
    motor2.distance--;
    //motor2.lastPulsePeriod = -(micros() - motor2.lastPulseTime);
  }
  //motor2.lastPulseTime = micros();
}

void IRAM_ATTR down_to_home() {
  control1.cases = 1;
  control2.cases = 1;
}

void setup() {
  Serial.begin(115200); //initialize serial comunication
  //Serial.print("setup running on core "); Serial.println(xPortGetCoreID());

  //encoder counter
  pinMode(motor1.pinA, INPUT_PULLDOWN); //INPUT_PULLUP ?
  pinMode(motor1.pinB, INPUT_PULLDOWN);
  attachInterrupt(motor1.pinA, encoder_A1, CHANGE);
  attachInterrupt(motor1.pinB, encoder_A2, CHANGE);
  pinMode(motor2.pinA, INPUT_PULLDOWN); //INPUT_PULLUP ?
  pinMode(motor2.pinB, INPUT_PULLDOWN);
  attachInterrupt(motor2.pinA, encoder_B1, CHANGE);
  attachInterrupt(motor2.pinB, encoder_B2, CHANGE);

  //top switch
  pinMode(25, INPUT_PULLUP);
  attachInterrupt(25, down_to_home, FALLING);
}

void loop(){
  //Serial.print("PID running on core "); Serial.println(xPortGetCoreID());
  int input1 = motor1.distance;            // data from encoder consider as a Process value
  int input2 = motor2.distance;

  int target1 = control1.get_target(input1);
  int target2 = control2.get_target(input2);

  int output1 = control1.Compute(input1, target1);                 // calculate new output
  int output2 = control2.Compute(input2, target2);                 // calculate new output    
  Serial.print("encoderValue: "); Serial.print(input1); Serial.print(", "); Serial.print(target1); Serial.print("    Output"); Serial.println(output1);

  motor1.motorGo(output1);
  motor2.motorGo(output2);

  //color();
}
