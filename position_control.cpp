#include "position_control.h"

position_control::position_control(float kp, float ki, float kd) {
  _kp = kp;
  _ki = ki;
  _kd = kd;
  _maxoutput = 128.0;
  _minoutput = -128.0;
  _outputi = 0;
}

double position_control::Compute(double input, double target) {
  _err = target - input;
  _outputp = _kp * _err;

  if (_err < 80 && _err > -80) { // Integral separation
    _outputi += _ki * _err;
  } else {
    _outputi = 0;
  }

  _outputd = _kd * (_err - _exerr);

  if (_err < 5 && _err > -5) { // Dead zone
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
