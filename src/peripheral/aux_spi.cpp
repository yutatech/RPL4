#include "rpl4/peripheral/aux_spi.hpp"

#include <array>
#include <memory>

#include "rpl4/system/log.hpp"
#include "rpl4/system/system.hpp"

namespace rpl {

std::array<std::shared_ptr<AuxSpi>, AuxSpi::kNumOfInstances>
    AuxSpi::instances_ = {nullptr};

std::shared_ptr<AuxSpi> AuxSpi::GetInstance(Port port) {
  if (!IsInitialized()) {
    Log(LogLevel::Error, "[SPI::GetInstance()] RPL is not initialized.");
  } else if (instances_[static_cast<size_t>(port)] == nullptr) {
    switch (port) {
      case Port::kAuxSpi1:
        instances_[static_cast<size_t>(port)] =
            std::shared_ptr<AuxSpi>(new AuxSpi(REG_SPI1));
        break;
      case Port::kAuxSpi2:
        instances_[static_cast<size_t>(port)] =
            std::shared_ptr<AuxSpi>(new AuxSpi(REG_SPI2));
        break;
      default:
        Log(LogLevel::Fatal,
            "[SPI::GetInstance()] Invalid port was given as an argument.");
        break;
    }
  }
  return instances_[static_cast<size_t>(port)];
}

AuxSpi::AuxSpi(AuxSpiRegisterMap* register_map) : register_map_(register_map) {}

void AuxSpi::SetClockDivider(uint16_t divider) {
  if (divider > 4095) {
    Log(LogLevel::Error,
        "[AuxSpi::SetClockDivider()] Invalid divider: {}. Must be 0 ~ 4095",
        static_cast<int>(divider));
    return;
  }
  register_map_->cntl_0.speed = static_cast<uint32_t>(divider);
}

void AuxSpi::SetCsHighCycles(uint8_t cycles) {
  if (cycles > 7) {
    Log(LogLevel::Error,
        "[AuxSpi::SetCsHighCycles()] Invalid cycles: {}. Must be 0 ~ 7",
        static_cast<int>(cycles));
    return;
  }
  register_map_->cntl_1.cs_high_time = static_cast<uint32_t>(cycles);
}

void AuxSpi::SetBitLength(uint8_t bit_length) {
  if (bit_length < 1 || bit_length > 32) {
    Log(LogLevel::Error,
        "[AuxSpi::SetBitLength()] Invalid bit length: {}. Must be 1 ~ 32",
        static_cast<int>(bit_length));
    return;
  }
  register_map_->cntl_0.shift_length = static_cast<uint32_t>(bit_length);
}

void AuxSpi::TransmitAndReceiveBlocking(const uint8_t* transmit_buf,
                                        uint8_t* receive_buf,
                                        uint32_t data_length) {
  // Clear the receive FIFO if it has data.
  while (IsRxFifoReadable()) { ReadDataFromRxFifo(); }
  uint32_t tx_counter = 0;
  uint32_t rx_counter = 0;
  while (rx_counter < data_length) {
    if (IsTxFifoWritable() && rx_counter == tx_counter) {
      if (tx_counter == data_length - 1) {
        WriteFinalDataToTxFifo(static_cast<uint32_t>(transmit_buf[tx_counter++])
                               << data_shift_tx_);
      } else if (tx_counter < data_length - 1) {
        WriteDataToTxFifo(static_cast<uint32_t>(transmit_buf[tx_counter++])
                          << data_shift_tx_);
      }
    }
    if (IsRxFifoReadable()) {
      receive_buf[rx_counter++] =
          static_cast<uint8_t>(ReadDataFromRxFifo() >> data_shift_rx_);
    }
  }
}

void AuxSpi::ConfigureDataShiftTx() {
  if (register_map_->cntl_0.shift_out_ms_bit_first == MosiBitOrder::kLsbFirst) {
    if ((register_map_->cntl_0.invert_spi_clock == ClockPolarity::kHigh &&
         register_map_->cntl_0.out_rising == MosiClockPhase::kFallingEdge) ||
        (register_map_->cntl_0.invert_spi_clock == ClockPolarity::kLow &&
         register_map_->cntl_0.out_rising == MosiClockPhase::kRisingEdge)) {
      data_shift_tx_ = 1;
    } else {
      data_shift_tx_ = 0;
    }
  } else {
    if ((register_map_->cntl_0.invert_spi_clock == ClockPolarity::kHigh &&
         register_map_->cntl_0.out_rising == MosiClockPhase::kFallingEdge) ||
        (register_map_->cntl_0.invert_spi_clock == ClockPolarity::kLow &&
         register_map_->cntl_0.out_rising == MosiClockPhase::kRisingEdge)) {
      data_shift_tx_ = 31 - register_map_->cntl_0.shift_length;
    } else {
      data_shift_tx_ = 32 - register_map_->cntl_0.shift_length;
    }
  }
}

void AuxSpi::ConfigureDataShiftRx() {
  if (register_map_->cntl_1.shift_in_ms_bit_first == MisoBitOrder::kMsbFirst) {
    if ((register_map_->cntl_0.invert_spi_clock == ClockPolarity::kHigh &&
         register_map_->cntl_0.in_rising == MisoClockPhase::kFallingEdge) ||
        (register_map_->cntl_0.invert_spi_clock == ClockPolarity::kLow &&
         register_map_->cntl_0.in_rising == MisoClockPhase::kRisingEdge)) {
      data_shift_rx_ = 0;
    } else {
      data_shift_rx_ = 1;
    }
  } else {
    if ((register_map_->cntl_0.invert_spi_clock == ClockPolarity::kHigh &&
         register_map_->cntl_0.in_rising == MisoClockPhase::kFallingEdge) ||
        (register_map_->cntl_0.invert_spi_clock == ClockPolarity::kLow &&
         register_map_->cntl_0.in_rising == MisoClockPhase::kRisingEdge)) {
      data_shift_rx_ = 32 - register_map_->cntl_0.shift_length;
    } else {
      data_shift_rx_ = 31 - register_map_->cntl_0.shift_length;
    }
  }
}

}  // namespace rpl