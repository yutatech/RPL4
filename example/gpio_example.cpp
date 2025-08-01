#include <chrono>
#include <memory>
#include <thread>

#include "rpl4/peripheral/gpio.hpp"
#include "rpl4/rpl4.hpp"

std::shared_ptr<rpl::Gpio> gpio = rpl::Gpio::GetInstance(4);  // GPIO pin 4

int main(void) {
  rpl::Init();

  gpio->SetAltFunction(rpl::Gpio::AltFunction::kOutput);
  gpio->SetPullRegister(rpl::Gpio::PullRegister::kNoRegister);

  using namespace std::chrono_literals;
  while (true) {
    gpio->Write(true);
    std::this_thread::sleep_for(1000ms);
    gpio->Write(false);
    std::this_thread::sleep_for(1000ms);
  }

  return 0;
}