#ifndef RPI4_SYSTEM_HPP_
#define RPI4_SYSTEM_HPP_

// Header files commonly used in the rpl4 library 
#include "rpl4/registers/registers.hpp"
#include "rpl4/system/clock.hpp"
#include "rpl4/system/log.hpp"
#include "iostream"

namespace rpl {

extern bool SystemInitialized;

extern GPIO_Typedef* REG_GPIO;
extern PWM_Typedef*  REG_PWM0;
extern PWM_Typedef*  REG_PWM1;
extern CLK_Typedef*  REG_CLK;

uint8_t Init(void);
uint8_t SetGpioFunction(uint8_t pin, GPIO_Function alt);

}

#endif