#ifndef PWMMOTORDRIVE_H
#define PWMMOTORDRIVE_H

#include <Arduino.h>

class pwmmotordrive {
public:
    pwmmotordrive(uint8_t pwmpin, uint8_t dirpin, uint8_t breakpin, uint8_t pwm_channel = 2, uint32_t freq = 5000, uint8_t resolution_bits = 8);
    
    void motorGo(long speed);
    void motorBreak();

private:
    uint8_t _pwmpin, _dirpin, _breakpin, _pwm_channel;
};

#endif // PWMMOTORDRIVE_H
