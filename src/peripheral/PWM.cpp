#include "rpl4/peripheral/PWM.hpp"

#include <unistd.h>

#include "rpl4/peripheral/gpio.hpp"
#include "rpl4/system/clock.hpp"
#include "rpl4/system/system.hpp"

PWM::PWM(uint8_t _pin) : pin(_pin) {
  switch (pin) {
    case 12:
    case 13:
    case 18:
    case 19:
    case 40:
    case 41:
    case 45:
      break;
    default:
      rpl::Log(rpl::LogLevel::Error, "GPIO %d has no PWM function\n", pin);
      return;
  }
};

uint8_t PWM::Init() {
  switch (pin) {
    case 12:
    case 13:
    case 40:
    case 41:
    case 45:
      rpl::Gpio::SetAltFunction(pin, rpl::Gpio::AltFunction::kAlt0);
      break;
    case 18:
    case 19:
      rpl::Gpio::SetAltFunction(pin, rpl::Gpio::AltFunction::kAlt5);
      break;
  }

  rpl::ClockConfig(rpl::REG_CLK->CM_PWMCTL, rpl::REG_CLK->CM_PWMDIV,
                   rpl::ClockSource::kOsc, 2.16,
                   1);  // set pwm source clock frequency to 25MHz
  return 0;
}

double PWM::Write(double duty) {
  uint32_t dat = clock_source * duty / freq;

  switch (pin) {
    case 12:
    case 18:
      rpl::REG_PWM0->dat1.data = dat;
      rpl::REG_PWM0->ctl.pwen1 = rpl::PwmRegisterMap::CTL::PWEN::kEnable;
      break;
    case 13:
    case 19:
    case 45:
      rpl::REG_PWM0->dat2.data = dat;
      rpl::REG_PWM0->ctl.pwen2 = rpl::PwmRegisterMap::CTL::PWEN::kEnable;
      break;
    case 40:
      rpl::REG_PWM1->dat1.data = dat;
      rpl::REG_PWM1->ctl.pwen1 = rpl::PwmRegisterMap::CTL::PWEN::kEnable;
      break;
    case 41:
      rpl::REG_PWM1->dat2.data = dat;
      rpl::REG_PWM1->ctl.pwen2 = rpl::PwmRegisterMap::CTL::PWEN::kEnable;
      break;
  }
  return duty;
}

double PWM::SetFrequency(double _freq) {
  freq = _freq;
  uint32_t rng = clock_source / freq;

  switch (pin) {
    case 12:
    case 18:
      rpl::REG_PWM0->ctl.pwen1 = rpl::PwmRegisterMap::CTL::PWEN::kDisable;
      rpl::REG_PWM0->rng1.range = rng;
      rpl::REG_PWM0->dat1.data = 0;
      rpl::REG_PWM0->ctl.msen1 = rpl::PwmRegisterMap::CTL::MSEN::kMSRatio;
      rpl::REG_PWM0->ctl.pwen1 = rpl::PwmRegisterMap::CTL::PWEN::kEnable;
      break;
    case 13:
    case 19:
    case 45:
      rpl::REG_PWM0->ctl.pwen2 = rpl::PwmRegisterMap::CTL::PWEN::kDisable;
      rpl::REG_PWM0->rng2.range = rng;
      rpl::REG_PWM0->dat2.data = 0;
      rpl::REG_PWM0->ctl.msen2 = rpl::PwmRegisterMap::CTL::MSEN::kMSRatio;
      rpl::REG_PWM0->ctl.pwen2 = rpl::PwmRegisterMap::CTL::PWEN::kEnable;
      break;
    case 40:
      rpl::REG_PWM1->ctl.pwen1 = rpl::PwmRegisterMap::CTL::PWEN::kDisable;
      rpl::REG_PWM1->rng1.range = rng;
      rpl::REG_PWM1->dat1.data = 0;
      rpl::REG_PWM1->ctl.msen1 = rpl::PwmRegisterMap::CTL::MSEN::kMSRatio;
      rpl::REG_PWM1->ctl.pwen1 = rpl::PwmRegisterMap::CTL::PWEN::kEnable;
      break;
    case 41:
      rpl::REG_PWM1->ctl.pwen2 = rpl::PwmRegisterMap::CTL::PWEN::kDisable;
      rpl::REG_PWM1->rng2.range = rng;
      rpl::REG_PWM1->dat2.data = 0;
      rpl::REG_PWM1->ctl.msen2 = rpl::PwmRegisterMap::CTL::MSEN::kMSRatio;
      rpl::REG_PWM1->ctl.pwen2 = rpl::PwmRegisterMap::CTL::PWEN::kEnable;
      break;
  }
  return freq;
}