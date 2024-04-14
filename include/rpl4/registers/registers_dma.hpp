#ifndef RPL4_REGISTERS_DMA_HPP_
#define RPL4_REGISTERS_DMA_HPP_

#include <cstdint>

namespace rpl {

const uint32_t DMA0_BASE  = 0xFE007000;
const uint32_t DMA1_BASE  = 0xFE007100;
const uint32_t DMA2_BASE  = 0xFE007200;
const uint32_t DMA3_BASE  = 0xFE007300;
const uint32_t DMA4_BASE  = 0xFE007400;
const uint32_t DMA5_BASE  = 0xFE007500;
const uint32_t DMA6_BASE  = 0xFE007600;
const uint32_t DMA7_BASE  = 0xFE007700;
const uint32_t DMA8_BASE  = 0xFE007800;
const uint32_t DMA9_BASE  = 0xFE007900;
const uint32_t DMA10_BASE = 0xFE007A00;
const uint32_t DMA11_BASE = 0xFE007B00;
const uint32_t DMA12_BASE = 0xFE007C00;
const uint32_t DMA13_BASE = 0xFE007D00;
const uint32_t DMA14_BASE = 0xFEE05000;
const uint32_t DMA_SIZE   = 0x00000020;
typedef struct
{
  volatile uint32_t C;      // 0x00
  volatile uint32_t S;      // 0x04
  volatile uint32_t DLEN;   // 0x08
  volatile uint32_t A;      // 0x0c
  volatile uint32_t FIFO;   // 0x10
  volatile uint32_t DIV;    // 0x14
  volatile uint32_t DEL;    // 0x18
  volatile uint32_t CLKT;   // 0x1c
} DMA_Typedef;

}

#endif