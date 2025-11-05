#include "rpl4/peripheral/dma_standard.hpp"

#include <cstring>

#include "rpl4/system/system.hpp"

namespace rpl {

std::array<std::shared_ptr<Dma>, Dma::kNumOfInstances> Dma::instances_ = {
    nullptr};

std::shared_ptr<Dma> Dma::GetInstance(Channel channel) {
  size_t index;
  DmaRegisterMap* reg_map = nullptr;

  switch (channel) {
    case Channel::kChannel0:
      index = 0;
      reg_map = REG_DMA0;
      break;
    case Channel::kChannel1:
      index = 1;
      reg_map = REG_DMA1;
      break;
    case Channel::kChannel2:
      index = 2;
      reg_map = REG_DMA2;
      break;
    case Channel::kChannel3:
      index = 3;
      reg_map = REG_DMA3;
      break;
    case Channel::kChannel4:
      index = 4;
      reg_map = REG_DMA4;
      break;
    case Channel::kChannel5:
      index = 5;
      reg_map = REG_DMA5;
      break;
    case Channel::kChannel6:
      index = 6;
      reg_map = REG_DMA6;
      break;
    case Channel::kChannel8:
      index = 7;
      reg_map = REG_DMA8;
      break;
    case Channel::kChannel9:
      index = 8;
      reg_map = REG_DMA9;
      break;
    case Channel::kChannel10:
      index = 9;
      reg_map = REG_DMA10;
      break;
    default:
      Log(LogLevel::Fatal, "[Dma::GetInstance()] Invalid channel.");
      return nullptr;
  }

  if (!IsInitialized()) {
    Log(LogLevel::Error, "[Dma::GetInstance()] RPL is not initialized.");
  } else if (instances_[index] == nullptr) {
    instances_[index] = std::shared_ptr<Dma>(new Dma(reg_map, channel));
  }
  return instances_[index];
}

Dma::Dma(DmaRegisterMap* register_map, Channel channel)
    : register_map_(register_map), channel_(channel) {
  Reset();
}

void Dma::SetControlBlockAddress(uint32_t control_block_physical_addr) {
  register_map_->conblk_ad.address = control_block_physical_addr;
}

void Dma::ConfigureMemoryToMemory(DmaControlBlock* control_block,
                                  uint32_t src_physical,
                                  uint32_t dest_physical, uint32_t length) {
  if (control_block == nullptr) {
    return;
  }

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wclass-memaccess"
  memset(control_block, 0, sizeof(DmaControlBlock));
  #pragma GCC diagnostic pop

  // Configure transfer info
  control_block->transfer_info.src_inc = DmaRegisterMap::TI::SRC_INC::kEnable;
  control_block->transfer_info.dest_inc = DmaRegisterMap::TI::DEST_INC::kEnable;
  control_block->transfer_info.wait_resp =
      DmaRegisterMap::TI::WAIT_RESP::kEnable;
  control_block->transfer_info.no_wide_bursts =
      DmaRegisterMap::TI::NO_WIDE_BURSTS::kEnable;

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

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wclass-memaccess"
  memset(control_block, 0, sizeof(DmaControlBlock));
  #pragma GCC diagnostic pop

  // Configure transfer info
  control_block->transfer_info.src_inc = DmaRegisterMap::TI::SRC_INC::kEnable;
  control_block->transfer_info.dest_dreq = DmaRegisterMap::TI::DEST_DREQ::kEnable;
  control_block->transfer_info.wait_resp =
      DmaRegisterMap::TI::WAIT_RESP::kEnable;
  control_block->transfer_info.no_wide_bursts =
      DmaRegisterMap::TI::NO_WIDE_BURSTS::kEnable;
  control_block->transfer_info.permap = dreq;

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

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wclass-memaccess"
  memset(control_block, 0, sizeof(DmaControlBlock));
  #pragma GCC diagnostic pop

  // Configure transfer info
  control_block->transfer_info.src_dreq = DmaRegisterMap::TI::SRC_DREQ::kEnable;
  control_block->transfer_info.dest_inc = DmaRegisterMap::TI::DEST_INC::kEnable;
  control_block->transfer_info.wait_resp =
      DmaRegisterMap::TI::WAIT_RESP::kEnable;
  control_block->transfer_info.no_wide_bursts =
      DmaRegisterMap::TI::NO_WIDE_BURSTS::kEnable;
  control_block->transfer_info.permap = dreq;

  control_block->source_addr = src_physical;
  control_block->dest_addr = dest_physical;
  control_block->transfer_length = length;
  control_block->stride = 0;
  control_block->next_control_block = 0;
}

}  // namespace rpl
