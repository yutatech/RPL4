#include "rpl4/peripheral/dma.hpp"

#include <chrono>
#include <cstring>
#include <thread>

#include "rpl4/system/system.hpp"

namespace rpl {

std::array<std::shared_ptr<Dma>, Dma::kNumOfInstances> Dma::instances_ = {
    nullptr};

std::shared_ptr<Dma> Dma::GetInstance(Channel channel) {
  size_t index = static_cast<size_t>(channel);
  if (index >= kNumOfInstances) {
    Log(LogLevel::Fatal, "[Dma::GetInstance()] Invalid channel %zu.", index);
    return nullptr;
  }

  if (!IsInitialized()) {
    Log(LogLevel::Error, "[Dma::GetInstance()] RPL is not initialized.");
  } else if (instances_[index] == nullptr) {
    DmaRegisterMap* reg_map = nullptr;
    switch (channel) {
      case Channel::kChannel0:
        reg_map = REG_DMA0;
        break;
      case Channel::kChannel1:
        reg_map = REG_DMA1;
        break;
      case Channel::kChannel2:
        reg_map = REG_DMA2;
        break;
      case Channel::kChannel3:
        reg_map = REG_DMA3;
        break;
      case Channel::kChannel4:
        reg_map = REG_DMA4;
        break;
      case Channel::kChannel5:
        reg_map = REG_DMA5;
        break;
      case Channel::kChannel6:
        reg_map = REG_DMA6;
        break;
      case Channel::kChannel7:
        reg_map = REG_DMA7;
        break;
      case Channel::kChannel8:
        reg_map = REG_DMA8;
        break;
      case Channel::kChannel9:
        reg_map = REG_DMA9;
        break;
      case Channel::kChannel10:
        reg_map = REG_DMA10;
        break;
      case Channel::kChannel11:
        reg_map = REG_DMA11;
        break;
      case Channel::kChannel12:
        reg_map = REG_DMA12;
        break;
      case Channel::kChannel13:
        reg_map = REG_DMA13;
        break;
      case Channel::kChannel14:
        reg_map = REG_DMA14;
        break;
    }
    instances_[index] = std::shared_ptr<Dma>(new Dma(reg_map, channel));
  }
  return instances_[index];
}

Dma::Dma(DmaRegisterMap* register_map, Channel channel)
    : register_map_(register_map), channel_(channel) {
  Reset();
}

void Dma::Enable() {
  uint32_t channel_bit = 1 << static_cast<uint32_t>(channel_);
  REG_DMA_ENABLE->enable |= channel_bit;
}

void Dma::Disable() {
  uint32_t channel_bit = 1 << static_cast<uint32_t>(channel_);
  REG_DMA_ENABLE->enable &= ~channel_bit;
}

void Dma::Reset() {
  register_map_->cs.reset = DmaRegisterMap::CS::RESET::kReset;
  // Wait for reset to complete
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1ms);
}

void Dma::Abort() {
  register_map_->cs.abort = DmaRegisterMap::CS::ABORT::kAbort;
}

bool Dma::IsActive() {
  return register_map_->cs.active == DmaRegisterMap::CS::ACTIVE::kActive;
}

bool Dma::IsComplete() {
  return register_map_->cs.end == DmaRegisterMap::CS::END::kSet;
}

bool Dma::HasError() {
  return register_map_->cs.error == DmaRegisterMap::CS::ERROR::kError;
}

void Dma::ClearInterrupt() {
  register_map_->cs.interrupt = DmaRegisterMap::CS::INT::kSet;
}

void Dma::SetControlBlockAddress(uint32_t control_block_physical_addr) {
  register_map_->conblk_ad.address = control_block_physical_addr;
}

void Dma::Start() {
  register_map_->cs.active = DmaRegisterMap::CS::ACTIVE::kActive;
}

bool Dma::WaitForCompletion(uint32_t timeout_ms) {
  using namespace std::chrono_literals;
  auto start_time = std::chrono::steady_clock::now();

  while (!IsComplete()) {
    if (HasError()) {
      Log(LogLevel::Error, "[Dma] Transfer error on channel %d",
          static_cast<int>(channel_));
      return false;
    }

    if (timeout_ms > 0) {
      auto current_time = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                         current_time - start_time)
                         .count();
      if (elapsed >= timeout_ms) {
        Log(LogLevel::Warning, "[Dma] Transfer timeout on channel %d",
            static_cast<int>(channel_));
        return false;
      }
    }

    std::this_thread::sleep_for(10us);
  }

  return true;
}

void Dma::SetPriority(uint8_t priority) {
  if (priority > 15) {
    priority = 15;
  }
  register_map_->cs.priority =
      static_cast<DmaRegisterMap::CS::PRIORITY>(priority);
}

void Dma::SetPanicPriority(uint8_t panic_priority) {
  if (panic_priority > 15) {
    panic_priority = 15;
  }
  register_map_->cs.panic_priority =
      static_cast<DmaRegisterMap::CS::PANIC_PRIORITY>(panic_priority);
}

void Dma::ConfigureMemoryToMemory(DmaControlBlock* control_block,
                                  uint32_t src_physical,
                                  uint32_t dest_physical, uint32_t length) {
  if (control_block == nullptr) {
    return;
  }

  memset(control_block, 0, sizeof(DmaControlBlock));

  // Configure transfer info
  uint32_t ti = 0;
  ti |= static_cast<uint32_t>(DmaRegisterMap::TI::SRC_INC::kEnable) << 8;
  ti |= static_cast<uint32_t>(DmaRegisterMap::TI::DEST_INC::kEnable) << 4;
  ti |= static_cast<uint32_t>(DmaRegisterMap::TI::WAIT_RESP::kEnable) << 3;

  control_block->transfer_info = ti;
  control_block->source_addr = src_physical;
  control_block->dest_addr = dest_physical;
  control_block->transfer_length = length;
  control_block->stride = 0;
  control_block->next_control_block = 0;
}

void Dma::ConfigureMemoryToPeripheral(DmaControlBlock* control_block,
                                      uint32_t src_physical,
                                      uint32_t dest_physical, uint32_t length,
                                      DmaRegisterMap::TI::PERMAP dreq) {
  if (control_block == nullptr) {
    return;
  }

  memset(control_block, 0, sizeof(DmaControlBlock));

  // Configure transfer info
  uint32_t ti = 0;
  ti |= static_cast<uint32_t>(DmaRegisterMap::TI::SRC_INC::kEnable) << 8;
  ti |= static_cast<uint32_t>(DmaRegisterMap::TI::DEST_DREQ::kEnable) << 6;
  ti |= static_cast<uint32_t>(DmaRegisterMap::TI::WAIT_RESP::kEnable) << 3;
  ti |= static_cast<uint32_t>(dreq) << 16;

  control_block->transfer_info = ti;
  control_block->source_addr = src_physical;
  control_block->dest_addr = dest_physical;
  control_block->transfer_length = length;
  control_block->stride = 0;
  control_block->next_control_block = 0;
}

void Dma::ConfigurePeripheralToMemory(DmaControlBlock* control_block,
                                      uint32_t src_physical,
                                      uint32_t dest_physical, uint32_t length,
                                      DmaRegisterMap::TI::PERMAP dreq) {
  if (control_block == nullptr) {
    return;
  }

  memset(control_block, 0, sizeof(DmaControlBlock));

  // Configure transfer info
  uint32_t ti = 0;
  ti |= static_cast<uint32_t>(DmaRegisterMap::TI::DEST_INC::kEnable) << 4;
  ti |= static_cast<uint32_t>(DmaRegisterMap::TI::SRC_DREQ::kEnable) << 10;
  ti |= static_cast<uint32_t>(DmaRegisterMap::TI::WAIT_RESP::kEnable) << 3;
  ti |= static_cast<uint32_t>(dreq) << 16;

  control_block->transfer_info = ti;
  control_block->source_addr = src_physical;
  control_block->dest_addr = dest_physical;
  control_block->transfer_length = length;
  control_block->stride = 0;
  control_block->next_control_block = 0;
}

}  // namespace rpl
