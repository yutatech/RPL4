#include "rpl4/peripheral/dma4.hpp"

#include <cstring>

#include "rpl4/system/system.hpp"

namespace rpl {

std::array<std::shared_ptr<Dma4>, Dma4::kNumOfInstances> Dma4::instances_ = {
    nullptr};

std::shared_ptr<Dma4> Dma4::GetInstance(Channel channel) {
  size_t index;
  Dma4RegisterMap* reg_map = nullptr;

  switch (channel) {
    case Channel::kChannel11:
      index = 0;
      reg_map = REG_DMA4_11;
      break;
    case Channel::kChannel12:
      index = 1;
      reg_map = REG_DMA4_12;
      break;
    case Channel::kChannel13:
      index = 2;
      reg_map = REG_DMA4_13;
      break;
    case Channel::kChannel14:
      index = 3;
      reg_map = REG_DMA4_14;
      break;
    default:
      Log(LogLevel::Fatal, "[Dma4::GetInstance()] Invalid channel.");
      return nullptr;
  }

  if (!IsInitialized()) {
    Log(LogLevel::Error, "[Dma4::GetInstance()] RPL is not initialized.");
  } else if (instances_[index] == nullptr) {
    instances_[index] = std::shared_ptr<Dma4>(new Dma4(reg_map, channel));
  }
  return instances_[index];
}

Dma4::Dma4(Dma4RegisterMap* register_map, Channel channel)
    : register_map_(register_map), channel_(channel) {
  Reset();
}

void Dma4::SetControlBlockAddress(uint32_t control_block_physical_addr) {
  // DMA4 uses 40-bit addressing, CB address must be shifted right by 5
  // (32-byte aligned)
  register_map_->cb_addr = control_block_physical_addr >> 5;
}

void Dma4::ConfigureMemoryToMemory(Dma4ControlBlock* control_block,
                                   uint64_t src_physical,
                                   uint64_t dest_physical, uint32_t length) {
  if (control_block == nullptr) {
    return;
  }

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wclass-memaccess"
  memset(control_block, 0, sizeof(Dma4ControlBlock));
  #pragma GCC diagnostic pop

  // Configure transfer info
  control_block->transfer_info.src_inc = DmaRegisterMap::TI::SRC_INC::kEnable;
  control_block->transfer_info.dest_inc = DmaRegisterMap::TI::DEST_INC::kEnable;
  control_block->transfer_info.wait_resp =
      DmaRegisterMap::TI::WAIT_RESP::kEnable;
  control_block->transfer_info.no_wide_bursts =
      DmaRegisterMap::TI::NO_WIDE_BURSTS::kEnable;

  // Split 40-bit addresses
  SplitAddress40(src_physical, control_block->source_addr,
                 control_block->source_info);
  SplitAddress40(dest_physical, control_block->dest_addr,
                 control_block->dest_info);

  control_block->transfer_length = length;
  control_block->stride = 0;
  control_block->next_control_block = 0;
}

void Dma4::ConfigureMemoryToPeripheral(Dma4ControlBlock* control_block,
                                       uint64_t src_physical,
                                       uint64_t dest_physical, uint32_t length,
                                       DmaRegisterMap::TI::PERMAP dreq) {
  if (control_block == nullptr) {
    return;
  }

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wclass-memaccess"
  memset(control_block, 0, sizeof(Dma4ControlBlock));
  #pragma GCC diagnostic pop

  // Configure transfer info
  control_block->transfer_info.src_inc = DmaRegisterMap::TI::SRC_INC::kEnable;
  control_block->transfer_info.dest_dreq =
      DmaRegisterMap::TI::DEST_DREQ::kEnable;
  control_block->transfer_info.wait_resp =
      DmaRegisterMap::TI::WAIT_RESP::kEnable;
  control_block->transfer_info.no_wide_bursts =
      DmaRegisterMap::TI::NO_WIDE_BURSTS::kEnable;
  control_block->transfer_info.permap = dreq;

  // Split 40-bit addresses
  SplitAddress40(src_physical, control_block->source_addr,
                 control_block->source_info);
  SplitAddress40(dest_physical, control_block->dest_addr,
                 control_block->dest_info);

  control_block->transfer_length = length;
  control_block->stride = 0;
  control_block->next_control_block = 0;
}

void Dma4::ConfigurePeripheralToMemory(Dma4ControlBlock* control_block,
                                       uint64_t src_physical,
                                       uint64_t dest_physical, uint32_t length,
                                       DmaRegisterMap::TI::PERMAP dreq) {
  if (control_block == nullptr) {
    return;
  }

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wclass-memaccess"
  memset(control_block, 0, sizeof(Dma4ControlBlock));
  #pragma GCC diagnostic pop

  // Configure transfer info
  control_block->transfer_info.src_dreq = DmaRegisterMap::TI::SRC_DREQ::kEnable;
  control_block->transfer_info.dest_inc = DmaRegisterMap::TI::DEST_INC::kEnable;
  control_block->transfer_info.wait_resp =
      DmaRegisterMap::TI::WAIT_RESP::kEnable;
  control_block->transfer_info.no_wide_bursts =
      DmaRegisterMap::TI::NO_WIDE_BURSTS::kEnable;
  control_block->transfer_info.permap = dreq;

  // Split 40-bit addresses
  SplitAddress40(src_physical, control_block->source_addr,
                 control_block->source_info);
  SplitAddress40(dest_physical, control_block->dest_addr,
                 control_block->dest_info);

  control_block->transfer_length = length;
  control_block->stride = 0;
  control_block->next_control_block = 0;
}

}  // namespace rpl
