#include "rpl4/system/system.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <sys/stat.h>

namespace rpl {

bool system_initialized = false;

AUX_Typedef*  REG_AUX;
BSC_Typedef*  REG_BSC0;
BSC_Typedef*  REG_BSC1;
BSC_Typedef*  REG_BSC3;
BSC_Typedef*  REG_BSC4;
BSC_Typedef*  REG_BSC5;
BSC_Typedef*  REG_BSC6;
CLK_Typedef*  REG_CLK;
DMA_Typedef*  REG_DMA0;
DMA_Typedef*  REG_DMA1;
DMA_Typedef*  REG_DMA2;
DMA_Typedef*  REG_DMA3;
DMA_Typedef*  REG_DMA4;
DMA_Typedef*  REG_DMA5;
DMA_Typedef*  REG_DMA6;
DMA_Typedef*  REG_DMA7;
DMA_Typedef*  REG_DMA8;
DMA_Typedef*  REG_DMA9;
DMA_Typedef*  REG_DMA10;
DMA_Typedef*  REG_DMA11;
DMA_Typedef*  REG_DMA12;
DMA_Typedef*  REG_DMA13;
DMA_Typedef*  REG_DMA14;
GPIO_Typedef* REG_GPIO;
PWM_Typedef*  REG_PWM0;
PWM_Typedef*  REG_PWM1;
SPI_Typedef*  REG_SPI0;
SPI_AUX_Typedef*  REG_SPI1;
SPI_AUX_Typedef*  REG_SPI2;
SPI_Typedef*  REG_SPI3;
SPI_Typedef*  REG_SPI4;
SPI_Typedef*  REG_SPI5;
SPI_Typedef*  REG_SPI6;
UART_Typedef*  REG_UART0;
UART_Typedef*  REG_UART2;
UART_Typedef*  REG_UART3;
UART_Typedef*  REG_UART4;
UART_Typedef*  REG_UART5;
UART_AUX_Typedef*  REG_UART1;

uint8_t Init(void){
	int fd;
	if ((fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0) {
        Log(LogLevel::Fatal, "Can't open /dev/mem. Root privileges required.");
        return -1;
    }

    constexpr static uint32_t region0_base = 0xfe007000;
    constexpr static uint32_t region0_size = 0x1000;
    uint32_t* region0 = reinterpret_cast<uint32_t*>(mmap(NULL, region0_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region0_base));
    REG_DMA0 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA0_BASE);
    REG_DMA1 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA1_BASE);
    REG_DMA2 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA2_BASE);
    REG_DMA3 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA3_BASE);
    REG_DMA4 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA4_BASE);
    REG_DMA5 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA5_BASE);
    REG_DMA6 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA6_BASE);
    REG_DMA7 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA7_BASE);
    REG_DMA8 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA8_BASE);
    REG_DMA9 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA9_BASE);
    REG_DMA10 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA10_BASE);
    REG_DMA11 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA11_BASE);
    REG_DMA12 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA12_BASE);
    REG_DMA13 = reinterpret_cast<DMA_Typedef*>(region0 + region0_base - DMA13_BASE);

    constexpr static uint32_t region1_base = 0xfe101000;
    constexpr static uint32_t region1_size = 0x1000;
    uint32_t* region1 = reinterpret_cast<uint32_t*>(mmap(NULL, region1_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region1_base));
    REG_CLK = reinterpret_cast<CLK_Typedef*>(region1 + region1_base - CLK_BASE);

    constexpr static uint32_t region2_base = 0xfe200000;
    constexpr static uint32_t region2_size = 0x2000;
    uint32_t* region2 = reinterpret_cast<uint32_t*>(mmap(NULL, region2_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region2_base));
    REG_GPIO = reinterpret_cast<GPIO_Typedef*>(region2 + region2_base - GPIO_BASE);
    REG_UART0 = reinterpret_cast<UART_Typedef*>(region2 + region2_base - UART0_BASE);
    REG_UART2 = reinterpret_cast<UART_Typedef*>(region2 + region2_base - UART2_BASE);
    REG_UART3 = reinterpret_cast<UART_Typedef*>(region2 + region2_base - UART3_BASE);
    REG_UART4 = reinterpret_cast<UART_Typedef*>(region2 + region2_base - UART4_BASE);
    REG_UART5 = reinterpret_cast<UART_Typedef*>(region2 + region2_base - UART5_BASE);

    constexpr static uint32_t region3_base = 0xfe204000;
    constexpr static uint32_t region3_size = 0x2000;
    uint32_t* region3 = reinterpret_cast<uint32_t*>(mmap(NULL, region3_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region3_base));
    REG_SPI0 = reinterpret_cast<SPI_Typedef*>(region3 + region3_base - SPI0_BASE);
    REG_SPI3 = reinterpret_cast<SPI_Typedef*>(region3 + region3_base - SPI3_BASE);
    REG_SPI4 = reinterpret_cast<SPI_Typedef*>(region3 + region3_base - SPI4_BASE);
    REG_SPI5 = reinterpret_cast<SPI_Typedef*>(region3 + region3_base - SPI5_BASE);
    REG_SPI6 = reinterpret_cast<SPI_Typedef*>(region3 + region3_base - SPI6_BASE);
    REG_BSC0 = reinterpret_cast<BSC_Typedef*>(region3 + region3_base - BSC0_BASE);
    REG_BSC3 = reinterpret_cast<BSC_Typedef*>(region3 + region3_base - BSC3_BASE);
    REG_BSC4 = reinterpret_cast<BSC_Typedef*>(region3 + region3_base - BSC4_BASE);
    REG_BSC5 = reinterpret_cast<BSC_Typedef*>(region3 + region3_base - BSC5_BASE);
    REG_BSC6 = reinterpret_cast<BSC_Typedef*>(region3 + region3_base - BSC6_BASE);

    constexpr static uint32_t region4_base = 0xfe20c000;
    constexpr static uint32_t region4_size = 0x1000;
    uint32_t* region4 = reinterpret_cast<uint32_t*>(mmap(NULL, region4_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region4_base));
    REG_PWM0 = reinterpret_cast<PWM_Typedef*>(region4 + region4_base - PWM0_BASE);
    REG_PWM1 = reinterpret_cast<PWM_Typedef*>(region4 + region4_base - PWM1_BASE);

    constexpr static uint32_t region5_base = 0xfe215000;
    constexpr static uint32_t region5_size = 0x1000;
    uint32_t* region5 = reinterpret_cast<uint32_t*>(mmap(NULL, region5_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region5_base));
    REG_AUX = reinterpret_cast<AUX_Typedef*>(region5 + region5_base - AUX_BASE);

    constexpr static uint32_t region6_base = 0xfe804000;
    constexpr static uint32_t region6_size = 0x1000;
    uint32_t* region6 = reinterpret_cast<uint32_t*>(mmap(NULL, region6_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region6_base));
    REG_BSC1 = reinterpret_cast<BSC_Typedef*>(region6 + region6_base - BSC1_BASE);

    constexpr static uint32_t region7_base = 0xfee05000;
    constexpr static uint32_t region7_size = 0x1000;
    uint32_t* region7 = reinterpret_cast<uint32_t*>(mmap(NULL, region7_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region7_base));
    REG_DMA14 = reinterpret_cast<DMA_Typedef*>(region7 + region7_base - DMA14_BASE);

    system_initialized = true;
	return 0;
}

uint8_t SetGpioFunction(uint8_t pin, GPIO_Function alt){
    uint32_t bit_mode = static_cast<uint32_t>(alt);

    if (0 <= pin && pin <= 9) {
        REG_GPIO->GPFSEL0 &= ~(0b111 << pin*3);
        REG_GPIO->GPFSEL0 |= bit_mode << pin*3;
    }
    else if (10 <= pin && pin <= 19) {
        REG_GPIO->GPFSEL1 &= ~(0b111 << (pin-10)*3);
        REG_GPIO->GPFSEL1 |= bit_mode << (pin-10)*3;
    }
    else if (20 <= pin && pin <= 29) {
        REG_GPIO->GPFSEL2 &= ~(0b111 << (pin-20)*3);
        REG_GPIO->GPFSEL2 |= bit_mode << (pin-20)*3;
    }
    else if (30 <= pin && pin <= 39) {
        REG_GPIO->GPFSEL3 &= ~(0b111 << (pin-30)*3);
        REG_GPIO->GPFSEL3 |= bit_mode << (pin-30)*3;
    }
    else if (40 <= pin && pin <= 49) {
        REG_GPIO->GPFSEL4 &= ~(0b111 << (pin-40)*3);
        REG_GPIO->GPFSEL4 |= bit_mode << (pin-40)*3;
    }
    else if (50 <= pin && pin <= 57) {
        REG_GPIO->GPFSEL5 &= ~(0b111 << (pin-50)*3);
        REG_GPIO->GPFSEL5 |= bit_mode << (pin-50)*3;
    }

	return bit_mode;
}

}