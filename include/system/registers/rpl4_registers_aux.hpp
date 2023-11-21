#ifndef RPL4_REGISTERS_PWM_HPP_
#define RPL4_REGISTERS_PWM_HPP_

#include <stdint.h>

namespace rpl {
namespace registers {

const uint32_t PWM0_BASE = 0xFE20C000;
const uint32_t PWM1_BASE = 0xFE20C800;
const uint32_t PWM_SIZE  = 0x00000028;
typedef struct
{
  volatile uint32_t CTL;           // 0x00
  volatile uint32_t STA;           // 0x04
  volatile uint32_t DMAC;          // 0x08
  volatile uint32_t RESERVED_1;    // 0x0c
  volatile uint32_t RNG1;          // 0x10
  volatile uint32_t DAT1;          // 0x14
  volatile uint32_t FIF1;          // 0x18
  volatile uint32_t RESERVED_2;    // 0x1c
  volatile uint32_t RNG2;          // 0x20
  volatile uint32_t DAT2;          // 0x24
} PWM_Typedef;

}
}

#endif