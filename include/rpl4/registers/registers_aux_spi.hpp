#ifndef RPL4_REGISTERS_AUX_SPI_HPP_
#define RPL4_REGISTERS_AUX_SPI_HPP_

#include <cstdint>

namespace rpl {

constexpr uint32_t kSpi1AddressBase = 0xFE215080;
constexpr uint32_t kSpi2AddressBase = 0xFE2150C0;
constexpr uint32_t kAuxSpiRegisterSize = 0x00000040;
struct AuxSpiRegisterMap {
  /**
   * @brief The AUX_SPIx_CNTL1_REG registers control more features of the SPI
   *        interfaces.
   */
  struct CNTL0 {
    CNTL0(const volatile CNTL0&);
    volatile CNTL0& operator=(const volatile CNTL0&) volatile;
    CNTL0& operator=(const volatile CNTL0&);
    volatile CNTL0& operator=(const CNTL0&) volatile;

    enum class ShiftOutMsBitFirst : uint32_t {
      // the data is shifted out starting with the LS bit. (bit 0)
      kLsbFirst = 0b0,
      // the data is shifted out starting with the MS bit. (bit 31 or bit 23)
      kMsbFirst = 0b1,
    };

    /**
     * @brief   Changing this bit will immediately change the polarity of the
     *          SPI clock output. It is recommended to not do this when the CS
     *          is active, as the connected devices will see this as a clock
     *          change.
     */
    enum class InvertSpiClock : uint32_t {
      kLow = 0b0,   // the 'idle' clock line state is low.
      kHigh = 0b1,  // the 'idle' clock line state is high
    };

    enum class OutRising : uint32_t {
      // data is clocked out on the falling edge of the SPI clock
      // should be sampled on the rising edge of the SPI clock
      kFallingEdge = 0b0,
      // data is clocked out on the rising edge of the SPI clock
      // should be sampled on the falling edge of the SPI clock
      kRisingEdge = 0b1,
    };

    /**
     * @brief If 1 the receive and transmit FIFOs are held in reset (and thus
     *        flushed.) This bit should be 0 during normal operation.
     */
    enum ClearFifos : uint32_t {
      kNotCleared = 0b0,  // FIFOs are not cleared.
      kCleared = 0b1,     // FIFOs are cleared.
    };

    enum class InRising : uint32_t {
      // data is clocked in on the falling edge of the SPI clock
      kFallingEdge = 0b0,
      // data is clocked in on the rising edge of the SPI clock.
      kRisingEdge = 0b1,
    };

    /**
     * @brief Enables the SPI interface. Whilst disabled the FIFOs can still be
     *        written to or read from. This bit should be 1 during normal
     *        operation.
     */
    enum class Enable : uint32_t {
      kDisable = 0b0,
      kEnable = 0b1,
    };

    /**
     * @brief Controls the extra DOUT hold time in system clock cycles.
     *        00 : No extra hold time
     *        01 : 1 system clock extra hold time
     *        10 : 4 system clocks extra hold time
     *        11 : 7 system clocks extra hold time
     * @details Because the interface runs off fast silicon the MOSI hold time
     *          against the clock will be very short. This can cause
     *          considerable problems on SPI slaves. To make it easier for the
     *          slave to see the data the hold time of the MOSI out against the
     *          SPI clock out is programmable.
     */
    enum class DoutHoldTime : uint32_t {
      k0Cycles = 0b00,
      k1Cycle = 0b01,
      k4Cycles = 0b10,
      k7Cycles = 0b11,
    };

    /**
     * @brief   In this mode the shift length is taken from the transmit FIFO.
     *          The transmit data bits 28:24 are used as shift length and the
     *          data bits 23:0 are the actual transmit data. If the option
     *          'shift MS out first' is selected the first bit shifted out will
     *          be bit 23. The receive data will arrive as normal.
     */
    enum class VariableWidth : uint32_t {
      // the SPI takes the shift length from bits 0-5 of this register
      kFromCtrl0Register = 0b0,
      // the SPI takes the shift length and the data from the TX FIFO.
      kFromTxFiFo = 0b1,
    };

    /**
     * @note Set this bit only if bit 14 (variable width) is also set.
     */
    enum class VariableCs : uint32_t {
      // the SPI takes the CS pattern from bits 17-19 of this register.
      kFromCtrl0Register = 0b0,
      // the SPI takes the CS pattern and the data from the TX FIFO register
      kFromTxFiFo = 0b1,
    };

    /**
     * @brief If set the SPI input works in post-input mode.
     */
    enum class PostInputMode : uint32_t {
      kDisabled = 0b0,
      kEnabled = 0b1,
    };

    /**
     * @brief The pattern output on the CS pins when active.
     */
    enum class ChipSelect : uint32_t {
      kChipSelect0 = 0b000,
      kChipSelect1 = 0b001,
      kChipSelect2 = 0b010,
    };

    /**
     * @brief Specifies the number of bits to shift
     * @note This field is ignored when using 'variable width' mode
     */
    uint32_t shift_length : 6;
    ShiftOutMsBitFirst shift_out_ms_bit_first : 1;
    InvertSpiClock invert_spi_clock : 1;
    OutRising out_rising : 1;
    ClearFifos clear_fifos : 1;
    InRising in_rising : 1;
    Enable enable : 1;
    DoutHoldTime dout_hold_time : 2;
    VariableWidth variable_width : 1;
    VariableCs variable_cs : 1;
    PostInputMode post_input_mode : 1;
    ChipSelect chip_select : 3;

    /**
     * @brief Sets the SPI clock speed.
     *        spi_clk_freq = system_clock_freq/2*(speed+1)
     */
    uint32_t speed : 12;
  };

  /**
   * @brief The AUX_SPIx_CNTL1_REG registers control more features of the SPI
   *        interfaces.
   */
  struct CNTL1 {
    CNTL1(const volatile CNTL1&);
    volatile CNTL1& operator=(const volatile CNTL1&) volatile;
    CNTL1& operator=(const volatile CNTL1&);
    volatile CNTL1& operator=(const CNTL1&) volatile;

    /**
     * @brief If 1 the receiver shift register is NOT cleared. Thus new data is
     *        concatenated to old data. If 0 the receiver shift register is
     *        cleared before each transaction.
     * @details Setting the 'Keep input' bit will prevent the input shift
     *          register being cleared between transactions. However the
     *          contents of the shift register is still written to the receive
     *          FIFO at the end of each transaction. E.g. if you receive two 8-
     *          bit values 0x81 followed by 0x46 the receive FIFO will contain:
     *          0x0081 in the first entry and 0x8146 in the second entry. This
     *          mode may save CPU time concatenating bits (4 bits followed by 12
     *          bits).
     */
    enum class KeepInput : uint32_t {
      kCleared = 0b0,
      kNotCleared = 0b1,
    };

    enum class ShiftInMsBitFirst : uint32_t {
      // the data is shifted in starting with the LS bit. (bit 0)
      kLsbFirst = 0b0,
      // the data is shifted in starting with the MS bit. (bit 15)
      kMsbFirst = 0b1,
    };

    /**
     * @brief If 1 the interrupt line is high when the interface is idle
     */
    enum class DoneIrq : uint32_t {
      kDisabled = 0b0,
      kEnabled = 0b1,
    };

    /**
     * @brief If 1 the interrupt line is high when the transmit FIFO is empty
     */
    enum class TxEmptyIrq : uint32_t {
      kDisabled = 0b0,
      kEnabled = 0b1,
    };

    KeepInput keep_input : 1;
    ShiftInMsBitFirst shift_in_ms_bit_first : 1;
    uint32_t reserved_1 : 4;
    DoneIrq done_irq : 1;
    TxEmptyIrq tx_empty_irq : 1;

    /**
     * @brief Additional SPI clock cycles where the CS is high.
     * @note 0 ~ 7 cycles.
     * @details The SPI CS will always be high for at least 1 SPI clock cycle.
     *          Some SPI devices need more time to process the data. This field
     *          will set a longer CS-high time. So the actual CS high time is
     *          (CS_high_time + 1) (in SPI clock cycles).
     */
    uint32_t cs_high_time : 3;
  };

  /**
   * @brief The AUX_SPIx_STAT_REG registers show the status of the SPI
   *        interfaces.
   */
  struct STAT {
    STAT(const volatile STAT&);
    volatile STAT& operator=(const volatile STAT&) volatile;
    STAT& operator=(const volatile STAT&);
    volatile STAT& operator=(const STAT&) volatile;

    /**
     * @brief Indicates the module is busy transferring data.
     */
    enum class Busy : uint32_t {
      kIdle = 0b0,
      kBusy = 0b1,
    };

    enum class RxEmpty : uint32_t {
      kNotEmpty = 0b0,  // the receiver FIFO holds at least 1 data unit.
      kEmpty = 0b1,     // the receiver FIFO is empty
    };

    enum class RxFull : uint32_t {
      kNotFull = 0b0,  // the receiver FIFO can accept at least 1 data unit.
      kFull = 0b1,     // the receiver FIFO is full
    };

    enum class TxEmpty : uint32_t {
      kNotEmpty = 0b0,  // transmit FIFO holds at least 1 data unit.
      kEmpty = 0b1,     // the transmit FIFO is empty
    };

    enum class TxFull : uint32_t {
      kNotFull = 0b0,  // the transmit FIFO can accept at least 1 data unit.
      kFull = 0b1,     // the transmit FIFO is full
    };

    /**
     * @brief The number of bits still to be processed. Starts with 'shift-
     *        length' and counts down.
     */
    uint32_t bit_count : 6;
    Busy busy : 1;
    RxEmpty rx_empty : 1;
    RxFull rx_full : 1;
    TxEmpty tx_empty : 1;
    TxFull tx_full : 1;
    uint32_t reserved_1 : 5;

    /**
     * @brief The number of data units in the receive data FIFO.
     */
    uint32_t rx_fifo_level : 4;
    uint32_t reserved_2 : 4;

    /**
     * @brief The number of data units in the transmit data FIFO
     */
    uint32_t tx_fifo_level : 4;
  };

  /**
   * @brief The AUX_SPIx_PEEK_REG registers show received data of the SPI
   *        interfaces.
   */
  struct PEEK {
    PEEK(const volatile PEEK&);
    volatile PEEK& operator=(const volatile PEEK&) volatile;
    PEEK& operator=(const volatile PEEK&);
    volatile PEEK& operator=(const PEEK&) volatile;

    /**
     * @brief Reads from this address will show the top entry from the receive
     *        FIFO, but the data is not taken from the FIFO. This provides a
     *        means of inspecting the data but not removing it from the FIFO.
     */
    uint32_t data : 32;
  };

  /**
   * @brief The AUX_SPIx_IO_REG registers are the primary data port of the SPI x
   *        interface. These four addresses all write to the same FIFO.
   */
  struct IO {
    IO(const volatile IO&);
    volatile IO& operator=(const volatile IO&) volatile;
    IO& operator=(const volatile IO&);
    volatile IO& operator=(const IO&) volatile;

    /**
     * @brief Writes to this address range end up in the transmit FIFO.
     *        Data is lost when writing whilst the transmit FIFO is full.
     *        Reads from this address will take the top entry from the
     *        receive FIFO. Reading whilst the receive FIFO is empty will
     *        return the last data received.
     */
    uint32_t data : 32;
  };

  /**
   * @brief The AUX_SPIx_TXHOLD_REG registers are the extended CS port of the
   *        SPI x interface. These four addresses all write to the same FIFO.
   */
  struct TXHOLD {
    TXHOLD(const volatile TXHOLD&);
    volatile TXHOLD& operator=(const volatile TXHOLD&) volatile;
    TXHOLD& operator=(const volatile TXHOLD&);
    volatile TXHOLD& operator=(const TXHOLD&) volatile;

    /**
     * @brief Writes to this address range end up in the transmit FIFO.
     *        Data is lost when writing whilst the transmit FIFO is full.
     *        Reads from this address will take the top entry from the
     *        receive FIFO. Reading whilst the receive FIFO is empty will
     *        return the last data received.
     */
    uint32_t data : 32;
  };

  volatile CNTL0 cntl_0;          // 0x00
  volatile CNTL1 cntl_1;          // 0x04
  volatile STAT stat;             // 0x08
  volatile PEEK peek;             // 0x0c
  volatile uint32_t reserved[4];  // 0x10 ~ 0x1c
  volatile IO io_a;               // 0x20
  volatile IO io_b;               // 0x24
  volatile IO io_c;               // 0x28
  volatile IO io_d;               // 0x2c
  volatile TXHOLD tx_hold_a;      // 0x30
  volatile TXHOLD tx_hold_b;      // 0x34
  volatile TXHOLD tx_hold_c;      // 0x38
  volatile TXHOLD tx_hold_d;      // 0x3c
};

extern AuxSpiRegisterMap* REG_SPI1;
extern AuxSpiRegisterMap* REG_SPI2;

}  // namespace rpl

#endif