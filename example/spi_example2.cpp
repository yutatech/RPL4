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

  std::shared_ptr<rpl::Spi> spi0 = rpl::Spi::GetInstance(rpl::Spi::Port::kSpi0);
  std::shared_ptr<rpl::Spi> spi6 = rpl::Spi::GetInstance(rpl::Spi::Port::kSpi3);
  
  rpl::SetGpioFunction(7, rpl::GPIO_Function::ALT0);  // SPI0_CE1
  rpl::SetGpioFunction(8, rpl::GPIO_Function::ALT0);  // SPI0_CE0
  rpl::SetGpioFunction(9, rpl::GPIO_Function::ALT0);  // SPI0_MISO
  rpl::SetGpioFunction(10, rpl::GPIO_Function::ALT0); // SPI0_MOSI
  rpl::SetGpioFunction(11, rpl::GPIO_Function::ALT0); // SPI0_SCLK
  
  rpl::SetGpioFunction(0, rpl::GPIO_Function::ALT3);  // SPI0_CE0
  rpl::SetGpioFunction(1, rpl::GPIO_Function::ALT3);  // SPI0_MISO
  rpl::SetGpioFunction(2, rpl::GPIO_Function::ALT3); // SPI0_MOSI
  rpl::SetGpioFunction(3, rpl::GPIO_Function::ALT3); // SPI0_SCLK


  // rpl::SetGpioFunction(27, rpl::GPIO_Function::ALT5); // SPI6_CE1
  // rpl::SetGpioFunction(18, rpl::GPIO_Function::ALT4); // SPI6_CE0
  // rpl::SetGpioFunction(19, rpl::GPIO_Function::ALT4); // SPI6_MISO
  // rpl::SetGpioFunction(20, rpl::GPIO_Function::ALT4); // SPI6_MOSI
  // rpl::SetGpioFunction(21, rpl::GPIO_Function::ALT4); // SPI6_SCLK

  spi0->SetClockPhase(rpl::Spi::ClockPhase::kBeginning);
  spi0->SetClockPolarity(rpl::Spi::ClockPolarity::kLow);
  spi0->SetClockDivider(4096);
  spi0->SetCs0Polarity(rpl::Spi::CsPolarity::kLow);
  spi0->SetCs1Polarity(rpl::Spi::CsPolarity::kHigh);

  spi6->SetClockPhase(rpl::Spi::ClockPhase::kBeginning);
  spi6->SetClockPolarity(rpl::Spi::ClockPolarity::kHigh);
  spi6->SetClockDivider(4096);
  spi6->SetCs0Polarity(rpl::Spi::CsPolarity::kLow);
  spi6->SetCs1Polarity(rpl::Spi::CsPolarity::kHigh);

  int i=0;
  while (true) {
    uint8_t tx_buf0[13] = "hello world\n";
    uint8_t tx_buf6[13] = "HELLO WORLD\n";
    uint8_t rx_buf0[13];
    uint8_t rx_buf6[13];
    spi0->SetChipSelectForCommunication(rpl::Spi::ChipSelect::kChipSelect0);
    spi6->SetChipSelectForCommunication(rpl::Spi::ChipSelect::kChipSelect0);
    spi0->StartTransmission();
    spi6->StartTransmission();
    for (uint32_t i = 0; i < 13; ++i) {
      spi0->ClearTxAndRxFifo();
      spi6->ClearTxAndRxFifo();
      while (!spi0->IsTxFifoWritable() && !spi6->IsTxFifoWritable()) {}
      spi0->WriteDataToTxFifo(static_cast<uint32_t>(tx_buf0[i]));
      spi6->WriteDataToTxFifo(static_cast<uint32_t>(tx_buf6[i]));
      while (!spi0->IsTransmissionCompleted() && !spi6->IsTransmissionCompleted()) {}
      while (!spi0->IsRxFifoReadable() && !spi6->IsRxFifoReadable()) {}
      rx_buf0[i] = static_cast<uint8_t>(spi0->ReadDataFromRxFifo());
      rx_buf6[i] = static_cast<uint8_t>(spi6->ReadDataFromRxFifo());
    }
    spi0->EndTransmission();
    spi6->EndTransmission();

    std::cout << i << std::endl;
    std::cout << rx_buf0;
    std::cout << i++ << std::endl;
    std::cout << rx_buf6;

    std::this_thread::sleep_for(100ms);
  }

  return 0;
}