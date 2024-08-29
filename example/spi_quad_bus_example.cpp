#include <bitset>
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

  std::shared_ptr<rpl::Spi> spi0 = rpl::Spi::GetInstance(rpl::Spi::Port::kSpi0);
  std::shared_ptr<rpl::Spi> spi3 = rpl::Spi::GetInstance(rpl::Spi::Port::kSpi3);
  std::shared_ptr<rpl::Spi> spi4 = rpl::Spi::GetInstance(rpl::Spi::Port::kSpi4);
  std::shared_ptr<rpl::Spi> spi5 = rpl::Spi::GetInstance(rpl::Spi::Port::kSpi5);

  rpl::Gpio::SetAltFunction(8, rpl::Gpio::AltFunction::kAlt0);   // SPI0_CE0
  rpl::Gpio::SetAltFunction(9, rpl::Gpio::AltFunction::kAlt0);   // SPI0_MISO
  rpl::Gpio::SetAltFunction(10, rpl::Gpio::AltFunction::kAlt0);  // SPI0_MOSI
  rpl::Gpio::SetAltFunction(11, rpl::Gpio::AltFunction::kAlt0);  // SPI0_SCLK

  // SPI0_CE0
  rpl::Gpio::SetPullRegister(8, rpl::Gpio::PullRegister::kNoRegister);
  // SPI0_MISO
  rpl::Gpio::SetPullRegister(9, rpl::Gpio::PullRegister::kPullDown);
  // SPI0_MOSI
  rpl::Gpio::SetPullRegister(10, rpl::Gpio::PullRegister::kPullDown);
  // SPI0_SCLK
  rpl::Gpio::SetPullRegister(11, rpl::Gpio::PullRegister::kNoRegister);

  rpl::Gpio::SetAltFunction(0, rpl::Gpio::AltFunction::kAlt3);  // SPI3_CE0
  rpl::Gpio::SetAltFunction(1, rpl::Gpio::AltFunction::kAlt3);  // SPI3_MISO
  rpl::Gpio::SetAltFunction(2, rpl::Gpio::AltFunction::kAlt3);  // SPI3_MOSI
  rpl::Gpio::SetAltFunction(3, rpl::Gpio::AltFunction::kAlt3);  // SPI3_SCLK

  // SPI3_CE0
  rpl::Gpio::SetPullRegister(0, rpl::Gpio::PullRegister::kNoRegister);
  // SPI3_MISO
  rpl::Gpio::SetPullRegister(1, rpl::Gpio::PullRegister::kPullDown);
  // SPI3_MOSI
  rpl::Gpio::SetPullRegister(2, rpl::Gpio::PullRegister::kPullDown);
  // SPI3_SCLK
  rpl::Gpio::SetPullRegister(3, rpl::Gpio::PullRegister::kNoRegister);

  rpl::Gpio::SetAltFunction(4, rpl::Gpio::AltFunction::kAlt3);  // SPI4_CE0
  rpl::Gpio::SetAltFunction(5, rpl::Gpio::AltFunction::kAlt3);  // SPI4_MISO
  rpl::Gpio::SetAltFunction(6, rpl::Gpio::AltFunction::kAlt3);  // SPI4_MOSI
  rpl::Gpio::SetAltFunction(7, rpl::Gpio::AltFunction::kAlt3);  // SPI4_SCLK

  // SPI4_CE0
  rpl::Gpio::SetPullRegister(4, rpl::Gpio::PullRegister::kNoRegister);
  // SPI4_MISO
  rpl::Gpio::SetPullRegister(5, rpl::Gpio::PullRegister::kPullDown);
  // SPI4_MOSI
  rpl::Gpio::SetPullRegister(6, rpl::Gpio::PullRegister::kPullDown);
  // SPI4_SCLK
  rpl::Gpio::SetPullRegister(7, rpl::Gpio::PullRegister::kNoRegister);

  rpl::Gpio::SetAltFunction(12, rpl::Gpio::AltFunction::kAlt3);  // SPI5_CE0
  rpl::Gpio::SetAltFunction(13, rpl::Gpio::AltFunction::kAlt3);  // SPI5_MISO
  rpl::Gpio::SetAltFunction(14, rpl::Gpio::AltFunction::kAlt3);  // SPI5_MOSI
  rpl::Gpio::SetAltFunction(15, rpl::Gpio::AltFunction::kAlt3);  // SPI5_SCLK

  // SPI5_CE0
  rpl::Gpio::SetPullRegister(12, rpl::Gpio::PullRegister::kNoRegister);
  // SPI5_MISO
  rpl::Gpio::SetPullRegister(13, rpl::Gpio::PullRegister::kPullDown);
  // SPI5_MOSI
  rpl::Gpio::SetPullRegister(14, rpl::Gpio::PullRegister::kPullDown);
  // SPI5_SCLK
  rpl::Gpio::SetPullRegister(15, rpl::Gpio::PullRegister::kNoRegister);

  spi0->SetClockPhase(rpl::Spi::ClockPhase::kBeginning);
  spi0->SetClockPolarity(rpl::Spi::ClockPolarity::kLow);
  spi0->SetClockDivider(4096);
  spi0->SetCs0Polarity(rpl::Spi::CsPolarity::kLow);
  spi0->SetReadEnable(rpl::Spi::ReadEnable::kDisable);

  spi3->SetClockPhase(rpl::Spi::ClockPhase::kBeginning);
  spi3->SetClockPolarity(rpl::Spi::ClockPolarity::kLow);
  spi3->SetClockDivider(4096);
  spi3->SetCs0Polarity(rpl::Spi::CsPolarity::kLow);
  spi3->SetReadEnable(rpl::Spi::ReadEnable::kDisable);

  spi4->SetClockPhase(rpl::Spi::ClockPhase::kBeginning);
  spi4->SetClockPolarity(rpl::Spi::ClockPolarity::kLow);
  spi4->SetClockDivider(4096);
  spi4->SetCs0Polarity(rpl::Spi::CsPolarity::kLow);
  spi4->SetReadEnable(rpl::Spi::ReadEnable::kDisable);

  spi5->SetClockPhase(rpl::Spi::ClockPhase::kBeginning);
  spi5->SetClockPolarity(rpl::Spi::ClockPolarity::kLow);
  spi5->SetClockDivider(4096);
  spi5->SetCs0Polarity(rpl::Spi::CsPolarity::kLow);
  spi5->SetReadEnable(rpl::Spi::ReadEnable::kDisable);

  int i = 0;
  while (true) {
    uint8_t tx_buf0[13] = "hello world\n";
    uint8_t tx_buf3[13] = "HELLO WORLD\n";
    uint8_t tx_buf4[13] = "dlrow olleh\n";
    uint8_t tx_buf5[13] = "DLROW OLLEH\n";
    uint8_t rx_buf0[13];
    uint8_t rx_buf3[13];
    uint8_t rx_buf4[13];
    uint8_t rx_buf5[13];
    spi0->SetChipSelectForCommunication(rpl::Spi::ChipSelect::kChipSelect0);
    spi3->SetChipSelectForCommunication(rpl::Spi::ChipSelect::kChipSelect0);
    spi4->SetChipSelectForCommunication(rpl::Spi::ChipSelect::kChipSelect0);
    spi5->SetChipSelectForCommunication(rpl::Spi::ChipSelect::kChipSelect0);
    spi0->StartTransmission();
    spi3->StartTransmission();
    spi4->StartTransmission();
    spi5->StartTransmission();
    for (uint32_t i = 0; i < 13; ++i) {
      spi0->ClearTxAndRxFifo();
      spi3->ClearTxAndRxFifo();
      spi4->ClearTxAndRxFifo();
      spi5->ClearTxAndRxFifo();
      while (!spi0->IsTxFifoWritable() && !spi3->IsTxFifoWritable() &&
             !spi4->IsTxFifoWritable() && !spi5->IsTxFifoWritable()) {}
      spi0->WriteDataToTxFifo(static_cast<uint32_t>(tx_buf0[i]));
      spi3->WriteDataToTxFifo(static_cast<uint32_t>(tx_buf3[i]));
      spi4->WriteDataToTxFifo(static_cast<uint32_t>(tx_buf4[i]));
      spi5->WriteDataToTxFifo(static_cast<uint32_t>(tx_buf5[i]));
      while (!spi0->IsTransmissionCompleted() &&
             !spi3->IsTransmissionCompleted() &&
             !spi4->IsTransmissionCompleted() &&
             !spi5->IsTransmissionCompleted()) {}
      while (!spi0->IsRxFifoReadable() && !spi3->IsRxFifoReadable() &&
             !spi4->IsRxFifoReadable() && !spi5->IsRxFifoReadable()) {}
      rx_buf0[i] = static_cast<uint8_t>(spi0->ReadDataFromRxFifo());
      rx_buf3[i] = static_cast<uint8_t>(spi3->ReadDataFromRxFifo());
      rx_buf4[i] = static_cast<uint8_t>(spi4->ReadDataFromRxFifo());
      rx_buf5[i] = static_cast<uint8_t>(spi5->ReadDataFromRxFifo());
    }
    spi0->EndTransmission();
    spi3->EndTransmission();
    spi4->EndTransmission();
    spi5->EndTransmission();

    std::cout << i++ << std::endl;
    std::cout << rx_buf0;
    std::cout << rx_buf3;
    std::cout << rx_buf4;
    std::cout << rx_buf5;

    std::this_thread::sleep_for(100ms);
  }

  return 0;
}