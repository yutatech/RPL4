#ifndef RPL4_REGISTERS_GPIO_HPP_
#define RPL4_REGISTERS_GPIO_HPP_

#include <cstdint>

namespace rpl {

const uint32_t GPIO_BASE = 0xFE200000;
const uint32_t GPIO_SIZE = 0x000000F4;
typedef struct
{
  volatile uint32_t GPFSEL0;               // 0x00
  volatile uint32_t GPFSEL1;               // 0x04
  volatile uint32_t GPFSEL2;               // 0x08
  volatile uint32_t GPFSEL3;               // 0x0c
  volatile uint32_t GPFSEL4;               // 0x10
  volatile uint32_t GPFSEL5;               // 0x14
  volatile uint32_t RESERVED_1;            // 0x18
  volatile uint32_t GPSET0;                // 0x1c
  volatile uint32_t GPSET1;                // 0x20
  volatile uint32_t RESERVED_2;            // 0x24
  volatile uint32_t GPCLR0;                // 0x28
  volatile uint32_t GPCLR1;                // 0x2c
  volatile uint32_t RESERVED_3;            // 0x30
  volatile uint32_t GPLEV0;                // 0x34
  volatile uint32_t GPLEV1;                // 0x38
  volatile uint32_t RESERVED_4;            // 0x3c
  volatile uint32_t GPEDS0;                // 0x40
  volatile uint32_t GPEDS1;                // 0x44
  volatile uint32_t RESERVED_5;            // 0x48
  volatile uint32_t GPREN0;                // 0x4c
  volatile uint32_t GPREN1;                // 0x50
  volatile uint32_t RESERVED_6;            // 0x54
  volatile uint32_t GPFEN0;                // 0x58
  volatile uint32_t GPFEN1;                // 0x5c
  volatile uint32_t RESERVED_7;            // 0x60
  volatile uint32_t GPHEN0;                // 0x64
  volatile uint32_t GPHEN1;                // 0x68
  volatile uint32_t RESERVED_8;            // 0x6c
  volatile uint32_t GPLEN0;                // 0x70
  volatile uint32_t GPLEN1;                // 0x74
  volatile uint32_t RESERVED_9;            // 0x78
  volatile uint32_t GPAREN0;               // 0x7c
  volatile uint32_t GPAREN1;               // 0x80
  volatile uint32_t RESERVED_10;           // 0x84
  volatile uint32_t GPAFEN0;               // 0x88
  volatile uint32_t GPAFEN1;               // 0x8c
  volatile uint32_t RESERVED_11[21];       // 0x90~0xe0
  volatile uint32_t PUP_PDN_CNTRL_REG0;    // 0xe4
  volatile uint32_t PUP_PDN_CNTRL_REG1;    // 0xe8
  volatile uint32_t PUP_PDN_CNTRL_REG2;    // 0xec
  volatile uint32_t PUP_PDN_CNTRL_REG3;    // 0xf0
} GPIO_Typedef;

enum class GPIO_Function : uint8_t
{
  INPUT =  0b000,
  OUTPUT = 0b001,
  ALT0   = 0b100,
  ALT1   = 0b101,
  ALT2   = 0b110,
  ALT3   = 0b111,
  ALT4   = 0b011,
  ALT5   = 0b010,
};

}

#endif