#include "drivers/GPIO.hpp"
#include "system/rpl4_system.hpp"

GPIO::GPIO(uint8_t _pin, PinMode mode) : pin(_pin) {
    if (pin < 0 || 57 < pin)
        rpl::Message("There is no GPIO" + std::to_string(pin), rpl::MessageLevel::error);

    SetPinMode(mode);
}

bool GPIO::Read(){
    if (0 <= pin && pin <= 31)
        return (rpl::REG_GPIO->GPLEV0 & 0b1<<pin) == 0b1<<pin;
    else if (32 <= pin && pin <= 57)
        return (rpl::REG_GPIO->GPLEV1 & 0b1<<(pin-32)) == 0b1<<(pin-32);
}

bool GPIO::Write(bool output){
    if (output) {
        if (0 <= pin && pin <= 31)
            rpl::REG_GPIO->GPSET0 |= 0b1<<pin;
        else if (32 <= pin && pin <= 57)
            rpl::REG_GPIO->GPSET1 |= 0b1<<(pin-32);
    }
    else {
        if (0 <= pin && pin <= 31)
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
    if (0 <= pin && pin <= 15) {
        rpl::REG_GPIO->GPFSEL5 &= ~(0b11 << pin*2);
        rpl::REG_GPIO->GPFSEL5 |= bit_pull_up_down << pin*2;
    }
    if (16 <= pin && pin <= 31) {
        rpl::REG_GPIO->GPFSEL5 &= ~(0b11 << (pin-16)*2);
        rpl::REG_GPIO->GPFSEL5 |= bit_pull_up_down << (pin-16)*2;
    }
    if (32 <= pin && pin <= 47) {
        rpl::REG_GPIO->GPFSEL5 &= ~(0b11 << (pin-32)*2);
        rpl::REG_GPIO->GPFSEL5 |= bit_pull_up_down << (pin-32)*2;
    }
    if (48 <= pin && pin <= 57) {
        rpl::REG_GPIO->GPFSEL5 &= ~(0b11 << (pin-48)*2);
        rpl::REG_GPIO->GPFSEL5 |= bit_pull_up_down << (pin-48)*2;
    }
}