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
GpioRegisterMap* REG_GPIO;
PWM_Typedef*  REG_PWM0;
PWM_Typedef*  REG_PWM1;
SpiRegisterMap*  REG_SPI0;
SPI_AUX_Typedef*  REG_SPI1;
SPI_AUX_Typedef*  REG_SPI2;
SpiRegisterMap*  REG_SPI3;
SpiRegisterMap*  REG_SPI4;
SpiRegisterMap*  REG_SPI5;
SpiRegisterMap*  REG_SPI6;
UART_Typedef*  REG_UART0;
UART_Typedef*  REG_UART2;
UART_Typedef*  REG_UART3;
UART_Typedef*  REG_UART4;
UART_Typedef*  REG_UART5;
UART_AUX_Typedef*  REG_UART1;

bool IsAvailable(void){
    struct stat buffer;
    if (stat("/dev/mem", &buffer) == 0) {
        return true;
    } else {
        Log(LogLevel::Warning, "/dev/mem does not exist. "
            "This system may not be Raspberry Pi.");
        return false;
    }
}

uint8_t Init(void){
	int fd;
	if ((fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0) {
        Log(LogLevel::Fatal, "Can't open /dev/mem. Root privileges required.");
        return -1;
    }

    constexpr static uint32_t region0_base = 0xfe007000;
    constexpr static uint32_t region0_size = 0x1000;
    uint32_t* region0 = reinterpret_cast<uint32_t*>(mmap(NULL, region0_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region0_base));
    REG_DMA0 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA0_BASE - region0_base) / 4);
    REG_DMA1 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA1_BASE - region0_base) / 4);
    REG_DMA2 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA2_BASE - region0_base) / 4);
    REG_DMA3 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA3_BASE - region0_base) / 4);
    REG_DMA4 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA4_BASE - region0_base) / 4);
    REG_DMA5 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA5_BASE - region0_base) / 4);
    REG_DMA6 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA6_BASE - region0_base) / 4);
    REG_DMA7 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA7_BASE - region0_base) / 4);
    REG_DMA8 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA8_BASE - region0_base) / 4);
    REG_DMA9 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA9_BASE - region0_base) / 4);
    REG_DMA10 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA10_BASE - region0_base) / 4);
    REG_DMA11 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA11_BASE - region0_base) / 4);
    REG_DMA12 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA12_BASE - region0_base) / 4);
    REG_DMA13 = reinterpret_cast<DMA_Typedef*>(region0 + (DMA13_BASE - region0_base) / 4);

    constexpr static uint32_t region1_base = 0xfe101000;
    constexpr static uint32_t region1_size = 0x1000;
    uint32_t* region1 = reinterpret_cast<uint32_t*>(mmap(NULL, region1_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region1_base));
    REG_CLK = reinterpret_cast<CLK_Typedef*>(region1 + (CLK_BASE - region1_base) / 4);

    constexpr static uint32_t region2_base = 0xfe200000;
    constexpr static uint32_t region2_size = 0x2000;
    uint32_t* region2 = reinterpret_cast<uint32_t*>(mmap(NULL, region2_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region2_base));
    REG_GPIO = reinterpret_cast<GpioRegisterMap*>(region2 + (kGpioAddressBase - region2_base) / 4);
    REG_UART0 = reinterpret_cast<UART_Typedef*>(region2 + (UART0_BASE - region2_base) / 4);
    REG_UART2 = reinterpret_cast<UART_Typedef*>(region2 + (UART2_BASE - region2_base) / 4);
    REG_UART3 = reinterpret_cast<UART_Typedef*>(region2 + (UART3_BASE - region2_base) / 4);
    REG_UART4 = reinterpret_cast<UART_Typedef*>(region2 + (UART4_BASE - region2_base) / 4);
    REG_UART5 = reinterpret_cast<UART_Typedef*>(region2 + (UART5_BASE - region2_base) / 4);

    constexpr static uint32_t region3_base = 0xfe204000;
    constexpr static uint32_t region3_size = 0x2000;
    uint32_t* region3 = reinterpret_cast<uint32_t*>(mmap(NULL, region3_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region3_base));
    REG_SPI0 = reinterpret_cast<SpiRegisterMap*>(region3 + (kSpi0AddressBase - region3_base) / 4);
    REG_SPI3 = reinterpret_cast<SpiRegisterMap*>(region3 + (kSpi3AddressBase - region3_base) / 4);
    REG_SPI4 = reinterpret_cast<SpiRegisterMap*>(region3 + (kSpi4AddressBase - region3_base) / 4);
    REG_SPI5 = reinterpret_cast<SpiRegisterMap*>(region3 + (kSpi5AddressBase - region3_base) / 4);
    REG_SPI6 = reinterpret_cast<SpiRegisterMap*>(region3 + (kSpi6AddressBase - region3_base) / 4);
    REG_BSC0 = reinterpret_cast<BSC_Typedef*>(region3 + (BSC0_BASE - region3_base) / 4);
    REG_BSC3 = reinterpret_cast<BSC_Typedef*>(region3 + (BSC3_BASE - region3_base) / 4);
    REG_BSC4 = reinterpret_cast<BSC_Typedef*>(region3 + (BSC4_BASE - region3_base) / 4);
    REG_BSC5 = reinterpret_cast<BSC_Typedef*>(region3 + (BSC5_BASE - region3_base) / 4);
    REG_BSC6 = reinterpret_cast<BSC_Typedef*>(region3 + (BSC6_BASE - region3_base) / 4);

    constexpr static uint32_t region4_base = 0xfe20c000;
    constexpr static uint32_t region4_size = 0x1000;
    uint32_t* region4 = reinterpret_cast<uint32_t*>(mmap(NULL, region4_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region4_base));
    REG_PWM0 = reinterpret_cast<PWM_Typedef*>(region4 + (PWM0_BASE - region4_base) / 4);
    REG_PWM1 = reinterpret_cast<PWM_Typedef*>(region4 + (PWM1_BASE - region4_base) / 4);

    constexpr static uint32_t region5_base = 0xfe215000;
    constexpr static uint32_t region5_size = 0x1000;
    uint32_t* region5 = reinterpret_cast<uint32_t*>(mmap(NULL, region5_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region5_base));
    REG_AUX = reinterpret_cast<AUX_Typedef*>(region5 + (AUX_BASE - region5_base) / 4);

    constexpr static uint32_t region6_base = 0xfe804000;
    constexpr static uint32_t region6_size = 0x1000;
    uint32_t* region6 = reinterpret_cast<uint32_t*>(mmap(NULL, region6_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region6_base));
    REG_BSC1 = reinterpret_cast<BSC_Typedef*>(region6 + (BSC1_BASE - region6_base) / 4);

    constexpr static uint32_t region7_base = 0xfee05000;
    constexpr static uint32_t region7_size = 0x1000;
    uint32_t* region7 = reinterpret_cast<uint32_t*>(mmap(NULL, region7_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region7_base));
    REG_DMA14 = reinterpret_cast<DMA_Typedef*>(region7 + (DMA14_BASE - region7_base) / 4);

    system_initialized = true;
	return 0;
}

}