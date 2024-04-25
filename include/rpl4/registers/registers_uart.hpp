#ifndef RPL4_REGISTERS_UART_HPP_
#define RPL4_REGISTERS_UART_HPP_

#include <cstdint>

namespace rpl {

const uint32_t UART0_BASE = 0xFE201000;
const uint32_t UART2_BASE = 0xFE201400;
const uint32_t UART3_BASE = 0xFE201600;
const uint32_t UART4_BASE = 0xFE201800;
const uint32_t UART5_BASE = 0xFE201A00;
const uint32_t UART_SIZE  = 0x00000090;
typedef struct
{
  volatile uint32_t DR;            // 0x00
  volatile uint32_t RSRECR;        // 0x04
  volatile uint32_t RESERVED_1[4]; // 0x08 ~ 0x14
  volatile uint32_t FR;            // 0x18
  volatile uint32_t RESERVED_2;    // 0x1c
  volatile uint32_t ILPR;          // 0x20
  volatile uint32_t IBRD;          // 0x24
  volatile uint32_t FBRD;          // 0x28
  volatile uint32_t LCRH;          // 0x2c
  volatile uint32_t CR;            // 0x30
  volatile uint32_t IFLS;          // 0x34
  volatile uint32_t IMSC;          // 0x38
  volatile uint32_t RIS;           // 0x3c
  volatile uint32_t MIS;           // 0x40
  volatile uint32_t ICR;           // 0x44
  volatile uint32_t DMACR;         // 0x48
  volatile uint32_t RESERVED_3[13];// 0x04c ~ 0x7c
  volatile uint32_t ITCR;          // 0x80
  volatile uint32_t ITIP;          // 0x84
  volatile uint32_t ITOP;          // 0x88
  volatile uint32_t TDR;           // 0x8c
} UART_Typedef;

}

#endif