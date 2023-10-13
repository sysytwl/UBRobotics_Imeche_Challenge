#include "pwmmotordrive.h"

pwmmotordrive::pwmmotordrive(uint8_t pwmpin, uint8_t dirpin, uint8_t breakpin, uint8_t pwm_channel, uint32_t freq, uint8_t resolution_bits) {
    _pwmpin = pwmpin;
    _dirpin = dirpin;
    _breakpin = breakpin;
    _pwm_channel = pwm_channel;

    pinMode(_breakpin, OUTPUT);
    pinMode(_dirpin, OUTPUT);
    ledcSetup(_pwm_channel, freq, resolution_bits);
    ledcAttachPin(_pwmpin, _pwm_channel);
}

void pwmmotordrive::motorGo(long speed) {
    if (speed > 0) {
        digitalWrite(_breakpin, HIGH);
        digitalWrite(_dirpin, HIGH);
        ledcWrite(_pwm_channel, speed);
    } else if (speed < 0) {
        digitalWrite(_breakpin, HIGH);
        digitalWrite(_dirpin, LOW);
        ledcWrite(_pwm_channel, -speed);
    } else {
        digitalWrite(_breakpin, LOW);
    }
}

void pwmmotordrive::motorBreak() {
    digitalWrite(_breakpin, LOW);
}
