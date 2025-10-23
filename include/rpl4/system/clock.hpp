#ifndef RPL4_SYSTEM_CLOCK_HPP_
#define RPL4_SYSTEM_CLOCK_HPP_

#include <cstdint>

namespace rpl {

enum class ClockSource : uint8_t {
  kGnd = 0,   // 0Hz
  kOsc = 1,   // 54MHz
  kPllA = 4,  // 0Hz
  kPllC = 5,  // ?MHz
  kPllD = 6,  // ?MHz
  kHdmi = 7   // ?MHz
};

void ClockConfig(volatile uint32_t& reg_ctl, volatile uint32_t& reg_div,
                 ClockSource src, double div, uint8_t mash);

}  // namespace rpl

#endif  // RPL4_SYSTEM_CLOCK_HPP_