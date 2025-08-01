#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <thread>

#include "rpl4/peripheral/aux_spi.hpp"
#include "rpl4/peripheral/gpio.hpp"
#include "rpl4/rpl4.hpp"

int main(void) {
  rpl::Init();
  using namespace std::chrono_literals;

  std::shared_ptr<rpl::AuxSpi> spi =
      rpl::AuxSpi::GetInstance(rpl::AuxSpi::Port::kAuxSpi1);

  // GPIO configuration
  rpl::Gpio::SetAltFunction(16, rpl::Gpio::AltFunction::kAlt4);  // SPI1_CE2
  rpl::Gpio::SetAltFunction(17, rpl::Gpio::AltFunction::kAlt4);  // SPI1_CE1
  rpl::Gpio::SetAltFunction(18, rpl::Gpio::AltFunction::kAlt4);  // SPI1_CE0
  rpl::Gpio::SetAltFunction(19, rpl::Gpio::AltFunction::kAlt4);  // SPI1_MISO
  rpl::Gpio::SetAltFunction(20, rpl::Gpio::AltFunction::kAlt4);  // SPI1_MOSI
  rpl::Gpio::SetAltFunction(21, rpl::Gpio::AltFunction::kAlt4);  // SPI1_SCLK

  // SPI1_CE2
  rpl::Gpio::SetPullRegister(16, rpl::Gpio::PullRegister::kNoRegister);
  // SPI1_CE1
  rpl::Gpio::SetPullRegister(17, rpl::Gpio::PullRegister::kNoRegister);
  // SPI1_CE0
  rpl::Gpio::SetPullRegister(18, rpl::Gpio::PullRegister::kNoRegister);
  // SPI1_MISO
  rpl::Gpio::SetPullRegister(19, rpl::Gpio::PullRegister::kPullDown);
  // SPI1_MOSI
  rpl::Gpio::SetPullRegister(20, rpl::Gpio::PullRegister::kNoRegister);
  // SPI1_SCLK
  rpl::Gpio::SetPullRegister(21, rpl::Gpio::PullRegister::kNoRegister);

  // SPI configuration
  spi->Enable();
  spi->SetMisoClockPhase(rpl::AuxSpi::MisoClockPhase::kRisingEdge);
  spi->SetMosiClockPhase(rpl::AuxSpi::MosiClockPhase::kFallingEdge);
  spi->SetClockPolarity(rpl::AuxSpi::ClockPolarity::kHigh);
  spi->SetClockDivider(1024);
  spi->SetBitLength(8);  // 8 bits per transfer
  spi->SetMisoBitOrder(rpl::AuxSpi::MisoBitOrder::kMsbFirst);
  spi->SetMosiBitOrder(rpl::AuxSpi::MosiBitOrder::kMsbFirst);

  while (true) {
    uint8_t tx_buf[13] = "Hello World\n";
    uint8_t rx_buf[13];
    spi->SetChipSelectForCommunication(rpl::AuxSpi::ChipSelect::kChipSelect0);
    spi->TransmitAndReceiveBlocking(tx_buf, rx_buf, 13);
    std::cout << rx_buf;

    std::this_thread::sleep_for(100ms);
  }

  return 0;
}