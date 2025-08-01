#ifndef RPL4_PERIPHERAL_SPI_BASE_HPP_
#define RPL4_PERIPHERAL_SPI_BASE_HPP_

#include <array>
#include <memory>

#include "rpl4/registers/registers_spi.hpp"

namespace rpl {

class SpiBase {
 public:
  SpiBase(const SpiBase&) = delete;
  SpiBase& operator=(const SpiBase&) = delete;
  SpiBase(SpiBase&&) = delete;
  SpiBase& operator=(SpiBase&&) = delete;
  ~SpiBase() = default;

  virtual void SetChipSelectForCommunication(uint8_t chip_select) = 0;

  virtual void TransmitAndReceiveBlocking(const uint8_t* transmit_buf,
                                          uint8_t* receive_buf,
                                          uint32_t data_length) = 0;

 protected:
  SpiBase() = default;
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_SPI_BASE_HPP_