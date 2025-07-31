#include "rpl4/peripheral/gpio.hpp"

#include "rpl4/registers/registers_gpio.hpp"
#include "rpl4/system/system.hpp"

namespace rpl {

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
    instances_[static_cast<size_t>(pin)] = std::shared_ptr<Gpio>(new Gpio(pin));
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
  if (pin_ <= 9) {
    REG_GPIO->gpfsel0 &= ~(0b111 << pin_ * 3);
    REG_GPIO->gpfsel0 |= static_cast<uint32_t>(alt_function) << pin_ * 3;
  } else if (10 <= pin_ && pin_ <= 19) {
    REG_GPIO->gpfsel1 &= ~(0b111 << (pin_ - 10) * 3);
    REG_GPIO->gpfsel1 |= static_cast<uint32_t>(alt_function) << (pin_ - 10) * 3;
  } else if (20 <= pin_ && pin_ <= 29) {
    REG_GPIO->gpfsel2 &= ~(0b111 << (pin_ - 20) * 3);
    REG_GPIO->gpfsel2 |= static_cast<uint32_t>(alt_function) << (pin_ - 20) * 3;
  } else if (30 <= pin_ && pin_ <= 39) {
    REG_GPIO->gpfsel3 &= ~(0b111 << (pin_ - 30) * 3);
    REG_GPIO->gpfsel3 |= static_cast<uint32_t>(alt_function) << (pin_ - 30) * 3;
  } else if (40 <= pin_ && pin_ <= 49) {
    REG_GPIO->gpfsel4 &= ~(0b111 << (pin_ - 40) * 3);
    REG_GPIO->gpfsel4 |= static_cast<uint32_t>(alt_function) << (pin_ - 40) * 3;
  } else if (50 <= pin_ && pin_ <= 57) {
    REG_GPIO->gpfsel5 &= ~(0b111 << (pin_ - 50) * 3);
    REG_GPIO->gpfsel5 |= static_cast<uint32_t>(alt_function) << (pin_ - 50) * 3;
  }
}

void Gpio::SetPullRegister(PullRegister pull_register) {
  if (pin_ <= 15) {
    REG_GPIO->pup_pdn_cntrl_reg0 &= ~(0b11 << pin_ * 2);
    REG_GPIO->pup_pdn_cntrl_reg0 |= static_cast<uint8_t>(pull_register)
                                    << pin_ * 2;
  } else if (16 <= pin_ && pin_ <= 31) {
    REG_GPIO->pup_pdn_cntrl_reg1 &= ~(0b11 << (pin_ - 16) * 2);
    REG_GPIO->pup_pdn_cntrl_reg1 |= static_cast<uint8_t>(pull_register)
                                    << (pin_ - 16) * 2;
  } else if (32 <= pin_ && pin_ <= 47) {
    REG_GPIO->pup_pdn_cntrl_reg2 &= ~(0b11 << (pin_ - 32) * 2);
    REG_GPIO->pup_pdn_cntrl_reg2 |= static_cast<uint8_t>(pull_register)
                                    << (pin_ - 32) * 2;
  } else if (48 <= pin_ && pin_ <= 57) {
    REG_GPIO->pup_pdn_cntrl_reg3 &= ~(0b11 << (pin_ - 48) * 2);
    REG_GPIO->pup_pdn_cntrl_reg3 |= static_cast<uint8_t>(pull_register)
                                    << (pin_ - 48) * 2;
  }
}

void Gpio::SetAltFunction(uint8_t pin, AltFunction alt_function) {
  if (!IsInitialized()) {
    Log(LogLevel::Error, "[Gpio::SetAltFunction()] RPL is not initialized.");
    return;
  }
  if (pin <= 9) {
    REG_GPIO->gpfsel0 &= ~(0b111 << pin * 3);
    REG_GPIO->gpfsel0 |= static_cast<uint32_t>(alt_function) << pin * 3;
  } else if (10 <= pin && pin <= 19) {
    REG_GPIO->gpfsel1 &= ~(0b111 << (pin - 10) * 3);
    REG_GPIO->gpfsel1 |= static_cast<uint32_t>(alt_function) << (pin - 10) * 3;
  } else if (20 <= pin && pin <= 29) {
    REG_GPIO->gpfsel2 &= ~(0b111 << (pin - 20) * 3);
    REG_GPIO->gpfsel2 |= static_cast<uint32_t>(alt_function) << (pin - 20) * 3;
  } else if (30 <= pin && pin <= 39) {
    REG_GPIO->gpfsel3 &= ~(0b111 << (pin - 30) * 3);
    REG_GPIO->gpfsel3 |= static_cast<uint32_t>(alt_function) << (pin - 30) * 3;
  } else if (40 <= pin && pin <= 49) {
    REG_GPIO->gpfsel4 &= ~(0b111 << (pin - 40) * 3);
    REG_GPIO->gpfsel4 |= static_cast<uint32_t>(alt_function) << (pin - 40) * 3;
  } else if (50 <= pin && pin <= 57) {
    REG_GPIO->gpfsel5 &= ~(0b111 << (pin - 50) * 3);
    REG_GPIO->gpfsel5 |= static_cast<uint32_t>(alt_function) << (pin - 50) * 3;
  } else {
    if (pin > 57) Log(LogLevel::Error, "[Gpio]GPIO %d is not exists.\n", pin);
  }
}

void Gpio::SetPullRegister(uint8_t pin, PullRegister pull_register) {
  if (!IsInitialized()) {
    Log(LogLevel::Error, "[Gpio::SetPullRegister ()] RPL is not initialized.");
    return;
  }
  if (pin <= 15) {
    REG_GPIO->pup_pdn_cntrl_reg0 &= ~(0b11 << pin * 2);
    REG_GPIO->pup_pdn_cntrl_reg0 |= static_cast<uint8_t>(pull_register)
                                    << pin * 2;
  } else if (16 <= pin && pin <= 31) {
    REG_GPIO->pup_pdn_cntrl_reg1 &= ~(0b11 << (pin - 16) * 2);
    REG_GPIO->pup_pdn_cntrl_reg1 |= static_cast<uint8_t>(pull_register)
                                    << (pin - 16) * 2;
  } else if (32 <= pin && pin <= 47) {
    REG_GPIO->pup_pdn_cntrl_reg2 &= ~(0b11 << (pin - 32) * 2);
    REG_GPIO->pup_pdn_cntrl_reg2 |= static_cast<uint8_t>(pull_register)
                                    << (pin - 32) * 2;
  } else if (48 <= pin && pin <= 57) {
    REG_GPIO->pup_pdn_cntrl_reg3 &= ~(0b11 << (pin - 48) * 2);
    REG_GPIO->pup_pdn_cntrl_reg3 |= static_cast<uint8_t>(pull_register)
                                    << (pin - 48) * 2;
  } else {
    if (pin > 57) Log(LogLevel::Error, "[Gpio]GPIO %d is not exists.\n", pin);
  }
}

}  // namespace rpl