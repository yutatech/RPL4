#ifndef RPL4_REGISTERS_BSC_HPP_
#define RPL4_REGISTERS_BSC_HPP_

#include <cstdint>

namespace rpl {

const uint32_t BSC0_BASE = 0xFE205000;
const uint32_t BSC1_BASE = 0xFE804000;
const uint32_t BSC3_BASE = 0xFE205600;
const uint32_t BSC4_BASE = 0xFE205800;
const uint32_t BSC5_BASE = 0xFE205A80;
const uint32_t BSC6_BASE = 0xFE205C00;
const uint32_t BSC_SIZE  = 0x00000020;
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
} BSC_Typedef;

extern BSC_Typedef*  REG_BSC0;
extern BSC_Typedef*  REG_BSC1;
extern BSC_Typedef*  REG_BSC3;
extern BSC_Typedef*  REG_BSC4;
extern BSC_Typedef*  REG_BSC5;
extern BSC_Typedef*  REG_BSC6;

}

#endif