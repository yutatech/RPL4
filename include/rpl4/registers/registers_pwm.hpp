#ifndef RPL4_REGISTERS_PWM_HPP_
#define RPL4_REGISTERS_PWM_HPP_

#include <cstdint>

namespace rpl {

constexpr uint32_t kPwm0AddressBase = 0xFE20C000;
constexpr uint32_t kPwm1AddressBase = 0xFE20C800;
constexpr uint32_t kPwmRegisterSize = 0x00000028;

struct PwmRegisterMap {
  /**
   * @brief PWM Control
   */
  struct CTL {
    CTL(const volatile CTL&);
    volatile CTL& operator=(const volatile CTL&) volatile;
    CTL& operator=(const volatile CTL&);
    volatile CTL& operator=(const CTL&) volatile;

    enum class PWEN : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    enum class MODE : uint32_t {
      kPwmMode = 0b0,
      kSerializerMode = 0b1,
    };

    enum class RPTL : uint32_t {
      kTransmit = 0b0,
      kRepeatLast = 0b1,
    };

    enum class SBIT : uint32_t {
      kLow = 0b0,
      kHigh = 0b1,
    };

    enum class POLA : uint32_t {
      kNormal = 0b0,
      kInverted = 0b1,
    };

    enum class USEF : uint32_t {
      kData = 0b0,
      kFifo = 0b1,
    };

    enum class CLRF : uint32_t {
      kNoEffect = 0b0,
      kClear = 0b1,
    };

    enum class MSEN : uint32_t {
      kPwmAlgorithm = 0b0,
      kMSRatio = 0b1,
    };

    PWEN pwen1 : 1;   // bit 0: Channel 1 Enable
    MODE mode1 : 1;   // bit 1: Channel 1 Mode
    RPTL rptl1 : 1;   // bit 2: Channel 1 Repeat Last Data
    SBIT sbit1 : 1;   // bit 3: Channel 1 Silence Bit
    POLA pola1 : 1;   // bit 4: Channel 1 Polarity
    USEF usef1 : 1;   // bit 5: Channel 1 Use Fifo
    CLRF clrf1 : 1;   // bit 6: Clear Fifo
    MSEN msen1 : 1;   // bit 7: Channel 1 M/S Enable
    PWEN pwen2 : 1;   // bit 8: Channel 2 Enable
    MODE mode2 : 1;   // bit 9: Channel 2 Mode
    RPTL rptl2 : 1;   // bit 10: Channel 2 Repeat Last Data
    SBIT sbit2 : 1;   // bit 11: Channel 2 Silence Bit
    POLA pola2 : 1;   // bit 12: Channel 2 Polarity
    USEF usef2 : 1;   // bit 13: Channel 2 Use Fifo
    uint32_t reserved1 : 1;  // bit 14
    MSEN msen2 : 1;   // bit 15: Channel 2 M/S Enable
    uint32_t reserved2 : 16;  // bits 16-31
  };

  /**
   * @brief PWM Status
   */
  struct STA {
    STA(const volatile STA&);
    volatile STA& operator=(const volatile STA&) volatile;
    STA& operator=(const volatile STA&);
    volatile STA& operator=(const STA&) volatile;

    enum class FULL : uint32_t {
      kNotFull = 0b0,
      kFull = 0b1,
    };

    enum class EMPT : uint32_t {
      kNotEmpty = 0b0,
      kEmpty = 0b1,
    };

    enum class WERR : uint32_t {
      kNoError = 0b0,
      kError = 0b1,
    };

    enum class RERR : uint32_t {
      kNoError = 0b0,
      kError = 0b1,
    };

    enum class GAPO : uint32_t {
      kNoGap = 0b0,
      kGapOccurred = 0b1,
    };

    enum class BERR : uint32_t {
      kNoError = 0b0,
      kError = 0b1,
    };

    enum class STATE : uint32_t {
      kNotTransmitting = 0b0,
      kTransmitting = 0b1,
    };

    FULL full1 : 1;   // bit 0: Fifo Full Flag
    EMPT empt1 : 1;   // bit 1: Fifo Empty Flag
    WERR werr1 : 1;   // bit 2: Fifo Write Error Flag
    RERR rerr1 : 1;   // bit 3: Fifo Read Error Flag
    GAPO gapo1 : 1;   // bit 4: Channel 1 Gap Occurred Flag
    GAPO gapo2 : 1;   // bit 5: Channel 2 Gap Occurred Flag
    GAPO gapo3 : 1;   // bit 6: Channel 3 Gap Occurred Flag
    GAPO gapo4 : 1;   // bit 7: Channel 4 Gap Occurred Flag
    BERR berr : 1;    // bit 8: Bus Error Flag
    STATE sta1 : 1;   // bit 9: Channel 1 State
    STATE sta2 : 1;   // bit 10: Channel 2 State
    STATE sta3 : 1;   // bit 11: Channel 3 State
    STATE sta4 : 1;   // bit 12: Channel 4 State
    uint32_t reserved : 19;  // bits 13-31
  };

  /**
   * @brief PWM DMA Configuration
   */
  struct DMAC {
    DMAC(const volatile DMAC&);
    volatile DMAC& operator=(const volatile DMAC&) volatile;
    DMAC& operator=(const volatile DMAC&);
    volatile DMAC& operator=(const DMAC&) volatile;

    enum class DREQ : uint32_t {
      k1Word = 0x1,
      k2Words = 0x2,
      k3Words = 0x3,
      k4Words = 0x4,
      k5Words = 0x5,
      k6Words = 0x6,
      k7Words = 0x7,
      k8Words = 0x8,
      k9Words = 0x9,
      k10Words = 0xA,
      k11Words = 0xB,
      k12Words = 0xC,
      k13Words = 0xD,
      k14Words = 0xE,
      k15Words = 0xF,
    };

    enum class PANIC : uint32_t {
      k1Word = 0x1,
      k2Words = 0x2,
      k3Words = 0x3,
      k4Words = 0x4,
      k5Words = 0x5,
      k6Words = 0x6,
      k7Words = 0x7,
      k8Words = 0x8,
      k9Words = 0x9,
      k10Words = 0xA,
      k11Words = 0xB,
      k12Words = 0xC,
      k13Words = 0xD,
      k14Words = 0xE,
      k15Words = 0xF,
    };

    enum class ENAB : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    DREQ dreq : 8;    // bits 0-7: DMA Threshold for DREQ signal
    PANIC panic : 8;  // bits 8-15: DMA Threshold for PANIC signal
    uint32_t reserved : 15;  // bits 16-30
    ENAB enab : 1;    // bit 31: DMA Enable
  };

  /**
   * @brief PWM Channel Range
   */
  struct RNG {
    RNG(const volatile RNG&);
    volatile RNG& operator=(const volatile RNG&) volatile;
    RNG& operator=(const volatile RNG&);
    volatile RNG& operator=(const RNG&) volatile;

    uint32_t range : 32;  // PWM Channel Range (period)
  };

  /**
   * @brief PWM Channel Data
   */
  struct DAT {
    DAT(const volatile DAT&);
    volatile DAT& operator=(const volatile DAT&) volatile;
    DAT& operator=(const volatile DAT&);
    volatile DAT& operator=(const DAT&) volatile;

    uint32_t data : 32;  // PWM Channel Data (pulse width)
  };

  /**
   * @brief PWM FIFO Input
   */
  struct FIF {
    FIF(const volatile FIF&);
    volatile FIF& operator=(const volatile FIF&) volatile;
    FIF& operator=(const volatile FIF&);
    volatile FIF& operator=(const FIF&) volatile;

    uint32_t data : 32;  // PWM FIFO Input
  };

  volatile CTL ctl;            // 0x00
  volatile STA sta;            // 0x04
  volatile DMAC dmac;          // 0x08
  volatile uint32_t reserved1; // 0x0C
  volatile RNG rng1;           // 0x10
  volatile DAT dat1;           // 0x14
  volatile FIF fif1;           // 0x18
  volatile uint32_t reserved2; // 0x1C
  volatile RNG rng2;           // 0x20
  volatile DAT dat2;           // 0x24
};

extern PwmRegisterMap* REG_PWM0;
extern PwmRegisterMap* REG_PWM1;

}  // namespace rpl

#endif  // RPL4_REGISTERS_PWM_HPP_