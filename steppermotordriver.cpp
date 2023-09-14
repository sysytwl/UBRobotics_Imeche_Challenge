#include "Arduino.h"

class StepperMotor {
public:
    StepperMotor(int pinA, int pinB, int pinC, int pinD, int stepsPerCircle, int speed) {
        _pins[0] = pinA;
        _pins[1] = pinB;
        _pins[2] = pinC;
        _pins[3] = pinD;
        _stepsPerCircle = stepsPerCircle;
        _speed = (60000000L / stepsPerCircle) / speed; // Convert RPM to microseconds per step

        // Initialize the pins as OUTPUT
        for (int i = 0; i < 4; i++) {
            pinMode(_pins[i], OUTPUT);
        }
    }

    // Rotate the stepper motor forward by a specified number of steps
    void forwardStep(int steps) {
        for (int i = 0; i < steps; i++) {
            if (stopped) {
                //stopped = false;
                break; // Exit the loop if stopped
            }
            stepRecords++;
            _currentStep = stepRecords % 4;
            driveStep(_stepDictionary[_currentStep]);
            delayMicroseconds(_speed);
        }
    }

    // Rotate the stepper motor backward by a specified number of steps
    void backwardStep(int steps) {
        for (int i = 0; i < steps; i++) {
            if (stopped) {
                //stopped = false;
                break; // Exit the loop if stopped
            }
            stepRecords--;
            _currentStep = stepRecords % 4;
            driveStep(_stepDictionary[_currentStep]);
            delayMicroseconds(_speed);
        }
    }

    // Rotate the stepper motor with a certain angle (in degrees)
    void rotate(int angle) {
        int steps = (_stepsPerCircle * angle) / 360;
        if (steps > 0) {
            forwardStep(steps);
        } else {
            backwardStep(-steps);
        }
    }

    // Stop the stepper motor
    void stop() {
        stopped = true;
    }

    // Resume the stepper motor
    void resume() {
        stopped = false;
    }

    // Brake the stepper motor by setting all pins to high
    void brake() {
        for (int i = 0; i < 4; i++) {
            digitalWrite(_pins[i], HIGH);
        }
        stopped = true;
    }

    // Disable the stepper motor by setting all pins to low
    void disable() {
        for (int i = 0; i < 4; i++) {
            digitalWrite(_pins[i], LOW);
        }
        stopped = true;
    }

    // Enable the stepper motor by setting pins to stepDictionary[0]
    void enable() {
        stopped = false;
        stepRecords = 0;
        driveStep(_stepDictionary[0]);
    }

    int stepRecords = 0;
    bool stopped = true; // Status variable to indicate if the motor is stopped

private:
    int _pins[4];
    int _stepsPerCircle;
    int _speed;
    int _currentStep;
    int _stepDictionary[4] = {0b1100, 0b0110, 0b0011, 0b1001};

    // Function to drive the stepper motor based on the step pattern
    void driveStep(int pattern) {
        for (int i = 0; i < 4; i++) {
            digitalWrite(_pins[i], (pattern >> i) & 1);
        }
    }
};
