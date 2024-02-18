#ifndef RPL4_CLOCK_HPP_
#define RPL4_CLOCK_HPP_

#include <stdint.h>

namespace rpl {

enum class CLKSRC : uint8_t{
    GND  = 0,       // 0Hz
    OSC  = 1,       // 54MHz
    PLLA = 4,       // 0Hz
    PLLC = 5,       // ?MHz
    PLLD = 6,       // ?MHz
    HDMI = 7        // ?MHz
};

void ClockConfig(volatile uint32_t &reg_ctl, volatile uint32_t &reg_div, CLKSRC src, double div, uint8_t mash);

}

#endif