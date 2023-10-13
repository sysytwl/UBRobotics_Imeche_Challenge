#ifndef POSITION_CONTROL_H
#define POSITION_CONTROL_H

class position_control {
public:
  position_control(float kp, float ki, float kd);

  double Compute(double input, double target);

private:
  double _maxoutput, _minoutput, _outputp, _outputi, _outputd, _output;
  double _err, _exerr;
  double _kp, _ki, _kd;
};

#endif // POSITION_CONTROL_H
