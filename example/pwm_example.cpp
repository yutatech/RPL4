#include "rpl4/rpl4.hpp"
#include "rpl4/peripheral/PWM.hpp"

#include <thread>
#include <chrono>

PWM pwm(18);

int main(void) {
  rpl::Init();

  pwm.Init();
  pwm.SetFrequency(38000);

  pwm = 0.5;

  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1000ms);

  while(true) {
    for(double duty = 0; duty <= 1; duty +=0.05) {
      pwm = duty;
      std::this_thread::sleep_for(10ms);
    }
    for(double duty = 1; duty >= 0; duty -=0.05) {
      pwm = duty;
      std::this_thread::sleep_for(10ms);
    }
  }

  return 0;
}