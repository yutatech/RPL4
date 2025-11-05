#include "rpl4/peripheral/pwm.hpp"

#include "rpl4/peripheral/gpio.hpp"
#include "rpl4/system/clock.hpp"
#include "rpl4/system/system.hpp"

namespace rpl {

std::shared_ptr<Pwm> PwmFactory::Create(PwmRegisterMap* register_map, Pwm::Port port) {
  struct EnableMakeShared : public Pwm {
    EnableMakeShared(PwmRegisterMap* reg_map, Pwm::Port p) : Pwm(reg_map, p) {}
  };
  return std::make_shared<EnableMakeShared>(register_map, port);
}

std::array<std::shared_ptr<Pwm>, Pwm::kNumOfInstances> Pwm::instances_ = {
    nullptr};

std::shared_ptr<Pwm> Pwm::GetInstance(Port port) {
  size_t index = static_cast<size_t>(port);
  if (index >= kNumOfInstances) {
    Log(LogLevel::Fatal, "[Pwm::GetInstance()] Invalid port %zu.", index);
    return nullptr;
  }

  if (!IsInitialized()) {
    Log(LogLevel::Error, "[Pwm::GetInstance()] RPL is not initialized.");
  } else if (instances_[index] == nullptr) {
    PwmRegisterMap* reg_map = nullptr;
    switch (port) {
      case Port::kPwm0:
        reg_map = REG_PWM0;
        break;
      case Port::kPwm1:
        reg_map = REG_PWM1;
        break;
    }
    instances_[index] = PwmFactory::Create(reg_map, port);
  }
  return instances_[index];
}

Pwm::Pwm(PwmRegisterMap* register_map, Port port)
    : register_map_(register_map),
      port_(port),
      clock_frequency_(kDefaultClockFrequency) {
  // Initialize PWM clock to default frequency
  InitializeClock(kDefaultClockFrequency);
}

bool Pwm::ConfigureGpioPin(uint8_t pin) {
  switch (pin) {
    case 12:
    case 13:
    case 40:
    case 41:
    case 45:
      Gpio::SetAltFunction(pin, Gpio::AltFunction::kAlt0);
      return true;
    case 18:
    case 19:
      Gpio::SetAltFunction(pin, Gpio::AltFunction::kAlt5);
      return true;
    default:
      Log(LogLevel::Error,
          "[Pwm::ConfigureGpioPin] GPIO %d has no PWM function", pin);
      return false;
  }
}

void Pwm::InitializeClock(double frequency) {
  // Set PWM source clock frequency
  // Using oscillator (54MHz) as source, divided to achieve target frequency
  double divisor = 54000000.0 / frequency;
  ClockConfig(REG_CLK->CM_PWMCTL, REG_CLK->CM_PWMDIV, ClockSource::kOsc,
              divisor, 1);
  clock_frequency_ = frequency;
}

void Pwm::Enable(Channel channel) {
  if (channel == Channel::kChannel1) {
    register_map_->ctl.pwen1 = PwmRegisterMap::CTL::PWEN::kEnable;
  } else if (channel == Channel::kChannel2) {
    register_map_->ctl.pwen2 = PwmRegisterMap::CTL::PWEN::kEnable;
  }
}

void Pwm::Disable(Channel channel) {
  if (channel == Channel::kChannel1) {
    register_map_->ctl.pwen1 = PwmRegisterMap::CTL::PWEN::kDisable;
  } else if (channel == Channel::kChannel2) {
    register_map_->ctl.pwen2 = PwmRegisterMap::CTL::PWEN::kDisable;
  }
}

void Pwm::SetFrequency(Channel channel, double frequency) {
  uint32_t range = static_cast<uint32_t>(clock_frequency_ / frequency);
  SetRange(channel, range);
}

void Pwm::SetDutyCycle(Channel channel, double duty) {
  if (duty < 0.0) duty = 0.0;
  if (duty > 1.0) duty = 1.0;

  uint32_t range = GetRange(channel);
  uint32_t data = static_cast<uint32_t>(range * duty);
  SetData(channel, data);
}

void Pwm::SetRange(Channel channel, uint32_t range) {
  if (channel == Channel::kChannel1) {
    register_map_->rng1.range = range;
  } else if (channel == Channel::kChannel2) {
    register_map_->rng2.range = range;
  }
}

void Pwm::SetData(Channel channel, uint32_t data) {
  if (channel == Channel::kChannel1) {
    register_map_->dat1.data = data;
  } else if (channel == Channel::kChannel2) {
    register_map_->dat2.data = data;
  }
}

uint32_t Pwm::GetRange(Channel channel) {
  if (channel == Channel::kChannel1) {
    return register_map_->rng1.range;
  } else if (channel == Channel::kChannel2) {
    return register_map_->rng2.range;
  }
  return 0;
}

uint32_t Pwm::GetData(Channel channel) {
  if (channel == Channel::kChannel1) {
    return register_map_->dat1.data;
  } else if (channel == Channel::kChannel2) {
    return register_map_->dat2.data;
  }
  return 0;
}

void Pwm::SetMode(Channel channel, PwmRegisterMap::CTL::MODE mode) {
  if (channel == Channel::kChannel1) {
    register_map_->ctl.mode1 = mode;
  } else if (channel == Channel::kChannel2) {
    register_map_->ctl.mode2 = mode;
  }
}

void Pwm::SetPolarity(Channel channel, PwmRegisterMap::CTL::POLA polarity) {
  if (channel == Channel::kChannel1) {
    register_map_->ctl.pola1 = polarity;
  } else if (channel == Channel::kChannel2) {
    register_map_->ctl.pola2 = polarity;
  }
}

void Pwm::SetMSMode(Channel channel, bool enable) {
  PwmRegisterMap::CTL::MSEN msen = enable
                                       ? PwmRegisterMap::CTL::MSEN::kMSRatio
                                       : PwmRegisterMap::CTL::MSEN::kPwmAlgorithm;
  if (channel == Channel::kChannel1) {
    register_map_->ctl.msen1 = msen;
  } else if (channel == Channel::kChannel2) {
    register_map_->ctl.msen2 = msen;
  }
}

void Pwm::EnableFifo(Channel channel) {
  if (channel == Channel::kChannel1) {
    register_map_->ctl.usef1 = PwmRegisterMap::CTL::USEF::kFifo;
  } else if (channel == Channel::kChannel2) {
    register_map_->ctl.usef2 = PwmRegisterMap::CTL::USEF::kFifo;
  }
}

void Pwm::DisableFifo(Channel channel) {
  if (channel == Channel::kChannel1) {
    register_map_->ctl.usef1 = PwmRegisterMap::CTL::USEF::kData;
  } else if (channel == Channel::kChannel2) {
    register_map_->ctl.usef2 = PwmRegisterMap::CTL::USEF::kData;
  }
}

void Pwm::ClearFifo() {
  register_map_->ctl.clrf1 = PwmRegisterMap::CTL::CLRF::kClear;
}

void Pwm::WriteFifo(uint32_t data) { register_map_->fif1.data = data; }

bool Pwm::IsFifoFull() {
  return register_map_->sta.full1 == PwmRegisterMap::STA::FULL::kFull;
}

bool Pwm::IsFifoEmpty() {
  return register_map_->sta.empt1 == PwmRegisterMap::STA::EMPT::kEmpty;
}

void Pwm::EnableDma(uint8_t dreq_threshold, uint8_t panic_threshold) {
  if (dreq_threshold > 15) dreq_threshold = 15;
  if (panic_threshold > 15) panic_threshold = 15;

  register_map_->dmac.dreq =
      static_cast<PwmRegisterMap::DMAC::DREQ>(dreq_threshold);
  register_map_->dmac.panic =
      static_cast<PwmRegisterMap::DMAC::PANIC>(panic_threshold);
  register_map_->dmac.enab = PwmRegisterMap::DMAC::ENAB::kEnable;
}

void Pwm::DisableDma() {
  register_map_->dmac.enab = PwmRegisterMap::DMAC::ENAB::kDisable;
}

uint32_t Pwm::GetFifoPhysicalAddress() const {
  // Calculate physical address of FIF1 register
  uint32_t base_physical;
  if (port_ == Port::kPwm0) {
    base_physical = kPwm0AddressBase - 0x80000000;
  } else {
    base_physical = kPwm1AddressBase- 0x80000000;
  }
  // FIF1 is at offset 0x18
  return base_physical + 0x18;
}

}  // namespace rpl
