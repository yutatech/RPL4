#ifndef RPL4_SYSTEM_HPP
#define RPL4_SYSTEM_HPP

// Header files commonly used in the rpl4 library
#include "iostream"
#include "rpl4/registers/registers.hpp"
#include "rpl4/system/clock.hpp"
#include "rpl4/system/log.hpp"

namespace rpl {

extern bool system_initialized;

/**
 * @brief Check if rpl is available
 * 
 * @details If the system doen't have the file dev/mem, it is not available.
 * 
 * @return true if available, false otherwise
 */
bool IsAvailable(void);

uint8_t Init(void);

}  // namespace rpl

#endif // RPL4_SYSTEM_HPP