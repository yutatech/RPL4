#include <chrono>
#include <thread>

#include "rpl4/peripheral/gpio.hpp"
#include "rpl4/rpl4.hpp"

rpl::Gpio gpio(12);

int main(void) {
  rpl::Init();

  gpio.SetAltFunction(rpl::Gpio::AltFunction::kOutput);
  gpio.SetPullRegister(rpl::Gpio::PullRegister::kNoRegister);

  using namespace std::chrono_literals;
  while (true) {
    gpio = true;
    std::this_thread::sleep_for(1000ms);
    gpio = false;
    std::this_thread::sleep_for(1000ms);
  }

  return 0;
}