#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "rpl4/peripheral/dma.hpp"
#include "rpl4/peripheral/pwm.hpp"
#include "rpl4/rpl4.hpp"
#include "rpl4/system/dma_memory.hpp"

int main(void) {
  rpl::Init();

  std::cout << "PWM with DMA Example - Generate varying PWM pattern"
            << std::endl;

  // Configure GPIO pin 18 for PWM
  if (!rpl::Pwm::ConfigureGpioPin(18)) {
    std::cerr << "Failed to configure GPIO pin" << std::endl;
    return 1;
  }

  // Get PWM and DMA instances
  auto pwm = rpl::Pwm::GetInstance(rpl::Pwm::Port::kPwm0);
  auto dma = rpl::Dma::GetInstance(rpl::Dma::Channel::kChannel5);
  if (pwm == nullptr || dma == nullptr) {
    std::cerr << "Failed to get PWM or DMA instance" << std::endl;
    return 1;
  }

  // Get DMA memory allocator
  auto& dma_memory = rpl::DmaMemory::GetInstance();

  // Set up PWM
  constexpr double kPwmFrequency = 1000.0;  // 1 kHz
  pwm->InitializeClock(25000000);  // 25 MHz clock
  pwm->SetFrequency(rpl::Pwm::Channel::kChannel1, kPwmFrequency);
  pwm->SetMSMode(rpl::Pwm::Channel::kChannel1, true);

  // Enable FIFO for PWM
  pwm->EnableFifo(rpl::Pwm::Channel::kChannel1);
  pwm->ClearFifo();

  // Create a pattern of PWM duty cycles (sine wave)
  constexpr size_t kPatternSize = 1000;
  uint32_t* pattern_buffer = static_cast<uint32_t*>(
      dma_memory.Allocate(kPatternSize * sizeof(uint32_t)));

  if (pattern_buffer == nullptr) {
    std::cerr << "Failed to allocate pattern buffer" << std::endl;
    return 1;
  }

  // Generate sine wave pattern
  uint32_t range = pwm->GetRange(rpl::Pwm::Channel::kChannel1);
  for (size_t i = 0; i < kPatternSize; i++) {
    double angle = 2.0 * M_PI * i / kPatternSize;
    double duty = 0.5 + 0.4 * std::sin(angle);  // 10% to 90% duty cycle
    pattern_buffer[i] = static_cast<uint32_t>(range * duty);
  }

  std::cout << "Generated " << kPatternSize << " PWM values" << std::endl;
  std::cout << "Pattern range: " << pattern_buffer[0] << " to "
            << pattern_buffer[kPatternSize / 4] << std::endl;

  // Create DMA control blocks for circular buffer
  auto* control_blocks = static_cast<rpl::DmaControlBlock*>(
      dma_memory.Allocate(sizeof(rpl::DmaControlBlock)));

  if (control_blocks == nullptr) {
    std::cerr << "Failed to allocate control blocks" << std::endl;
    dma_memory.Free(pattern_buffer);
    return 1;
  }

  // Get physical addresses
  uint32_t pattern_physical = dma_memory.GetPhysicalAddress(pattern_buffer);
  uint32_t cb_physical = dma_memory.GetPhysicalAddress(control_blocks);
  uint32_t pwm_fifo_physical = pwm->GetFifoPhysicalAddress();

  // Configure control block
  rpl::Dma::ConfigureMemoryToPeripheral(
      control_blocks, pattern_physical, pwm_fifo_physical,
      (kPatternSize) * sizeof(uint32_t),
      rpl::DmaRegisterMap::TI::PERMAP::kPwm0);

  // Link to second control block (circular buffer)
  control_blocks->next_control_block = cb_physical;

  std::cout << "Control blocks configured at physical address: 0x" << std::hex
            << cb_physical << std::endl;
  std::cout << "PWM FIFO physical address: 0x" << std::hex
            << pwm_fifo_physical << std::endl;

  // Enable DMA on PWM
  pwm->EnableDma(7, 7);

  // Enable and start DMA
  dma->Enable();
  dma->Reset();
  dma->SetPriority(8);
  dma->SetControlBlockAddress(cb_physical);

  // Enable PWM
  pwm->Enable(rpl::Pwm::Channel::kChannel1);

  std::cout << "Starting PWM with DMA..." << std::endl;
  std::cout << "The PWM output will cycle through the sine wave pattern."
            << std::endl;
  std::cout << "Running for 5 seconds..." << std::endl;

  dma->Start();

  // Run for 5 seconds
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(5000ms);

  std::cout << "Stopping..." << std::endl;

  // Stop DMA and PWM
  dma->Abort();
  pwm->Disable(rpl::Pwm::Channel::kChannel1);
  pwm->DisableDma();
  dma->Disable();

  // Clean up
  dma_memory.Free(control_blocks);
  dma_memory.Free(pattern_buffer);

  std::cout << "Example completed successfully!" << std::endl;

  return 0;
}
