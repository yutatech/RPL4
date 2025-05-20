#include "rpl4/peripheral/spi.hpp"

#include <array>
#include <memory>

#include "rpl4/system/log.hpp"
#include "rpl4/system/system.hpp"

namespace rpl {

std::array<std::shared_ptr<Spi>, 5> Spi::instances_ = {nullptr};

std::shared_ptr<Spi> Spi::GetInstance(Port port) {
  if (!IsInitialized()) {
    Log(LogLevel::Error, "[SPI::GetInstance()] RPL is not initialized.");
  } else if (instances_[static_cast<size_t>(port)] == nullptr) {
    switch (port) {
      case Port::kSpi0:
        instances_[static_cast<size_t>(port)] =
            std::shared_ptr<Spi>(new Spi(REG_SPI0));
        break;
      case Port::kSpi3:
        instances_[static_cast<size_t>(port)] =
            std::shared_ptr<Spi>(new Spi(REG_SPI3));
        break;
      case Port::kSpi4:
        instances_[static_cast<size_t>(port)] =
            std::shared_ptr<Spi>(new Spi(REG_SPI4));
        break;
      case Port::kSpi5:
        instances_[static_cast<size_t>(port)] =
            std::shared_ptr<Spi>(new Spi(REG_SPI5));
        break;
      case Port::kSpi6:
        instances_[static_cast<size_t>(port)] =
            std::shared_ptr<Spi>(new Spi(REG_SPI6));
        break;
      default:
        Log(LogLevel::Fatal,
            "[SPI::GetInstance()] Invalid port was given as an argument.");
        break;
    }
  }
  return instances_[static_cast<size_t>(port)];
}

Spi::Spi(SpiRegisterMap* register_map) : register_map_(register_map) {}

void Spi::TransmitAndReceiveBlocking(const uint8_t* transmit_buf,
                                     uint8_t* receive_buf,
                                     uint32_t data_length) {
  StartTransmission();
  for (uint32_t i = 0; i < data_length; ++i) {
    ClearTxAndRxFifo();
    while (!IsTxFifoWritable()) {}
    WriteDataToTxFifo(static_cast<uint32_t>(transmit_buf[i]));
    while (!IsTransmissionCompleted()) {}
    while (!IsRxFifoReadable()) {}
    receive_buf[i] = static_cast<uint8_t>(ReadDataFromRxFifo());
  }
  EndTransmission();
}

}  // namespace rpl