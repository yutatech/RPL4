#include "rpl4/peripheral/PWM.hpp"

#include "rpl4/peripheral/gpio.hpp"
#include "rpl4/system/system.hpp"
#include <unistd.h>

PWM::PWM(uint8_t _pin) : pin(_pin) {
    switch (pin){
        case 12: case 13: case 18: case 19: case 40: case 41: case 45:
            break;
        default:
            rpl::Log(rpl::LogLevel::Error, "GPIO %d has no PWM function\n", pin);
            return;
    }
};

uint8_t PWM::Init() {
    switch (pin){
        case 12: case 13: case 40: case 41: case 45:
            rpl::Gpio::SetAltFunction(pin, rpl::Gpio::AltFunction::kAlt0);
            break;
        case 18: case 19:
            rpl::Gpio::SetAltFunction(pin, rpl::Gpio::AltFunction::kAlt5);
            break;
    }

    rpl::ClockConfig(rpl::REG_CLK->CM_PWMCTL, rpl::REG_CLK->CM_PWMDIV, rpl::CLKSRC::OSC, 2.16, 1); // set pwm source clock frequency to 25MHz
    return 0;
}

double PWM::Write(double duty){
    uint32_t dat = clock_source * duty / freq;

    switch (pin){
        case 12:
        case 18:
            rpl::REG_PWM0->DAT1 = dat;
            rpl::REG_PWM0->CTL |= 0b1;
            break;
        case 13:
        case 19:
        case 45:
            rpl::REG_PWM0->DAT2 = dat;
            rpl::REG_PWM1->CTL |= 0b1 << 8;
            break;
        case 40:
            rpl::REG_PWM1->DAT1 = dat;
            rpl::REG_PWM1->CTL |= 0b1;
            break;
        case 41:
            rpl::REG_PWM1->DAT2 = dat;
            rpl::REG_PWM1->CTL |= 0b1 << 8;
            break;
    }
    return duty;
}

double PWM::SetFrequency(double _freq){
    freq = _freq;
    uint32_t rng = clock_source / freq;
    
    switch (pin){
        case 12:
        case 18:
            rpl::REG_PWM0->CTL &= ~0b10111111;
            rpl::REG_PWM0->RNG1 = rng;
            rpl::REG_PWM0->DAT1 = 0;
            rpl::REG_PWM0->CTL |= 0b10000001;
            break;
        case 13:
        case 19:
        case 45:
            rpl::REG_PWM0->CTL &= ~(0b10111111 << 8);
            rpl::REG_PWM0->RNG2 = rng;
            rpl::REG_PWM0->DAT2 = 0;
            rpl::REG_PWM0->CTL |= 0b10000001 << 8;
            break;
        case 40:
            rpl::REG_PWM1->CTL &= ~0b10111111;
            rpl::REG_PWM1->RNG1 = rng;
            rpl::REG_PWM1->DAT1 = 0;
            rpl::REG_PWM1->CTL |= 0b10000001;
            break;
        case 41:
            rpl::REG_PWM1->CTL &= ~(0b10111111 << 8);
            rpl::REG_PWM1->RNG2 = rng;
            rpl::REG_PWM1->DAT2 = 0;
            rpl::REG_PWM1->CTL |= 0b10000001 << 8;
            break;
    }
    return freq;
}