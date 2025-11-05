#include "rpl4/peripheral/gpio.hpp"

#include "rpl4/registers/registers_gpio.hpp"
#include "rpl4/system/system.hpp"

namespace rpl {

std::shared_ptr<Gpio> GpioFactory::Create(uint8_t pin) {
  struct EnableMakeShared : public Gpio {
    explicit EnableMakeShared(uint8_t p) : Gpio(p) {}
  };
  return std::make_shared<EnableMakeShared>(pin);
}

std::array<std::shared_ptr<Gpio>, Gpio::kNumOfInstances> Gpio::instances_ = {
    nullptr};

std::shared_ptr<Gpio> Gpio::GetInstance(uint8_t pin) {
  if (pin >= kNumOfInstances) {
    Log(LogLevel::Fatal, "[Gpio::GetInstance()] Invalid pin number %d.", pin);
    return nullptr;
  }

  if (!IsInitialized()) {
    Log(LogLevel::Error, "[Gpio::GetInstance()] RPL is not initialized.");
  } else if (instances_[static_cast<size_t>(pin)] == nullptr) {
    instances_[static_cast<size_t>(pin)] = GpioFactory::Create(pin);
  }
  return instances_[static_cast<size_t>(pin)];
}

Gpio::Gpio(uint8_t pin) : pin_(pin) {
  if (pin_ > 57) Log(LogLevel::Fatal, "[Gpio]GPIO %d is not exists.\n", pin);
}

bool Gpio::Read() {
  if (pin_ <= 31) {
    return (REG_GPIO->gplev0 & (0b1 << pin_)) ==
           static_cast<uint32_t>(0b1 << pin_);
  } else if (32 <= pin_ && pin_ <= 57) {
    return (REG_GPIO->gplev1 & (0b1 << (pin_ - 32))) ==
           static_cast<uint32_t>(0b1 << (pin_ - 32));
  } else {
    Log(LogLevel::Error, "[Gpio::Read()] Invalid pin_ number %d is set.", pin_);
    return false;
  }
}

bool Gpio::Write(bool output) {
  if (output) {
    if (pin_ <= 31)
      REG_GPIO->gpset0 = 0b1 << pin_;
    else if (32 <= pin_ && pin_ <= 57)
      REG_GPIO->gpset1 = 0b1 << (pin_ - 32);
  } else {
    if (pin_ <= 31)
      REG_GPIO->gpclr0 = 0b1 << pin_;
    else if (32 <= pin_ && pin_ <= 57)
      REG_GPIO->gpclr1 = 0b1 << (pin_ - 32);
  }

  return output;
}

void Gpio::SetAltFunction(AltFunction alt_function) {
  SetAltFunction(pin_, alt_function);
}

void Gpio::SetPullRegister(PullRegister pull_register) {
  SetPullRegister(pin_, pull_register);
}

void Gpio::SetAltFunction(uint8_t pin, AltFunction alt_function) {
  if (!IsInitialized()) {
    Log(LogLevel::Error, "[Gpio::SetAltFunction()] RPL is not initialized.");
    return;
  }
  if (pin <= 9) {
    uint32_t val = REG_GPIO->gpfsel0;
    val &= ~(0b111 << pin * 3);
    val |= static_cast<uint32_t>(alt_function) << pin * 3;
    REG_GPIO->gpfsel0 = val;
  } else if (10 <= pin && pin <= 19) {
    uint32_t val = REG_GPIO->gpfsel1;
    val &= ~(0b111 << (pin - 10) * 3);
    val |= static_cast<uint32_t>(alt_function) << (pin - 10) * 3;
    REG_GPIO->gpfsel1 = val;
  } else if (20 <= pin && pin <= 29) {
    uint32_t val = REG_GPIO->gpfsel2;
    val &= ~(0b111 << (pin - 20) * 3);
    val |= static_cast<uint32_t>(alt_function) << (pin - 20) * 3;
    REG_GPIO->gpfsel2 = val;
  } else if (30 <= pin && pin <= 39) {
    uint32_t val = REG_GPIO->gpfsel3;
    val &= ~(0b111 << (pin - 30) * 3);
    val |= static_cast<uint32_t>(alt_function) << (pin - 30) * 3;
    REG_GPIO->gpfsel3 = val;
  } else if (40 <= pin && pin <= 49) {
    uint32_t val = REG_GPIO->gpfsel4;
    val &= ~(0b111 << (pin - 40) * 3);
    val |= static_cast<uint32_t>(alt_function) << (pin - 40) * 3;
    REG_GPIO->gpfsel4 = val;
  } else if (50 <= pin && pin <= 57) {
    uint32_t val = REG_GPIO->gpfsel5;
    val &= ~(0b111 << (pin - 50) * 3);
    val |= static_cast<uint32_t>(alt_function) << (pin - 50) * 3;
    REG_GPIO->gpfsel5 = val;
  } else {
    Log(LogLevel::Error, "[Gpio]GPIO %d is not exists.\n", pin);
  }
}

void Gpio::SetPullRegister(uint8_t pin, PullRegister pull_register) {
  if (!IsInitialized()) {
    Log(LogLevel::Error, "[Gpio::SetPullRegister ()] RPL is not initialized.");
    return;
  }
  if (pin <= 15) {
    uint32_t val = REG_GPIO->pup_pdn_cntrl_reg0;
    val &= ~(0b11 << pin * 2);
    val |= static_cast<uint8_t>(pull_register) << pin * 2;
    REG_GPIO->pup_pdn_cntrl_reg0 = val;
  } else if (16 <= pin && pin <= 31) {
    uint32_t val = REG_GPIO->pup_pdn_cntrl_reg1;
    val &= ~(0b11 << (pin - 16) * 2);
    val |= static_cast<uint8_t>(pull_register) << (pin - 16) * 2;
    REG_GPIO->pup_pdn_cntrl_reg1 = val;
  } else if (32 <= pin && pin <= 47) {
    uint32_t val = REG_GPIO->pup_pdn_cntrl_reg2;
    val &= ~(0b11 << (pin - 32) * 2);
    val |= static_cast<uint8_t>(pull_register) << (pin - 32) * 2;
    REG_GPIO->pup_pdn_cntrl_reg2 = val;
  } else if (48 <= pin && pin <= 57) {
    uint32_t val = REG_GPIO->pup_pdn_cntrl_reg3;
    val &= ~(0b11 << (pin - 48) * 2);
    val |= static_cast<uint8_t>(pull_register) << (pin - 48) * 2;
    REG_GPIO->pup_pdn_cntrl_reg3 = val;
  } else {
    Log(LogLevel::Error, "[Gpio]GPIO %d is not exists.\n", pin);
  }
}

}  // namespace rpl