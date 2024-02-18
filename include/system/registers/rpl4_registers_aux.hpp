#ifndef RPL4_REGISTERS_AUX_HPP_
#define RPL4_REGISTERS_AUX_HPP_

#include <stdint.h>

namespace rpl {

const uint32_t AUX_BASE = 0xFE215000;
const uint32_t AUX_SIZE = 0x00000100;
typedef struct
{
    volatile uint32_t AUX_IRQ;               // 0x00
    volatile uint32_t AUX_ENABLES;           // 0x04
    volatile uint32_t RESERVED_1[16];        // 0x08 ~ 0x3c
    volatile uint32_t AUX_MU_IO_REG;         // 0x40
    volatile uint32_t AUX_MU_IER_REG;        // 0x44
    volatile uint32_t AUX_MU_IIR_REG;        // 0x48
    volatile uint32_t AUX_MU_LCR_REG;        // 0x4c
    volatile uint32_t AUX_MU_MCR_REG;        // 0x50
    volatile uint32_t AUX_MU_LSR_REG;        // 0x54
    volatile uint32_t AUX_MU_MSR_REG;        // 0x58
    volatile uint32_t AUX_MU_SCRATCH;        // 0x5c
    volatile uint32_t AUX_MU_CNTL_REG;       // 0x60
    volatile uint32_t AUX_MU_STAT_REG;       // 0x64
    volatile uint32_t AUX_MU_BAUD_REG;       // 0x68
    volatile uint32_t RESERVED_2[5];         // 0x6c ~ 0x7c
    volatile uint32_t AUX_SPI1_CNTL0_REG;    // 0x80
    volatile uint32_t AUX_SPI1_CNTL1_REG;    // 0x84
    volatile uint32_t AUX_SPI1_STAT_REG;     // 0x88
    volatile uint32_t AUX_SPI1_PEEK_REG;     // 0x8c
    volatile uint32_t RESERVED_3[4];         // 0x90 ~ 0x9c
    volatile uint32_t AUX_SPI1_IO_REGa;      // 0xa0
    volatile uint32_t AUX_SPI1_IO_REGb;      // 0xa4
    volatile uint32_t AUX_SPI1_IO_REGc;      // 0xa8
    volatile uint32_t AUX_SPI1_IO_REGd;      // 0xac
    volatile uint32_t AUX_SPI1_TXHOLD_REGa;  // 0xb0
    volatile uint32_t AUX_SPI1_TXHOLD_REGb;  // 0xb4
    volatile uint32_t AUX_SPI1_TXHOLD_REGc;  // 0xb8
    volatile uint32_t AUX_SPI1_TXHOLD_REGd;  // 0xbc
    volatile uint32_t AUX_SPI2_CNTL0_REG;    // 0xc0
    volatile uint32_t AUX_SPI2_CNTL1_REG;    // 0xc4
    volatile uint32_t AUX_SPI2_STAT_REG;     // 0xc8
    volatile uint32_t AUX_SPI2_PEEK_REG;     // 0xcc
    volatile uint32_t RESERVED_4[4];         // 0xd0 ~0xdc
    volatile uint32_t AUX_SPI2_IO_REGa;      // 0xe0
    volatile uint32_t AUX_SPI2_IO_REGb;      // 0xe4
    volatile uint32_t AUX_SPI2_IO_REGc;      // 0xe8
    volatile uint32_t AUX_SPI2_IO_REGd;      // 0xec
    volatile uint32_t AUX_SPI2_TXHOLD_REGa;  // 0xf0
    volatile uint32_t AUX_SPI2_TXHOLD_REGb;  // 0xf4
    volatile uint32_t AUX_SPI2_TXHOLD_REGc;  // 0xf8
    volatile uint32_t AUX_SPI2_TXHOLD_REGd;  // 0xfc
} AUX_Typedef;

}

#endif