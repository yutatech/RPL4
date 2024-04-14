#ifndef GPIO_HPP_
#define GPIO_HPP_

#include <cstdint>

enum class PinMode{
    input,
    input_pullup,
    input_pulldown,
    output
};

class GPIO{
private:
    uint8_t pin;
public:
    GPIO(uint8_t _pin, PinMode mode);
    bool Read();
    bool Write(bool output);
    void SetPinMode(PinMode mode);
    bool operator = (bool output){
        return Write(output);
    }
};

#endif