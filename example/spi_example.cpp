#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <thread>

#include "rpl4/peripheral/gpio.hpp"
#include "rpl4/peripheral/spi.hpp"
#include "rpl4/rpl4.hpp"

int main(void) {
  rpl::Init();
  using namespace std::chrono_literals;

  std::shared_ptr<rpl::Spi> spi = rpl::Spi::GetInstance(rpl::Spi::Port::kSpi0);

  // GPIO configuration
  rpl::Gpio::SetAltFunction(7, rpl::Gpio::AltFunction::kAlt0);   // SPI0_CE1
  rpl::Gpio::SetAltFunction(8, rpl::Gpio::AltFunction::kAlt0);   // SPI0_CE0
  rpl::Gpio::SetAltFunction(9, rpl::Gpio::AltFunction::kAlt0);   // SPI0_MISO
  rpl::Gpio::SetAltFunction(10, rpl::Gpio::AltFunction::kAlt0);  // SPI0_MOSI
  rpl::Gpio::SetAltFunction(11, rpl::Gpio::AltFunction::kAlt0);  // SPI0_SCLK

  // SPI0_CE1
  rpl::Gpio::SetPullRegister(7, rpl::Gpio::PullRegister::kNoRegister);
  // SPI0_CE0
  rpl::Gpio::SetPullRegister(8, rpl::Gpio::PullRegister::kNoRegister);
  // SPI0_MISO
  rpl::Gpio::SetPullRegister(9, rpl::Gpio::PullRegister::kPullDown);
  // SPI0_MOSI
  rpl::Gpio::SetPullRegister(10, rpl::Gpio::PullRegister::kNoRegister);
  // SPI0_SCLK
  rpl::Gpio::SetPullRegister(11, rpl::Gpio::PullRegister::kNoRegister);

  // SPI configuration
  spi->SetClockPhase(rpl::Spi::ClockPhase::kBeginning);
  spi->SetClockPolarity(rpl::Spi::ClockPolarity::kLow);
  spi->SetClockDivider(1024);
  spi->SetCs0Polarity(rpl::Spi::CsPolarity::kLow);
  spi->SetCs1Polarity(rpl::Spi::CsPolarity::kHigh);
  spi->SetReadEnable(rpl::Spi::ReadEnable::kDisable);

  while (true) {
    uint8_t tx_buf[13] = "Hello World\n";
    uint8_t rx_buf[13];
    spi->SetChipSelectForCommunication(rpl::Spi::ChipSelect::kChipSelect0);
    spi->TransmitAndReceiveBlocking(tx_buf, rx_buf, 13);
    std::cout << rx_buf;

    spi->SetChipSelectForCommunication(rpl::Spi::ChipSelect::kChipSelect1);
    spi->TransmitAndReceiveBlocking(tx_buf, rx_buf, 13);
    std::cout << rx_buf;

    std::this_thread::sleep_for(100ms);
  }

  return 0;
}