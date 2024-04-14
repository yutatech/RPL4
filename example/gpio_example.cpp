#include "rpl4/rpl4.hpp"
#include "rpl4/peripheral/GPIO.hpp"

#include <thread>
#include <chrono>

GPIO gpio(12);

int main(void) {
  rpl::Init();

  gpio.SetPinMode(PinMode::output);

  using namespace std::chrono_literals;
  while(true) {
    gpio = true;
    std::this_thread::sleep_for(1000ms);
    gpio = false;
    std::this_thread::sleep_for(1000ms);
  }

  return 0;
}