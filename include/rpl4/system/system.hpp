#ifndef RPI4_SYSTEM_HPP_
#define RPI4_SYSTEM_HPP_

// Header files commonly used in the rpl4 library 
#include "rpl4/registers/registers.hpp"
#include "rpl4/system/clock.hpp"
#include "rpl4/system/log.hpp"
#include "iostream"

namespace rpl {

extern bool system_initialized;

extern AUX_Typedef*  REG_AUX;
extern BSC_Typedef*  REG_BSC0;
extern BSC_Typedef*  REG_BSC1;
extern BSC_Typedef*  REG_BSC3;
extern BSC_Typedef*  REG_BSC4;
extern BSC_Typedef*  REG_BSC5;
extern BSC_Typedef*  REG_BSC6;
extern CLK_Typedef*  REG_CLK;
extern DMA_Typedef*  REG_DMA0;
extern DMA_Typedef*  REG_DMA1;
extern DMA_Typedef*  REG_DMA2;
extern DMA_Typedef*  REG_DMA3;
extern DMA_Typedef*  REG_DMA4;
extern DMA_Typedef*  REG_DMA5;
extern DMA_Typedef*  REG_DMA6;
extern DMA_Typedef*  REG_DMA7;
extern DMA_Typedef*  REG_DMA8;
extern DMA_Typedef*  REG_DMA9;
extern DMA_Typedef*  REG_DMA10;
extern DMA_Typedef*  REG_DMA11;
extern DMA_Typedef*  REG_DMA12;
extern DMA_Typedef*  REG_DMA13;
extern DMA_Typedef*  REG_DMA14;
extern GPIO_Typedef* REG_GPIO;
extern PWM_Typedef*  REG_PWM0;
extern PWM_Typedef*  REG_PWM1;
extern SpiRegisterMap*  REG_SPI0;
extern SPI_AUX_Typedef*  REG_SPI1;
extern SPI_AUX_Typedef*  REG_SPI2;
extern SpiRegisterMap*  REG_SPI3;
extern SpiRegisterMap*  REG_SPI4;
extern SpiRegisterMap*  REG_SPI5;
extern SpiRegisterMap*  REG_SPI6;
extern UART_Typedef*  REG_UART0;
extern UART_Typedef*  REG_UART2;
extern UART_Typedef*  REG_UART3;
extern UART_Typedef*  REG_UART4;
extern UART_Typedef*  REG_UART5;
extern UART_AUX_Typedef*  REG_UART1;

uint8_t Init(void);
uint8_t SetGpioFunction(uint8_t pin, GPIO_Function alt);

}

#endif