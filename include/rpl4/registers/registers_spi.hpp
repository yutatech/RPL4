#ifndef RPL4_REGISTERS_SPI_HPP_
#define RPL4_REGISTERS_SPI_HPP_

#include <cstdint>

namespace rpl {

constexpr uint32_t kSpi0AddressBase = 0xFE204000;
constexpr uint32_t kSpi3AddressBase = 0xFE204600;
constexpr uint32_t kSpi4AddressBase = 0xFE204800;
constexpr uint32_t kSpi5AddressBase = 0xFE204A00;
constexpr uint32_t kSpi6AddressBase = 0xFE204C00;
constexpr uint32_t kSpiRegisterSize = 0x00000018;

struct SpiRegisterMap {
  /**
   * @brief SPI Master Control and Status
   * @note This register contains the main control and status bits for the SPI.
   */
  struct CS {
    CS(const volatile CS&);
    volatile CS& operator=(const volatile CS&) volatile;
    CS& operator=(const volatile CS&);
    volatile CS& operator=(const CS&) volatile;

    /**
     * @brief Chip Select
     */
    enum class CS_ : uint32_t {
      kChipSelect0 = 0b00,  // Chip select 0
      kChipSelect1 = 0b01,  // Chip select 1
      kChipSelect2 = 0b10,  // Chip select 2
      kDisable = 0b11,      // Do not automatically perform chip select
    };

    /**
     * @brief Clock Phase
     */
    enum class CPHA : uint32_t {
      kMiddle = 0b0,     // First SCLK transition at middle of data bit.
      kBeginning = 0b1,  // First SCLK transition at middle of data bit.
    };

    /**
     * @brief Clock Polarity
     */
    enum class CPOL : uint32_t {
      kLow = 0b0,   // Rest state of clock = low.
      kHigh = 0b1,  // Rest state of clock = high.
    };

    /**
     * @brief FIFO Clear
     * @note If CLEAR and TA are both set in the same operation, the FIFOs are
     *       cleared before the new frame is started. Read back as 0.
     */
    enum class CLEAR : uint32_t {
      kNoAction = 0b00,       // No action
      kClearTxFifo = 0b01,    // Clear TX FIFO. One-shot operation
      kClearRxFifo = 0b10,    // Clear RX FIFO. One-shot operation
      kClearBothFifo = 0b11,  // Clear TX and RX FIFO. One-shot operation
    };

    /**
     * @brief Chip Select Polarity
     */
    enum class CSPOL : uint32_t {
      kLow = 0b0,   // Chip select lines are active low
      kHigh = 0b1,  // Chip select lines are active high
    };

    /**
     * @brief Transfer Active
     */
    enum class TA : uint32_t {
      // Transfer not active. /CS lines are all high (assuming CSPOL = 0). RXR
      // and DONE are 0. Writes to SPI_FIFO write data into bits 15:0 of SPIDLEN
      // and bits 7:0 of SPICS allowing DMA data blocks to set mode before
      // sending data.
      kInactive = 0b0,

      // Transfer active. /CS lines are set according to CS bits and CSPOL.
      // Writes to SPI_FIFO write data to TX FIFO. TA is cleared by a
      // dma_frame_end pulse from the DMA controller.
      kActive = 0b1,

    };

    /**
     * @brief DMA Enable
     * @note Peripheral generates data requests. These will be taken in
     *       four-byte words until the SPIDLEN has been reached.
     */
    enum class DMAEN : uint32_t {
      kDisable = 0b0,  // No DMA requests will be issued.
      kEnable = 0b1,   // Enable DMA operation.
    };

    /**
     * @brief Interrupt on Done
     */
    enum class INTD : uint32_t {
      kDisable = 0b0,  // Don’t generate interrupt on transfer complete.
      kEnable = 0b1,   // Generate interrupt when DONE = 1.
    };

    /**
     * @brief Interrupt on RXR
     */
    enum class INTR : uint32_t {
      kDisable = 0b0,  // Don’t generate interrupts on RX FIFO condition.
      kEnable = 0b1,   // Generate interrupt while RXR = 1.
    };

    /**
     * @brief Automatically De-assert Chip Select
     */
    enum class ADCS : uint32_t {
      // Don’t automatically de-assert chip select at the end of a DMA transfer;
      // chip select is manually controlled by software.
      kDisable = 0b0,

      // Automatically de-assert chip select at the end of a DMAtransfer (as
      // determined by SPIDLEN)
      kEnable = 0b1,
    };

    /**
     * @brief Read Enable
     * @note Read enable if you are using bidirectional mode. If this bit is
     *       set, the SPI peripheral will be able to send data to this device.
     */
    enum class REN : uint32_t {
      kDisable = 0b0,  // We intend to write to the SPI peripheral.
      kEnable = 0b1,   // We intend to read from the SPI peripheral.
    };

    /**
     * @brief LoSSI enable
     * @note The serial interface is configured as a LoSSI master.
     */
    enum class LEN : uint32_t {
      kDisable = 0b0,  // The serial interface will behave as an SPI master.
      kEnable = 0b1,   // The serial interface will behave as a LoSSI master.
    };

    /**
     * @brief Transfer Done
     */
    enum class DONE : uint32_t {
      // Transfer is in progress (or not active TA = 0).
      kInProgress = 0b0,

      // Transfer is complete. Cleared by writing more datato the TX FIFO or
      // setting TA to 0.
      kCompleted = 0b1,
    };

    /**
     * @brief RX FIFO contains Data
     */
    enum class RXD : uint32_t {
      kEmpty = 0b0,     // RX FIFO is empty.
      kContains = 0b1,  // RX FIFO contains at least 1 byte.
    };

    /**
     * @brief TX FIFO can accept Data
     */
    enum class TXD : uint32_t {
      kFull = 0b0,      // TX FIFO is full and so cannot accept more data.
      kWritable = 0b1,  // TX FIFO has space for at least 1 byte.
    };

    /**
     * @brief RX FIFO needs Reading (3⁄4 full)
     */
    enum class RXR : uint32_t {
      // RX FIFO is less than 3⁄4 full (or not active TA = 0).
      kStillAvailable = 0b0,

      // RX FIFO is 3⁄4 or more full. Cleared by reading sufficientdata from the
      // RX FIFO or setting TA to 0.
      kNearlyFull = 0b1,
    };

    /**
     * @brief RX FIFO Full
     */
    enum class RXF : uint32_t {
      // RX FIFO is not full.
      kNotFull = 0b0,

      // RX FIFO is full. No further serial data will be sent /received until
      // data is read from FIFO.
      kFull = 0b1,
    };

    /**
     * @brief Enable DMA mode in LoSSI mode
     */
    enum class DMA_LEN : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    /**
     * @brief Enable Long data word in LoSSI mode if DMA_LEN is set
     */
    enum class LEN_LONG : uint32_t {
      kSingleByte = 0b0,  // writing to the FIFO will write a single byte
      kFourBytes = 0b1,   // writing to the FIFO will write a 32-bit word
    };

    CS_ cs : 2;            // Chip Select
    CPHA cpha : 1;         // Clock Phase
    CPOL cpol : 1;         // Clock Polarity
    CLEAR clear : 2;       // FIFO Clear
    CSPOL cspol : 1;       // Chip Select Polarity
    TA ta : 1;             // Transfer Active
    DMAEN dmaen : 1;       // DMA Enable
    INTD intd : 1;         // Interrupt on Done
    INTR intr : 1;         // Interrupt on RXR
    ADCS adcs : 1;         // Automatically De-assert Chip Select
    REN ren : 1;           // Read Enable
    LEN len : 1;           // LoSSI enable
    uint32_t lmono : 1;    // unused
    uint32_t te_en : 1;    // unused
    DONE done : 1;         // Transfer Done
    RXD rxd : 1;           // RX FIFO contains Data
    TXD txd : 1;           // TX FIFO can accept Data
    RXR rxr : 1;           // RX FIFO needs Reading (3⁄4 full)
    RXF rxf : 1;           // RX FIFO Full
    CSPOL cspol0 : 1;      // Chip Select 0 Polarity
    CSPOL cspol1 : 1;      // Chip Select 1 Polarity
    CSPOL cspol2 : 1;      // Chip Select 2 Polarity
    uint32_t dma_len : 1;  // Enable DMA mode in LoSSI mode
    uint32_t
        len_long : 1;  // Enable Long data word in LoSSI mode if DMA_LEN is set
  };

  /**
   * @brief SPI Master TX and RX FIFOs
   * @note This register allows TX data to be written to the TX FIFO and RX data
   *       to be read from the RX FIFO.
   */
  struct FIFO {
    FIFO(const volatile FIFO&);
    volatile FIFO& operator=(const volatile FIFO&) volatile;
    FIFO& operator=(const volatile FIFO&);
    volatile FIFO& operator=(const FIFO&) volatile;

    /**
     * @brief DMA Mode (DMAEN set)
     * @note If TA is clear, the first 32-bit write to this register will
     *       control SPIDLEN and SPICS. Subsequent reads and writes will be
     *       taken as four-byte data words to be read/written to the FIFOs\n
     *       Poll/Interrupt Mode (DMAEN clear, TA set)\n
     *       Writes to the register write bytes to TX FIFO. Reads from register
     *       read bytes from the RX FIFO.
     */
    uint32_t data : 32;
  };

  /**
   * @brief SPI Master Clock Divider
   * @note This register allows the SPI clock rate to be set.
   */
  struct CLK {
    CLK(const volatile CLK&);
    volatile CLK& operator=(const volatile CLK&) volatile;
    CLK& operator=(const volatile CLK&);
    volatile CLK& operator=(const CLK&) volatile;

    /**
     * @brief Clock Divider
     * @note SCLK = Core Clock / CDIV\n
     *       If CDIV is set to 0, the divisor is 65536. The divisor must be a
     *       multiple of 2. Odd numbers rounded down. The maximum SPI clock rate
     *       is of the APB clock.
     */
    uint32_t cdiv : 16;
  };

  /**
   * @brief SPI Master Data Length
   * @note This register allows the SPI data length rate to be set.
   */
  struct DLEN {
    DLEN(const volatile DLEN&);
    volatile DLEN& operator=(const volatile DLEN&) volatile;
    DLEN& operator=(const volatile DLEN&);
    volatile DLEN& operator=(const DLEN&) volatile;

    /**
     * @brief Data Length
     * @note The number of bytes to transfer.
     *       This field is only valid for DMA mode (DMAEN set) and controls how
     *       many bytes to transmit (and therefore receive).
     */
    uint32_t len : 16;
  };

  /**
   * @brief SPI LoSSI mode TOH
   * @note This register allows the LoSSI output hold delay to be set.
   */
  struct LTOH {
    LTOH(const volatile LTOH&);
    volatile LTOH& operator=(const volatile LTOH&) volatile;
    LTOH& operator=(const volatile LTOH&);
    volatile LTOH& operator=(const LTOH&) volatile;

    /**
     * @brief This sets the Output Hold delay in APB clocks. A value of 0 causes
     *        a 1 clock delay.
     */
    uint32_t toh : 4;
  };

  /**
   * @brief SPI DMA DREQ Controls
   * @note This register controls the generation of the DREQ and Panic signals
   *       to an external DMA engine. The DREQ signals are generated when the
   *       FIFOs reach their defined levels and need servicing. The Panic
   *       signals instruct the external DMA engine to raise the priority of its
   *       AXI requests.
   */
  struct DC {
    DC(const volatile DC&);
    volatile DC& operator=(const volatile DC&) volatile;
    DC& operator=(const volatile DC&);
    volatile DC& operator=(const DC&) volatile;

    /**
     * @brief DMA Read Panic Threshold.
     * @note Generate the Panic signal to the RX DMA engine whenever the RX FIFO
     *       level is greater than this amount.
     */
    uint32_t tdreq : 8;

    /**
     * @brief DMA Read Request Threshold.
     * @note Generate a DREQ to the RX DMA engine whenever the RX FIFO level is
     *       greater than this amount (RX DREQ is also generated if the transfer
     *       has finished but the RX FIFO isn’t empty).
     */
    uint32_t tpanic : 8;

    /**
     * @brief DMA Write Panic Threshold.
     * @note Generate the Panic signal to the TX DMA engine whenever the TX FIFO
     *       level is less than or equal to this amount.
     */
    uint32_t rdreq : 8;

    /**
     * @brief DMA Write Request Threshold.
     * @note Generate a DREQ signal to the TX DMA engine whenever the TX FIFO
     *       level is less than or equal to this amount.
     */
    uint32_t rpanic : 8;
  };

  volatile CS cs;      // 0x00
  volatile FIFO fifo;  // 0x04
  volatile CLK clk;    // 0x08
  volatile DLEN dlen;  // 0x0c
  volatile LTOH ltoh;  // 0x10
  volatile DC dc;      // 0x14
};

extern SpiRegisterMap* REG_SPI0;
extern SpiRegisterMap* REG_SPI3;
extern SpiRegisterMap* REG_SPI4;
extern SpiRegisterMap* REG_SPI5;
extern SpiRegisterMap* REG_SPI6;

constexpr uint32_t SPI1_BASE = 0xFE215080;
constexpr uint32_t SPI2_BASE = 0xFE2150C0;
constexpr uint32_t SPI_AUX_SIZE = 0x00000040;
typedef struct {
  volatile uint32_t CNTL0_REG;    // 0x00
  volatile uint32_t CNTL1_REG;    // 0x04
  volatile uint32_t STAT_REG;     // 0x08
  volatile uint32_t PEEK_REG;     // 0x0c
  volatile uint32_t RESERVED[4];  // 0x10 ~ 0x1c
  volatile uint32_t IO_REGa;      // 0x20
  volatile uint32_t IO_REGb;      // 0x24
  volatile uint32_t IO_REGc;      // 0x28
  volatile uint32_t IO_REGd;      // 0x2c
  volatile uint32_t TXHOLD_REGa;  // 0x30
  volatile uint32_t TXHOLD_REGb;  // 0x34
  volatile uint32_t TXHOLD_REGc;  // 0x38
  volatile uint32_t TXHOLD_REGd;  // 0x3c
} SPI_AUX_Typedef;

extern SPI_AUX_Typedef* REG_SPI1;
extern SPI_AUX_Typedef* REG_SPI2;

}  // namespace rpl

#endif