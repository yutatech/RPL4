#include <chrono>
#include <memory>
#include <thread>
#include <cstdio>
#include <iostream>
#include <bitset>

#include "rpl4/peripheral/spi.hpp"
#include "rpl4/rpl4.hpp"

int main(void) {
  rpl::Init();
  using namespace std::chrono_literals;

  std::shared_ptr<rpl::Spi> spi = rpl::Spi::GetInstance(rpl::Spi::Port::kSpi3);
  
  rpl::SetGpioFunction(7, rpl::GPIO_Function::ALT0);  // SPI0_CE1
  rpl::SetGpioFunction(8, rpl::GPIO_Function::ALT0);  // SPI0_CE0
  rpl::SetGpioFunction(9, rpl::GPIO_Function::ALT0);  // SPI0_MISO
  rpl::SetGpioFunction(10, rpl::GPIO_Function::ALT0); // SPI0_MOSI
  rpl::SetGpioFunction(11, rpl::GPIO_Function::ALT0); // SPI0_SCLK

  rpl::SetGpioFunction(27, rpl::GPIO_Function::ALT5); // SPI6_CE1
  rpl::SetGpioFunction(18, rpl::GPIO_Function::ALT3); // SPI6_CE0
  rpl::SetGpioFunction(19, rpl::GPIO_Function::ALT3); // SPI6_MISO
  rpl::SetGpioFunction(20, rpl::GPIO_Function::ALT3); // SPI6_MOSI
  rpl::SetGpioFunction(21, rpl::GPIO_Function::ALT3); // SPI6_SCLK

  rpl::SetGpioFunction(0, rpl::GPIO_Function::ALT3);  // SPI3_CE0
  rpl::SetGpioFunction(1, rpl::GPIO_Function::ALT3);  // SPI3_MISO
  rpl::SetGpioFunction(2, rpl::GPIO_Function::ALT3); // SPI3_MOSI
  rpl::SetGpioFunction(3, rpl::GPIO_Function::ALT3); // SPI3_SCLK

  spi->SetClockPhase(rpl::Spi::ClockPhase::kBeginning);
  spi->SetClockPolarity(rpl::Spi::ClockPolarity::kLow);
  spi->SetClockDivider(4096);
  spi->SetCs0Polarity(rpl::Spi::CsPolarity::kLow);
  spi->SetCs1Polarity(rpl::Spi::CsPolarity::kHigh);

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