#ifndef RPL4_PERIPHERAL_SPI_HPP_
#define RPL4_PERIPHERAL_SPI_HPP_

#include <array>
#include <memory>

#include "rpl4/registers/registers_spi.hpp"

namespace rpl {

class Spi {
 public:
  enum class Port {
    kSpi0 = 0,
    kSpi3 = 1,
    kSpi4 = 2,
    kSpi5 = 3,
    kSpi6 = 4,
  };

  /**
   * @brief Get the Spi instance of specified spi port.
   * @details To save memory, only the port instance obtained with GetInstance()
   *          is created. If a port instance has already been created, the same
   *          instance will be returned.
   *
   * @param port
   * @return std::shared_ptr<Spi>
   */
  static std::shared_ptr<Spi> GetInstance(Port port);

  Spi(const Spi&) = delete;
  Spi& operator=(const Spi&) = delete;
  Spi(Spi&&) = delete;
  Spi& operator=(Spi&&) = delete;
  ~Spi() = default;

  /**
   * @brief Get the SpiRegisterMap pointer.
   *
   * @return SpiRegisterMap*
   */
  inline SpiRegisterMap* GetRegister() const { return register_map_; }

  using ChipSelect = SpiRegisterMap::CS::CS_;
  inline void SetChipSelectForCommunication(ChipSelect chip_select) {
    register_map_->cs.cs = chip_select;
  }

  using ClockPhase = SpiRegisterMap::CS::CPHA;
  /**
   * @brief Set the Clock Phase
   *
   * @param clock_phase
   *    If kBeginning is set, SCLK transitions in the beginning of the data bit,
   *    and the receiver should sample the data bit on the first edge of SCLK.
   *    If kMiddle is set, SCLK transitions in the middle of the data bit, and
   *    the receiver should sample the data bit on the second edge of SCLK.
   */
  inline void SetClockPhase(ClockPhase clock_phase) {
    register_map_->cs.cpha = clock_phase;
  }

  using ClockPolarity = SpiRegisterMap::CS::CPOL;
  /**
   * @brief Specifies the output polarity of the SCLK in the idle state.
   *
   * @param clock_polarity
   */
  inline void SetClockPolarity(ClockPolarity clock_polarity) {
    register_map_->cs.cpol = clock_polarity;
  }

  using CsPolarity = SpiRegisterMap::CS::CSPOL;
  /**
   * @brief Specifies the output polarity of the CS0 in the active state.
   *
   * @param cs_polarity
   */
  inline void SetCs0Polarity(CsPolarity cs_polarity) {
    register_map_->cs.cspol0 = cs_polarity;
  }

  /**
   * @brief Specifies the output polarity of the CS1 in the active state.
   *
   * @param cs_polarity
   */
  inline void SetCs1Polarity(CsPolarity cs_polarity) {
    register_map_->cs.cspol1 = cs_polarity;
  }

  /**
   * @brief Specifies the output polarity of the CS2 in the active state.
   *
   * @param cs_polarity
   */
  inline void SetCs2Polarity(CsPolarity cs_polarity) {
    register_map_->cs.cspol2 = cs_polarity;
  }

  inline void EnableDma() {
    register_map_->cs.dmaen = SpiRegisterMap::CS::DMAEN::kEnable;
  }

  inline void DisableDma() {
    register_map_->cs.dmaen = SpiRegisterMap::CS::DMAEN::kDisable;
  }

  /**
   * @brief Specifies the SCK frequency.
   *
   * @param divider
   *
   * @note SCK frequency is 500MHz / DIV. If 0 or 1 is set to
   *       divider, DIV is 65536. If another value is set to divider, DIV is
   *       divider. For example, if the divider is set to 100, the frequency
   *       will be 5MHz.
   */
  inline void SetClockDivider(uint16_t divider) {
    register_map_->clk.cdiv = static_cast<uint32_t>(divider);
  }

  inline void ClearTxFifo() {
    register_map_->cs.clear = SpiRegisterMap::CS::CLEAR::kClearTxFifo;
  }

  inline void ClearRxFifo() {
    register_map_->cs.clear = SpiRegisterMap::CS::CLEAR::kClearRxFifo;
  }

  inline void ClearTxAndRxFifo() {
    register_map_->cs.clear = SpiRegisterMap::CS::CLEAR::kClearBothFifo;
  }

  inline void StartTransmission() {
    register_map_->cs.ta = SpiRegisterMap::CS::TA::kActive;
  }

  inline void EndTransmission() {
    register_map_->cs.ta = SpiRegisterMap::CS::TA::kInactive;
  }

  inline bool IsTransmissionCompleted() {
    return register_map_->cs.done == SpiRegisterMap::CS::DONE::kCompleted;
  }

  inline bool IsTxFifoWritable() {
    return register_map_->cs.txd == SpiRegisterMap::CS::TXD::kWritable;
  }

  inline bool IsRxFifoReadable() {
    return register_map_->cs.rxd == SpiRegisterMap::CS::RXD::kContains;
  }

  inline void WriteDataToTxFifo(uint32_t data) {
    register_map_->fifo.data = data;
  }

  using ReadEnable = SpiRegisterMap::CS::REN;
  /**
   * @brief Set Read Enable state.
   *
   * @param read_enable
   *        kDisable : MOSI pin is in write mode. kEnable : MOSI pin is in
   *        read mode and does not output any signal.
   */
  inline void SetReadEnable(ReadEnable read_enable) {
    register_map_->cs.ren = read_enable;
  }

  inline uint32_t ReadDataFromRxFifo() const { return register_map_->fifo.data; }

  void TransmitAndReceiveBlocking(const uint8_t* transmit_buf, uint8_t* receive_buf,
                                  uint32_t data_length);

 private:
  Spi(SpiRegisterMap* register_map);

  static std::array<std::shared_ptr<Spi>, 5> instances_;

  SpiRegisterMap* register_map_;
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_SPI_HPP_