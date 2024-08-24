#ifndef RPL4_REGISTERS_GPIO_HPP_
#define RPL4_REGISTERS_GPIO_HPP_

#include <cstdint>

namespace rpl {

constexpr uint32_t kGpioAddressBase = 0xFE200000;
constexpr uint32_t kGpioRegisterSize = 0x000000F4;
struct GpioRegisterMap {
  volatile uint32_t gpfsel0;             // 0x00
  volatile uint32_t gpfsel1;             // 0x04
  volatile uint32_t gpfsel2;             // 0x08
  volatile uint32_t gpfsel3;             // 0x0c
  volatile uint32_t gpfsel4;             // 0x10
  volatile uint32_t gpfsel5;             // 0x14
  volatile uint32_t reserved_1;          // 0x18
  volatile uint32_t gpset0;              // 0x1c
  volatile uint32_t gpset1;              // 0x20
  volatile uint32_t reserved_2;          // 0x24
  volatile uint32_t gpclr0;              // 0x28
  volatile uint32_t gpclr1;              // 0x2c
  volatile uint32_t reserved_3;          // 0x30
  volatile uint32_t gplev0;              // 0x34
  volatile uint32_t gplev1;              // 0x38
  volatile uint32_t reserved_4;          // 0x3c
  volatile uint32_t gpeds0;              // 0x40
  volatile uint32_t gpeds1;              // 0x44
  volatile uint32_t reserved_5;          // 0x48
  volatile uint32_t gpren0;              // 0x4c
  volatile uint32_t gpren1;              // 0x50
  volatile uint32_t reserved_6;          // 0x54
  volatile uint32_t gpfen0;              // 0x58
  volatile uint32_t gpfen1;              // 0x5c
  volatile uint32_t reserved_7;          // 0x60
  volatile uint32_t gphen0;              // 0x64
  volatile uint32_t gphen1;              // 0x68
  volatile uint32_t reserved_8;          // 0x6c
  volatile uint32_t gplen0;              // 0x70
  volatile uint32_t gplen1;              // 0x74
  volatile uint32_t reserved_9;          // 0x78
  volatile uint32_t gparen0;             // 0x7c
  volatile uint32_t gparen1;             // 0x80
  volatile uint32_t reserved_10;         // 0x84
  volatile uint32_t gpafen0;             // 0x88
  volatile uint32_t gpafen1;             // 0x8c
  volatile uint32_t reserved_11[21];     // 0x90~0xe0
  volatile uint32_t pup_pdn_cntrl_reg0;  // 0xe4
  volatile uint32_t pup_pdn_cntrl_reg1;  // 0xe8
  volatile uint32_t pup_pdn_cntrl_reg2;  // 0xec
  volatile uint32_t pup_pdn_cntrl_reg3;  // 0xf0
};

extern GpioRegisterMap* REG_GPIO;

}  // namespace rpl

#endif