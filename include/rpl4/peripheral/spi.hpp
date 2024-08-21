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
  inline SpiRegisterMap* GetRegister() { return register_map_; }

  inline void SetChipSelectForCommunication(SpiRegisterMap::CS::CS_ cs) {
    register_map_->cs.cs = cs;
  }

  inline void SetClockPhase(SpiRegisterMap::CS::CPHA cpha) {
    register_map_->cs.cpha = cpha;
  }

  inline void SetClockPolarity(SpiRegisterMap::CS::CPOL cpol) {
    register_map_->cs.cpol = cpol;
  }

  inline void SetCsPolarity(SpiRegisterMap::CS::CSPOL cspol) {
    register_map_->cs.cspol = cspol;
  }

  inline void SetCs0Polarity(SpiRegisterMap::CS::CSPOL cspol0) {
    register_map_->cs.cspol0 = cspol0;
  }

  inline void SetCs1Polarity(SpiRegisterMap::CS::CSPOL cspol1) {
    register_map_->cs.cspol1 = cspol1;
  }

  inline void SetCs2Polarity(SpiRegisterMap::CS::CSPOL cspol2) {
    register_map_->cs.cspol2 = cspol2;
  }

  inline void EnableDma() {
    register_map_->cs.dmaen = SpiRegisterMap::CS::DMAEN::kEnable;
  }

  inline void DisableDma() {
    register_map_->cs.dmaen = SpiRegisterMap::CS::DMAEN::kDisable;
  }

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

  inline uint32_t ReadDataFromRxFifo() { return register_map_->fifo.data; }

  void TransmitAndReceiveBlocking(uint8_t* transmit_buf, uint8_t* receive_buf,
                                  uint32_t data_length);

 private:
  Spi(SpiRegisterMap* register_map);

  static std::array<std::shared_ptr<Spi>, 5> instances_;

  SpiRegisterMap* register_map_;
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_SPI_HPP_