#include "pid.hpp"

TPid::TPid(double kp, double ki, double kd)
    : kp(kp), ki(ki), kd(kd),  pre_error(0), integral(0)
{
    dt = 0.05;
}

TPid::~TPid()
{
}

double TPid::compute(double pv)
{
    // Calcul erreur
    double error = setpoint - pv;

    // Calcul de la partie proportionnelle
    double Pout = kp * error;

    // Calcul de la partie intégrale
    double Iout = 0;
    if ( error > -20 && error < 20 )
    {
        integral += error * dt;
        Iout = ki * integral;
    }

    // Calcul de la partie dérivée
    double derivative = (error - pre_error) / dt;
    double Dout = kd * derivative;
    
    // Calcul de la sortie
    double output = Pout + Iout + Dout;

    // Mise à jour de l'erreur
    pre_error = error;

    return output;
}

void TPid::setSetpoint(double setpoint)
{
    this->integral -= (this->setpoint - setpoint) / 100 * 20;
    this->setpoint = setpoint;
}
