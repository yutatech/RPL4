#ifndef RPL4_REGISTERS_DMA_HPP_
#define RPL4_REGISTERS_DMA_HPP_

#include <cstdint>

namespace rpl {

constexpr uint32_t kDma0AddressBase = 0xFE007000;
constexpr uint32_t kDma1AddressBase = 0xFE007100;
constexpr uint32_t kDma2AddressBase = 0xFE007200;
constexpr uint32_t kDma3AddressBase = 0xFE007300;
constexpr uint32_t kDma4AddressBase = 0xFE007400;
constexpr uint32_t kDma5AddressBase = 0xFE007500;
constexpr uint32_t kDma6AddressBase = 0xFE007600;
constexpr uint32_t kDma7AddressBase = 0xFE007700;
constexpr uint32_t kDma8AddressBase = 0xFE007800;
constexpr uint32_t kDma9AddressBase = 0xFE007900;
constexpr uint32_t kDma10AddressBase = 0xFE007A00;
constexpr uint32_t kDma11AddressBase = 0xFE007B00;
constexpr uint32_t kDma12AddressBase = 0xFE007C00;
constexpr uint32_t kDma13AddressBase = 0xFE007D00;
constexpr uint32_t kDma14AddressBase = 0xFEE05000;
constexpr uint32_t kDmaRegisterSize = 0x00000100;
constexpr uint32_t kDmaEnableAddressBase = 0xFE007FF0;

struct DmaRegisterMap {
  /**
   * @brief DMA Control and Status
   */
  struct CS {
    CS(const volatile CS&);
    volatile CS& operator=(const volatile CS&) volatile;
    CS& operator=(const volatile CS&);
    volatile CS& operator=(const CS&) volatile;

    enum class ACTIVE : uint32_t {
      kInactive = 0b0,
      kActive = 0b1,
    };

    enum class END : uint32_t {
      kNotSet = 0b0,
      kSet = 0b1,
    };

    enum class INT : uint32_t {
      kNotSet = 0b0,
      kSet = 0b1,
    };

    enum class DREQ : uint32_t {
      kNotSet = 0b0,
      kSet = 0b1,
    };

    enum class PAUSED : uint32_t {
      kNotPaused = 0b0,
      kPaused = 0b1,
    };

    enum class DREQ_STOPS_DMA : uint32_t {
      kNotPaused = 0b0,
      kPaused = 0b1,
    };

    enum class WAITING_FOR_OUTSTANDING_WRITES : uint32_t {
      kNotWaiting = 0b0,
      kWaiting = 0b1,
    };

    enum class ERROR : uint32_t {
      kNoError = 0b0,
      kError = 0b1,
    };

    enum class PRIORITY : uint32_t {
      kPriority0 = 0x0,
      kPriority1 = 0x1,
      kPriority2 = 0x2,
      kPriority3 = 0x3,
      kPriority4 = 0x4,
      kPriority5 = 0x5,
      kPriority6 = 0x6,
      kPriority7 = 0x7,
      kPriority8 = 0x8,
      kPriority9 = 0x9,
      kPriority10 = 0xA,
      kPriority11 = 0xB,
      kPriority12 = 0xC,
      kPriority13 = 0xD,
      kPriority14 = 0xE,
      kPriority15 = 0xF,
    };

    enum class PANIC_PRIORITY : uint32_t {
      kPriority0 = 0x0,
      kPriority1 = 0x1,
      kPriority2 = 0x2,
      kPriority3 = 0x3,
      kPriority4 = 0x4,
      kPriority5 = 0x5,
      kPriority6 = 0x6,
      kPriority7 = 0x7,
      kPriority8 = 0x8,
      kPriority9 = 0x9,
      kPriority10 = 0xA,
      kPriority11 = 0xB,
      kPriority12 = 0xC,
      kPriority13 = 0xD,
      kPriority14 = 0xE,
      kPriority15 = 0xF,
    };

    enum class WAIT_FOR_OUTSTANDING_WRITES : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    enum class DISDEBUG : uint32_t {
      kEnable = 0b0,
      kDisable = 0b1,
    };

    enum class ABORT : uint32_t {
      kNoEffect = 0b0,
      kAbort = 0b1,
    };

    enum class RESET : uint32_t {
      kNoEffect = 0b0,
      kReset = 0b1,
    };

    ACTIVE active : 1;                                        // bit 0
    END end : 1;                                              // bit 1
    INT interrupt : 1;                                        // bit 2
    DREQ dreq : 1;                                            // bit 3
    PAUSED paused : 1;                                        // bit 4
    DREQ_STOPS_DMA dreq_stops_dma : 1;                        // bit 5
    WAITING_FOR_OUTSTANDING_WRITES waiting_for_outstanding_writes : 1;  // bit 6
    uint32_t reserved1 : 1;                                   // bit 7
    ERROR error : 1;                                          // bit 8
    uint32_t reserved2 : 7;                                   // bits 9-15
    PRIORITY priority : 4;                                    // bits 16-19
    PANIC_PRIORITY panic_priority : 4;                        // bits 20-23
    uint32_t reserved3 : 4;                                   // bits 24-27
    WAIT_FOR_OUTSTANDING_WRITES wait_for_outstanding_writes : 1;  // bit 28
    DISDEBUG disdebug : 1;                                    // bit 29
    ABORT abort : 1;                                          // bit 30
    RESET reset : 1;                                          // bit 31
  };

  /**
   * @brief DMA Control Block Address
   */
  struct CONBLK_AD {
    CONBLK_AD(const volatile CONBLK_AD&);
    volatile CONBLK_AD& operator=(const volatile CONBLK_AD&) volatile;
    CONBLK_AD& operator=(const volatile CONBLK_AD&);
    volatile CONBLK_AD& operator=(const CONBLK_AD&) volatile;

    uint32_t address : 32;  // Control Block Address (must be 32-byte aligned)
  };

  /**
   * @brief DMA Transfer Information
   */
  struct TI {
    TI() = default;
    TI(const volatile TI&);
    volatile TI& operator=(const volatile TI&) volatile;
    TI& operator=(const volatile TI&);
    volatile TI& operator=(const TI&) volatile;

    enum class INTEN : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    enum class TDMODE : uint32_t {
      kLinear = 0b0,
      k2D = 0b1,
    };

    enum class WAIT_RESP : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    enum class DEST_INC : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    enum class DEST_WIDTH : uint32_t {
      k32Bit = 0b0,
      k128Bit = 0b1,
    };

    enum class DEST_DREQ : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    enum class DEST_IGNORE : uint32_t {
      kNormal = 0b0,
      kIgnore = 0b1,
    };

    enum class SRC_INC : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    enum class SRC_WIDTH : uint32_t {
      k32Bit = 0b0,
      k128Bit = 0b1,
    };

    enum class SRC_DREQ : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    enum class SRC_IGNORE : uint32_t {
      kNormal = 0b0,
      kIgnore = 0b1,
    };

    enum class BURST_LENGTH : uint32_t {
      k0 = 0x0,
      k1 = 0x1,
      k2 = 0x2,
      k3 = 0x3,
      k4 = 0x4,
      k5 = 0x5,
      k6 = 0x6,
      k7 = 0x7,
      k8 = 0x8,
      k9 = 0x9,
      k10 = 0xA,
      k11 = 0xB,
      k12 = 0xC,
      k13 = 0xD,
      k14 = 0xE,
      k15 = 0xF,
    };

    enum class PERMAP : uint32_t {
      kContinuous = 0,
      kDsiCore0 = 1,
      kPcmTx = 2,
      kPcmRx = 3,
      kSmi = 4,
      kPwm0 = 5,
      kPwm1 = 6,
      kSpiTx = 7,
      kSpiRx = 8,
      kBscSpiSlaveTx = 9,
      kBscSpiSlaveRx = 10,
      kEmmc = 11,
      kUart0Tx = 12,
      kSdHost = 13,
      kUart0Rx = 14,
      kDsiCore1 = 15,
      kSlimbusMcTx = 16,
      kHdmi = 17,
      kSlimbusMcRx = 18,
      kSlimbusDc0 = 19,
      kSlimbusDc1 = 20,
      kSlimbusDc2 = 21,
      kSlimbusDc3 = 22,
      kSlimbusDc4 = 23,
      kScalerFifo0Smi = 24,
      kScalerFifo1Smi = 25,
      kScalerFifo2Smi = 26,
      kSlimbusDc5 = 27,
      kSlimbusDc6 = 28,
      kSlimbusDc7 = 29,
      kSlimbusDc8 = 30,
      kSlimbusDc9 = 31,
    };

    enum class WAITS : uint32_t {
      k0 = 0x0,
      k1 = 0x1,
      k2 = 0x2,
      k3 = 0x3,
      k4 = 0x4,
      k5 = 0x5,
      k6 = 0x6,
      k7 = 0x7,
      k8 = 0x8,
      k9 = 0x9,
      k10 = 0xA,
      k11 = 0xB,
      k12 = 0xC,
      k13 = 0xD,
      k14 = 0xE,
      k15 = 0xF,
      k16 = 0x10,
      k17 = 0x11,
      k18 = 0x12,
      k19 = 0x13,
      k20 = 0x14,
      k21 = 0x15,
      k22 = 0x16,
      k23 = 0x17,
      k24 = 0x18,
      k25 = 0x19,
      k26 = 0x1A,
      k27 = 0x1B,
      k28 = 0x1C,
      k29 = 0x1D,
      k30 = 0x1E,
      k31 = 0x1F,
    };

    enum class NO_WIDE_BURSTS : uint32_t {
      kEnable = 0b0,
      kDisable = 0b1,
    };

    INTEN inten : 1;            // bit 0
    TDMODE tdmode : 1;          // bit 1
    uint32_t reserved1 : 1;     // bit 2
    WAIT_RESP wait_resp : 1;    // bit 3
    DEST_INC dest_inc : 1;      // bit 4
    DEST_WIDTH dest_width : 1;  // bit 5
    DEST_DREQ dest_dreq : 1;    // bit 6
    DEST_IGNORE dest_ignore : 1;  // bit 7
    SRC_INC src_inc : 1;        // bit 8
    SRC_WIDTH src_width : 1;    // bit 9
    SRC_DREQ src_dreq : 1;      // bit 10
    SRC_IGNORE src_ignore : 1;  // bit 11
    BURST_LENGTH burst_length : 4;  // bits 12-15
    PERMAP permap : 5;          // bits 16-20
    WAITS waits : 5;            // bits 21-25
    NO_WIDE_BURSTS no_wide_bursts : 1;  // bit 26
    uint32_t reserved2 : 5;     // bits 27-31
  };

  /**
   * @brief DMA Source Address
   */
  struct SOURCE_AD {
    SOURCE_AD(const volatile SOURCE_AD&);
    volatile SOURCE_AD& operator=(const volatile SOURCE_AD&) volatile;
    SOURCE_AD& operator=(const volatile SOURCE_AD&);
    volatile SOURCE_AD& operator=(const SOURCE_AD&) volatile;

    uint32_t address : 32;  // Source Address (physical)
  };

  /**
   * @brief DMA Destination Address
   */
  struct DEST_AD {
    DEST_AD(const volatile DEST_AD&);
    volatile DEST_AD& operator=(const volatile DEST_AD&) volatile;
    DEST_AD& operator=(const volatile DEST_AD&);
    volatile DEST_AD& operator=(const DEST_AD&) volatile;

    uint32_t address : 32;  // Destination Address (physical)
  };

  /**
   * @brief DMA Transfer Length
   */
  struct TXFR_LEN {
    TXFR_LEN(const volatile TXFR_LEN&);
    volatile TXFR_LEN& operator=(const volatile TXFR_LEN&) volatile;
    TXFR_LEN& operator=(const volatile TXFR_LEN&);
    volatile TXFR_LEN& operator=(const TXFR_LEN&) volatile;

    uint32_t xlength : 16;  // Transfer Length in bytes (linear mode)
    uint32_t ylength : 14;  // Number of transfers in y direction (2D mode)
    uint32_t reserved : 2;
  };

  /**
   * @brief DMA 2D Stride
   */
  struct STRIDE {
    STRIDE(const volatile STRIDE&);
    volatile STRIDE& operator=(const volatile STRIDE&) volatile;
    STRIDE& operator=(const volatile STRIDE&);
    volatile STRIDE& operator=(const STRIDE&) volatile;

    uint32_t s_stride : 16;  // Source Stride (signed)
    uint32_t d_stride : 16;  // Destination Stride (signed)
  };

  /**
   * @brief DMA Next Control Block Address
   */
  struct NEXTCONBK {
    NEXTCONBK(const volatile NEXTCONBK&);
    volatile NEXTCONBK& operator=(const volatile NEXTCONBK&) volatile;
    NEXTCONBK& operator=(const volatile NEXTCONBK&);
    volatile NEXTCONBK& operator=(const NEXTCONBK&) volatile;

    uint32_t address : 32;  // Next Control Block Address (physical, 32-byte aligned)
  };

  /**
   * @brief DMA Debug
   */
  struct DEBUG {
    DEBUG(const volatile DEBUG&);
    volatile DEBUG& operator=(const volatile DEBUG&) volatile;
    DEBUG& operator=(const volatile DEBUG&);
    volatile DEBUG& operator=(const DEBUG&) volatile;

    enum class READ_LAST_NOT_SET_ERROR : uint32_t {
      kNoError = 0b0,
      kError = 0b1,
    };

    enum class FIFO_ERROR : uint32_t {
      kNoError = 0b0,
      kError = 0b1,
    };

    enum class READ_ERROR : uint32_t {
      kNoError = 0b0,
      kError = 0b1,
    };

    uint32_t read_last_not_set_error : 1;  // bit 0
    FIFO_ERROR fifo_error : 1;              // bit 1
    READ_ERROR read_error : 1;              // bit 2
    uint32_t reserved1 : 1;                 // bit 3
    uint32_t outstanding_writes : 4;        // bits 4-7
    uint32_t dma_id : 8;                    // bits 8-15
    uint32_t dma_state : 9;                 // bits 16-24
    uint32_t version : 3;                   // bits 25-27
    uint32_t lite : 1;                      // bit 28
    uint32_t reserved2 : 3;                 // bits 29-31
  };

  volatile CS cs;                // 0x00
  volatile CONBLK_AD conblk_ad;  // 0x04
  volatile TI ti;                // 0x08
  volatile SOURCE_AD source_ad;  // 0x0C
  volatile DEST_AD dest_ad;      // 0x10
  volatile TXFR_LEN txfr_len;    // 0x14
  volatile STRIDE stride;        // 0x18
  volatile NEXTCONBK nextconbk;  // 0x1C
  volatile DEBUG debug;          // 0x20
  volatile uint32_t reserved[55];  // 0x24-0xFF
};

struct DmaEnableRegisterMap {
  volatile uint32_t enable;  // 0xFF0: DMA Enable register
};

extern DmaRegisterMap* REG_DMA0;
extern DmaRegisterMap* REG_DMA1;
extern DmaRegisterMap* REG_DMA2;
extern DmaRegisterMap* REG_DMA3;
extern DmaRegisterMap* REG_DMA4;
extern DmaRegisterMap* REG_DMA5;
extern DmaRegisterMap* REG_DMA6;
extern DmaRegisterMap* REG_DMA7;
extern DmaRegisterMap* REG_DMA8;
extern DmaRegisterMap* REG_DMA9;
extern DmaRegisterMap* REG_DMA10;
extern DmaRegisterMap* REG_DMA11;
extern DmaRegisterMap* REG_DMA12;
extern DmaRegisterMap* REG_DMA13;
extern DmaRegisterMap* REG_DMA14;
extern DmaEnableRegisterMap* REG_DMA_ENABLE;

// DMA Control Block structure (must be 32-byte aligned in physical memory)
struct DmaControlBlock {
  volatile DmaRegisterMap::TI transfer_info; // Transfer Information
  volatile uint32_t source_addr;             // Source Address (physical)
  volatile uint32_t dest_addr;               // Destination Address (physical)
  volatile uint32_t transfer_length;         // Transfer Length
  volatile uint32_t stride;                  // 2D Mode Stride
  volatile uint32_t next_control_block;      // Next Control Block Address (physical)
  volatile uint32_t reserved[2];             // Reserved
};

}  // namespace rpl

#endif  // RPL4_REGISTERS_DMA_HPP_