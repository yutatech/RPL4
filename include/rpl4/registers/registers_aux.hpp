#ifndef RPL4_REGISTERS_AUX_HPP_
#define RPL4_REGISTERS_AUX_HPP_

#include <cstdint>

namespace rpl {

const uint32_t kAuxAddressBase = 0xFE215000;
const uint32_t kAuxRegisterSize = 0x00000008;
struct AuxRegisterMap {
  struct Irq {
    Irq(const volatile Irq&);
    volatile Irq& operator=(const volatile Irq&) volatile;
    Irq& operator=(const volatile Irq&);
    volatile Irq& operator=(const Irq&) volatile;

    /**
     * @brief If set the mini UART has an interrupt pending.
     */
    enum class MiniUartIrq : uint32_t {
      kNoInterrupt = 0b0,
      kInterruptPending = 0b1,
    };

    /**
     * @brief If set the SPI1 module has an interrupt pending.
     */
    enum class Spi1Irq : uint32_t {
      kNoInterrupt = 0b0,
      kInterruptPending = 0b1,
    };

    /**
     * @brief If set the SPI2 module has an interrupt pending.
     */
    enum class Spi2Irq : uint32_t {
      kNoInterrupt = 0b0,
      kInterruptPending = 0b1,
    };

    MiniUartIrq mini_uart_irq : 1;
    Spi1Irq spi1_irq : 1;
    Spi2Irq spi2_irq : 1;
  };

  struct Enables {
    Enables(const volatile Enables&);
    volatile Enables& operator=(const volatile Enables&) volatile;
    Enables& operator=(const volatile Enables&);
    volatile Enables& operator=(const Enables&) volatile;

    /**
     * @brief If set the mini UART is enabled.
     */
    enum class MiniUartEnable : uint32_t {
      // the mini UART is disabled. That also disables any mini UART register
      // access
      kDisabled = 0b0,
      // the mini UART is enabled. The UART will immediately start receiving
      // data, especially if the UART1_RX line is low.
      kEnabled = 0b1,
    };

    /**
     * @brief If set the SPI1 module is enabled.
     */
    enum class Spi1Enable : uint32_t {
      // the SPI 1 module is disabled. That also disables any SPI 1 module
      // register access
      kDisabled = 0b0,
      // the SPI 1 module is enabled.
      kEnabled = 0b1,
    };

    /**
     * @brief If set the SPI2 module is enabled.
     */
    enum class Spi2Enable : uint32_t {
      // the SPI 2 module is disabled. That also disables any SPI 2 module
      // register access
      kDisabled = 0b0,
      // the SPI 2 module is enabled.
      kEnabled = 0b1,
    };

    MiniUartEnable mini_uart : 1;
    Spi1Enable spi1 : 1;
    Spi2Enable spi2 : 1;
  };

  volatile Irq irq;          // 0x00
  volatile Enables enables;  // 0x04
};

extern AuxRegisterMap* REG_AUX;

}  // namespace rpl

#endif