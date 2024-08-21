#include <chrono>
#include <memory>
#include <thread>

#include "rpl4/peripheral/spi.hpp"
#include "rpl4/rpl4.hpp"

int main(void) {
  rpl::Init();
  using namespace std::chrono_literals;

  std::shared_ptr<rpl::Spi> spi = rpl::Spi::GetInstance(rpl::Spi::Port::kSpi0);
  spi->SetClockPhase(rpl::SpiRegisterMap::CS::CPHA::kBeginning);
  spi->SetClockPolarity(rpl::SpiRegisterMap::CS::CPOL::kHigh);
  spi->SetClockDivider(256);

  spi->SetChipSelectForCommunication(rpl::SpiRegisterMap::CS::CS_::kChipSelect0);

  while (true) {
    uint8_t tx_buf[13] = "Hello World\n";
    uint8_t rx_buf[12];
    spi->TransmitAndReceiveBlocking(tx_buf, rx_buf, 12);

    std::this_thread::sleep_for(1000ms);
  }

  return 0;
}