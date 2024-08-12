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


const uint32_t SPI1_BASE = 0xFE215080;
const uint32_t SPI2_BASE = 0xFE2150C0;
const uint32_t SPI_AUX_SIZE = 0x00000040;
typedef struct
{
  volatile uint32_t CNTL0_REG;     // 0x00
  volatile uint32_t CNTL1_REG;     // 0x04
  volatile uint32_t STAT_REG;      // 0x08
  volatile uint32_t PEEK_REG;      // 0x0c
  volatile uint32_t RESERVED[4]; // 0x10 ~ 0x1c
  volatile uint32_t IO_REGa;       // 0x20
  volatile uint32_t IO_REGb;       // 0x24
  volatile uint32_t IO_REGc;       // 0x28
  volatile uint32_t IO_REGd;       // 0x2c
  volatile uint32_t TXHOLD_REGa;   // 0x30
  volatile uint32_t TXHOLD_REGb;   // 0x34
  volatile uint32_t TXHOLD_REGc;   // 0x38
  volatile uint32_t TXHOLD_REGd;   // 0x3c
} SPI_AUX_Typedef;

}

#endif