#ifndef RPI4_SYSTEM_HPP_
#define RPI4_SYSTEM_HPP_

// Header files commonly used in the rpl4 library 
#include "system/registers/rpl4_registers.hpp"
#include "rpl4_clock.hpp"
#include "rpl4_message.hpp"
#include "iostream"

namespace rpl {

uint8_t Init(void);
uint8_t SetGpioFunction(uint8_t pin, GPIO_Function alt);

extern GPIO_Typedef* REG_GPIO;
extern PWM_Typedef*  REG_PWM0;
extern PWM_Typedef*  REG_PWM1;
extern CLK_Typedef*  REG_CLK;

}

#endif