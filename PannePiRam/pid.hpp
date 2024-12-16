#ifndef PID_H_
#define PID_H_

#include <stdlib.h>
#include <math.h>

class TPid
{
private:
    double kp;
    double ki;
    double kd;
    double dt;
    double pre_error;
    double integral;
    double setpoint;

public:
    TPid(double kp = 0.0, double ki = 0.0, double kd = 0.0);
    ~TPid();

    double compute(double pv);
    void setSetpoint(double setpoint);
};

#endif // PID_H_
