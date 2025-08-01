#ifndef RPL4_PERIPHERAL_AUX_SPI_HPP_
#define RPL4_PERIPHERAL_AUX_SPI_HPP_

#include <array>
#include <memory>

#include "rpl4/registers/registers_aux.hpp"
#include "rpl4/registers/registers_aux_spi.hpp"

namespace rpl {

class AuxSpi {
 public:
  enum class Port : size_t {
    kAuxSpi1 = 0,
    kAuxSpi2 = 1,
  };

  /**
   * @brief Get the AuxSpi instance of specified spi port.
   * @details To save memory, only the port instance obtained with GetInstance()
   *          is created. If a port instance has already been created, the same
   *          instance will be returned.
   *
   * @param port
   * @return std::shared_ptr<AuxSpi>
   */
  static std::shared_ptr<AuxSpi> GetInstance(Port port);

  AuxSpi(const AuxSpi&) = delete;
  AuxSpi& operator=(const AuxSpi&) = delete;
  AuxSpi(AuxSpi&&) = delete;
  AuxSpi& operator=(AuxSpi&&) = delete;
  ~AuxSpi() = default;

  /**
   * @brief Get the AuxSpiRegisterMap pointer.
   *
   * @return AuxSpiRegisterMap*
   */
  inline AuxSpiRegisterMap* GetRegister() const { return register_map_; }

  /**
   * @brief Enable the AuxSpi peripheral.
   */
  inline void Enable() {
    register_map_->cntl_0.enable = AuxSpiRegisterMap::CNTL0::Enable::kEnable;
    if (register_map_ == REG_SPI1) {
      REG_AUX->enables.spi1 = AuxRegisterMap::Enables::Spi1Enable::kEnabled;
    } else if (register_map_ == REG_SPI2) {
      REG_AUX->enables.spi2 = AuxRegisterMap::Enables::Spi2Enable::kEnabled;
    }
    ConfigureDataShiftTx();
    ConfigureDataShiftRx();
  }

  /**
   * @brief Disable the AuxSpi peripheral.
   * @details The FIFOs can still be written to or read from when disabled.
   */
  inline void Disable() {
    register_map_->cntl_0.enable = AuxSpiRegisterMap::CNTL0::Enable::kDisable;
    if (register_map_ == REG_SPI1) {
      REG_AUX->enables.spi1 = AuxRegisterMap::Enables::Spi1Enable::kDisabled;
    } else if (register_map_ == REG_SPI2) {
      REG_AUX->enables.spi2 = AuxRegisterMap::Enables::Spi2Enable::kDisabled;
    }
  }

  using ChipSelect = AuxSpiRegisterMap::CNTL0::ChipSelect;
  inline void SetChipSelectForCommunication(ChipSelect chip_select) {
    register_map_->cntl_0.chip_select = chip_select;
  }

  using MisoClockPhase = AuxSpiRegisterMap::CNTL0::InRising;
  /**
   * @brief Sets the timing of the clock phase that changes the MISO line.
   *
   * @param clock_phase
   */
  inline void SetMisoClockPhase(MisoClockPhase clock_phase) {
    register_map_->cntl_0.in_rising = clock_phase;
    ConfigureDataShiftRx();
  }

  using MosiClockPhase = AuxSpiRegisterMap::CNTL0::OutRising;
  /**
   * @brief Sets the timing of the clock phase that changes the MOSI line.
   *
   * @param clock_phase
   */
  inline void SetMosiClockPhase(MosiClockPhase clock_phase) {
    register_map_->cntl_0.out_rising = clock_phase;
    ConfigureDataShiftTx();
  }

  /**
   * @brief Get the MISO Clock Phase
   *
   * @return ClockPhase
   */
  inline MisoClockPhase GetMisoClockPhase() {
    return register_map_->cntl_0.in_rising;
  }

  /**
   * @brief Get the MOSI Clock Phase
   *
   * @return ClockPhase
   */
  inline MosiClockPhase GetMosiClockPhase() {
    return register_map_->cntl_0.out_rising;
  }

  using ClockPolarity = AuxSpiRegisterMap::CNTL0::InvertSpiClock;
  /**
   * @brief Specifies the output polarity of the SCLK in the idle state.
   *
   * @param clock_polarity
   */
  inline void SetClockPolarity(ClockPolarity clock_polarity) {
    register_map_->cntl_0.invert_spi_clock = clock_polarity;
    ConfigureDataShiftTx();
    ConfigureDataShiftRx();
  }

  /**
   * @brief Get the Clock Polarity
   *
   * @return ClockPolarity
   *    If kLow is set, SCLK is low in the idle state.
   *    If kHigh is set, SCLK is high in the idle state.
   */
  inline ClockPolarity GetClockPolarity() {
    return register_map_->cntl_0.invert_spi_clock;
  }

  /**
   * @brief Specifies the SCK frequency.
   *
   * @param divider
   *
   * @note SCK frequency is 250MHz / (2 * (divider + 1)). Must be 0 ~ 4095
   */
  void SetClockDivider(uint16_t divider);

  using MisoBitOrder = AuxSpiRegisterMap::CNTL1::ShiftInMsBitFirst;
  inline void SetMisoBitOrder(MisoBitOrder bit_order) {
    register_map_->cntl_1.shift_in_ms_bit_first = bit_order;
    ConfigureDataShiftRx();
  }

  using MosiBitOrder = AuxSpiRegisterMap::CNTL0::ShiftOutMsBitFirst;
  inline void SetMosiBitOrder(MosiBitOrder bit_order) {
    register_map_->cntl_0.shift_out_ms_bit_first = bit_order;
    ConfigureDataShiftTx();
  }

  /**
   * @brief The SPI CS will always be high for at least 1 SPI clock cycle.
   *        Some SPI devices need more time to process the data. This field
   *        will set a longer CS-high time. So the actual CS high time is
   *        (CS_high_time + 1) (in SPI clock cycles).
   *
   * @param cycles Must be 0 ~ 7
   */
  void SetCsHighCycles(uint8_t cycles);

  void SetBitLength(uint8_t bit_length);

  inline bool IsTransmissionCompleted() {
    return register_map_->stat.busy == AuxSpiRegisterMap::STAT::Busy::kIdle;
  }

  inline bool IsTxFifoWritable() {
    return register_map_->stat.tx_full !=
           AuxSpiRegisterMap::STAT::TxFull::kFull;
  }

  inline bool IsRxFifoReadable() {
    return register_map_->stat.rx_empty !=
           AuxSpiRegisterMap::STAT::RxEmpty::kEmpty;
  }

  inline void WriteDataToTxFifo(uint32_t data) {
    register_map_->tx_hold_a.data = data;
  }
  inline void WriteFinalDataToTxFifo(uint32_t data) {
    register_map_->io_a.data = data;
  }

  inline uint32_t ReadDataFromRxFifo() const {
    return register_map_->io_a.data;
  }

  void TransmitAndReceiveBlocking(const uint8_t* transmit_buf,
                                  uint8_t* receive_buf, uint32_t data_length);

 private:
  AuxSpi(AuxSpiRegisterMap* register_map);

  static constexpr size_t kNumOfInstances = 5;
  static std::array<std::shared_ptr<AuxSpi>, kNumOfInstances> instances_;

  AuxSpiRegisterMap* register_map_;

  // How many bit are right shifted when writing to FIFO.
  uint8_t data_shift_tx_ = 0;
  // How many bit are right shifted when reading from FIFO.
  uint8_t data_shift_rx_ = 0;

  void ConfigureDataShiftTx();
  void ConfigureDataShiftRx();
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_AUX_SPI_HPP_