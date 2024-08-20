#ifndef RPI4_SYSTEM_HPP_
#define RPI4_SYSTEM_HPP_

// Header files commonly used in the rpl4 library 
#include "rpl4/registers/registers.hpp"
#include "rpl4/system/clock.hpp"
#include "rpl4/system/log.hpp"
#include "iostream"

namespace rpl {

extern bool system_initialized;

uint8_t Init(void);
uint8_t SetGpioFunction(uint8_t pin, GPIO_Function alt);

} //rpl

#endif