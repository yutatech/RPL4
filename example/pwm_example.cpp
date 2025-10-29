#include "rpl4/rpl4.hpp"
#include "rpl4/peripheral/pwm.hpp"

#include <thread>
#include <chrono>
#include <iostream>

int main(void) {
  rpl::Init();

  std::cout << "PWM Example - simple PWM output on GPIO18" << std::endl;

  // Configure GPIO pin 18 for PWM output
  if (!rpl::Pwm::ConfigureGpioPin(18)) {
    std::cerr << "Failed to configure GPIO pin 18 for PWM" << std::endl;
    return 1;
  }

  // Get PWM instance for PWM0
  auto pwm = rpl::Pwm::GetInstance(rpl::Pwm::Port::kPwm0);
  if (pwm == nullptr) {
    std::cerr << "Failed to obtain PWM instance" << std::endl;
    return 1;
  }

  // Initialize clock and set frequency for channel 1
  pwm->InitializeClock(25000000);  // 25 MHz clock
  constexpr double kFrequency = 38000.0;  // 38 kHz
  pwm->SetFrequency(rpl::Pwm::Channel::kChannel1, kFrequency);

  // Use M/S mode for stable duty control
  pwm->SetMSMode(rpl::Pwm::Channel::kChannel1, true);
  pwm->DisableFifo(rpl::Pwm::Channel::kChannel1);

  // Enable PWM channel
  pwm->Enable(rpl::Pwm::Channel::kChannel1);

  using namespace std::chrono_literals;

  // Ramp duty up and down
  while (true) {
    for (double duty = 0.0; duty <= 1.0; duty += 0.05) {
      pwm->SetDutyCycle(rpl::Pwm::Channel::kChannel1, duty);
      std::this_thread::sleep_for(10ms);
    }
    for (double duty = 1.0; duty >= 0.0; duty -= 0.05) {
      pwm->SetDutyCycle(rpl::Pwm::Channel::kChannel1, duty);
      std::this_thread::sleep_for(10ms);
    }
  }

  return 0;
}