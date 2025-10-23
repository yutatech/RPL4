#include "rpl4/system/system.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <sys/stat.h>

#include <fstream>

namespace rpl {

bool system_initialized = false;

AuxRegisterMap*  REG_AUX;
BSC_Typedef*  REG_BSC0;
BSC_Typedef*  REG_BSC1;
BSC_Typedef*  REG_BSC3;
BSC_Typedef*  REG_BSC4;
BSC_Typedef*  REG_BSC5;
BSC_Typedef*  REG_BSC6;
ClockRegisterMap*  REG_CLK;
DmaRegisterMap*  REG_DMA0;
DmaRegisterMap*  REG_DMA1;
DmaRegisterMap*  REG_DMA2;
DmaRegisterMap*  REG_DMA3;
DmaRegisterMap*  REG_DMA4;
DmaRegisterMap*  REG_DMA5;
DmaRegisterMap*  REG_DMA6;
DmaRegisterMap*  REG_DMA7;
DmaRegisterMap*  REG_DMA8;
DmaRegisterMap*  REG_DMA9;
DmaRegisterMap*  REG_DMA10;
DmaRegisterMap*  REG_DMA11;
DmaRegisterMap*  REG_DMA12;
DmaRegisterMap*  REG_DMA13;
DmaRegisterMap*  REG_DMA14;
DmaEnableRegisterMap* REG_DMA_ENABLE;
GpioRegisterMap* REG_GPIO;
PwmRegisterMap*  REG_PWM0;
PwmRegisterMap*  REG_PWM1;
SpiRegisterMap*  REG_SPI0;
AuxSpiRegisterMap*  REG_SPI1;
AuxSpiRegisterMap*  REG_SPI2;
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
    std::ifstream file("/proc/cpuinfo");
    if (!file) {
      Log(LogLevel::Warning,
          "Failed to open /proc/cpuinfo. "
          "Cannot determine if this is a Raspberry Pi system.");
      return false;
    }
  
    std::string line;
    while (std::getline(file, line)) {
      if (line.find("Hardware") != std::string::npos &&
          line.find("BCM2835") != std::string::npos) {
        return true;
      }
    }
    Log(LogLevel::Warning,
        "This system does not appear to be a Raspberry Pi. "
        "RPL4 may not work as expected.");
    return false;
}

bool IsInitialized(void){
    return system_initialized;
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
    REG_DMA0 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma0AddressBase - region0_base) / 4);
    REG_DMA1 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma1AddressBase - region0_base) / 4);
    REG_DMA2 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma2AddressBase - region0_base) / 4);
    REG_DMA3 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma3AddressBase - region0_base) / 4);
    REG_DMA4 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma4AddressBase - region0_base) / 4);
    REG_DMA5 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma5AddressBase - region0_base) / 4);
    REG_DMA6 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma6AddressBase - region0_base) / 4);
    REG_DMA7 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma7AddressBase - region0_base) / 4);
    REG_DMA8 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma8AddressBase - region0_base) / 4);
    REG_DMA9 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma9AddressBase - region0_base) / 4);
    REG_DMA10 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma10AddressBase - region0_base) / 4);
    REG_DMA11 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma11AddressBase - region0_base) / 4);
    REG_DMA12 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma12AddressBase - region0_base) / 4);
    REG_DMA13 = reinterpret_cast<DmaRegisterMap*>(region0 + (kDma13AddressBase - region0_base) / 4);
    REG_DMA_ENABLE = reinterpret_cast<DmaEnableRegisterMap*>(region0 + (kDmaEnableAddressBase - region0_base) / 4);

    constexpr static uint32_t region1_base = 0xfe101000;
    constexpr static uint32_t region1_size = 0x1000;
    uint32_t* region1 = reinterpret_cast<uint32_t*>(mmap(NULL, region1_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region1_base));
    REG_CLK = reinterpret_cast<ClockRegisterMap*>(region1 + (kClockAddressBase - region1_base) / 4);

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
    REG_PWM0 = reinterpret_cast<PwmRegisterMap*>(region4 + (kPwm0AddressBase - region4_base) / 4);
    REG_PWM1 = reinterpret_cast<PwmRegisterMap*>(region4 + (kPwm1AddressBase - region4_base) / 4);

    constexpr static uint32_t region5_base = 0xfe215000;
    constexpr static uint32_t region5_size = 0x1000;
    uint32_t* region5 = reinterpret_cast<uint32_t*>(mmap(NULL, region5_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region5_base));
    REG_AUX = reinterpret_cast<AuxRegisterMap*>(region5 + (kAuxAddressBase - region5_base) / 4);
    REG_SPI1 = reinterpret_cast<AuxSpiRegisterMap*>(region5 + (kSpi1AddressBase - region5_base) / 4);
    REG_SPI2 = reinterpret_cast<AuxSpiRegisterMap*>(region5 + (kSpi2AddressBase - region5_base) / 4);

    constexpr static uint32_t region6_base = 0xfe804000;
    constexpr static uint32_t region6_size = 0x1000;
    uint32_t* region6 = reinterpret_cast<uint32_t*>(mmap(NULL, region6_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region6_base));
    REG_BSC1 = reinterpret_cast<BSC_Typedef*>(region6 + (BSC1_BASE - region6_base) / 4);

    constexpr static uint32_t region7_base = 0xfee05000;
    constexpr static uint32_t region7_size = 0x1000;
    uint32_t* region7 = reinterpret_cast<uint32_t*>(mmap(NULL, region7_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region7_base));
    REG_DMA14 = reinterpret_cast<DmaRegisterMap*>(region7 + (kDma14AddressBase - region7_base) / 4);

    system_initialized = true;
	return 0;
}

}