#ifndef PWM_HPP_
#define PWM_HPP_

#include <stdint.h>

class PWM{
private:
    uint8_t pin;
    double freq;
    const int clock_source = 25000000;
public:
    PWM(uint8_t _pin, double _freq);
    double Write(double duty);
    double SetFrequency(double _freq);
    double operator = (double duty){
        return Write(duty);
    }
};

#endif