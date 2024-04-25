#ifndef RPL4_REGISTERS_SPI_HPP_
#define RPL4_REGISTERS_SPI_HPP_

#include <cstdint>

namespace rpl {

const uint32_t SPI0_BASE = 0xFE204000;
const uint32_t SPI3_BASE = 0xFE204600;
const uint32_t SPI4_BASE = 0xFE204800;
const uint32_t SPI5_BASE = 0xFE204A00;
const uint32_t SPI6_BASE = 0xFE204C00;
const uint32_t SPI_SIZE  = 0x00000018;
typedef struct
{
  volatile uint32_t CS;            // 0x00
  volatile uint32_t FIFO;          // 0x04
  volatile uint32_t CLK;           // 0x08
  volatile uint32_t DLEN;          // 0x0c
  volatile uint32_t LTOH;          // 0x10
  volatile uint32_t DC;            // 0x14
} SPI_Typedef;

}

#endif