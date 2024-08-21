#include "rpl4/peripheral/GPIO.hpp"
#include "rpl4/system/system.hpp"

GPIO::GPIO(uint8_t _pin) : pin(_pin) {
    if (pin > 57)
        rpl::Log(rpl::LogLevel::Fatal, "There is no GPIO %d\n", pin);
}

bool GPIO::Read(){
    if (pin <= 31) {
        return (rpl::REG_GPIO->GPLEV0 & 0b1<<pin) == static_cast<uint32_t>(0b1<<pin);
    } else if (32 <= pin && pin <= 57) {
        return (rpl::REG_GPIO->GPLEV1 & 0b1<<(pin-32)) == static_cast<uint32_t>(0b1<<(pin-32));
    } else {
        rpl::Log(rpl::LogLevel::Error, "[GPIO::Read()] Invalid pin number %d is set.", pin);
        return false;
    }
}

bool GPIO::Write(bool output){
    if (output) {
        if (pin <= 31)
            rpl::REG_GPIO->GPSET0 |= 0b1<<pin;
        else if (32 <= pin && pin <= 57)
            rpl::REG_GPIO->GPSET1 |= 0b1<<(pin-32);
    }
    else {
        if (pin <= 31)
            rpl::REG_GPIO->GPCLR0 |= 0b1<<pin;
        else if (32 <= pin && pin <= 57)
            rpl::REG_GPIO->GPCLR1 |= 0b1<<(pin-32);
    }

    return output;
}

void GPIO::SetPinMode(PinMode mode){
    rpl::GPIO_Function bit_mode;
    uint8_t bit_pull_up_down;
    switch (mode) {
        case PinMode::input:
            bit_mode = rpl::GPIO_Function::INPUT;
            bit_pull_up_down = 0b00;
            break;
        case PinMode::input_pullup:
            bit_mode = rpl::GPIO_Function::INPUT;
            bit_pull_up_down = 0b01;
            break;
        case PinMode::input_pulldown:
            bit_mode = rpl::GPIO_Function::INPUT;
            bit_pull_up_down = 0b10;
            break;
        case PinMode::output:
            bit_mode = rpl::GPIO_Function::OUTPUT;
            bit_pull_up_down = 0b00;
            break;
    }

    //set input or output
    SetGpioFunction(pin, bit_mode);

    //set pullup or pulldown
    if (pin <= 15) {
        rpl::REG_GPIO->PUP_PDN_CNTRL_REG0 &= ~(0b11 << pin*2);
        rpl::REG_GPIO->PUP_PDN_CNTRL_REG0 |= bit_pull_up_down << pin*2;
    }
    else if (16 <= pin && pin <= 31) {
        rpl::REG_GPIO->PUP_PDN_CNTRL_REG1 &= ~(0b11 << (pin-16)*2);
        rpl::REG_GPIO->PUP_PDN_CNTRL_REG1 |= bit_pull_up_down << (pin-16)*2;
    }
    else if (32 <= pin && pin <= 47) {
        rpl::REG_GPIO->PUP_PDN_CNTRL_REG2 &= ~(0b11 << (pin-32)*2);
        rpl::REG_GPIO->PUP_PDN_CNTRL_REG2 |= bit_pull_up_down << (pin-32)*2;
    }
    else if (48 <= pin && pin <= 57) {
        rpl::REG_GPIO->PUP_PDN_CNTRL_REG3 &= ~(0b11 << (pin-48)*2);
        rpl::REG_GPIO->PUP_PDN_CNTRL_REG3 |= bit_pull_up_down << (pin-48)*2;
    }
}