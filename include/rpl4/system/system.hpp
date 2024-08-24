#ifndef RPI4_SYSTEM_HPP_
#define RPI4_SYSTEM_HPP_

// Header files commonly used in the rpl4 library
#include "iostream"
#include "rpl4/registers/registers.hpp"
#include "rpl4/system/clock.hpp"
#include "rpl4/system/log.hpp"

namespace rpl {

extern bool system_initialized;

uint8_t Init(void);

}  // namespace rpl

#endif