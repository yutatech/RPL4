#ifndef RPL4_REGISTERS_SPI_HPP_
#define RPL4_REGISTERS_SPI_HPP_

#include <cstdint>

namespace rpl {

constexpr uint32_t kSpi0AddressBase = 0xFE204000;
constexpr uint32_t kSpi3AddressBase = 0xFE204600;
constexpr uint32_t kSpi4AddressBase = 0xFE204800;
constexpr uint32_t kSpi5AddressBase = 0xFE204A00;
constexpr uint32_t kSpi6AddressBase = 0xFE204C00;
constexpr uint32_t kSpiRegisterSize = 0x00000018;

struct SpiCS {
  volatile uint32_t CS : 2;
  volatile uint32_t CPHA : 1;
  volatile uint32_t CPOL : 1;
  volatile uint32_t CLEAR : 2;
  volatile uint32_t CSPOL : 1;
  volatile uint32_t TA : 1;
  volatile uint32_t DMAEN : 1;
  volatile uint32_t INTD : 1;
  volatile uint32_t INTR : 1;
  volatile uint32_t ADCS : 1;
  volatile uint32_t REN : 1;
  volatile uint32_t LEN : 1;
  volatile uint32_t LMONO : 1;
  volatile uint32_t TE_EN : 1;
  volatile uint32_t DONE : 1;
  volatile uint32_t RXD : 1;
  volatile uint32_t TXD : 1;
  volatile uint32_t RXR : 1;
  volatile uint32_t RXF : 1;
  volatile uint32_t CSPOL0 : 1;
  volatile uint32_t CSPOL1 : 1;
  volatile uint32_t CSPOL2 : 1;
  volatile uint32_t DMA_LEN : 1;
  volatile uint32_t LEN_LONG : 1;
};

struct SpiFIFO {
  volatile uint32_t DATA : 32;
};

struct SpiCLK {
  volatile uint32_t CDIV : 16;
};

struct SpiDLEN {
  volatile uint32_t LEN : 16;
};

struct SpiLTOH {
  volatile uint32_t TOH : 4;
};

struct SpiDC {
  volatile uint32_t TDREQ : 8;
  volatile uint32_t TPANIC : 8;
  volatile uint32_t RDREQ : 8;
  volatile uint32_t RPANIC : 8;
};

struct SpiRegisterMap {
  volatile SpiCS   CS;            // 0x00
  volatile SpiFIFO FIFO;          // 0x04
  volatile SpiCLK  CLK;           // 0x08
  volatile SpiDLEN DLEN;          // 0x0c
  volatile SpiLTOH LTOH;          // 0x10
  volatile SpiDC   DC;            // 0x14
};


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