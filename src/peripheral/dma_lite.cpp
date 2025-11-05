#include "rpl4/peripheral/dma_lite.hpp"

#include <cstring>

#include "rpl4/system/system.hpp"

namespace rpl {

std::array<std::shared_ptr<DmaLite>, DmaLite::kNumOfInstances>
    DmaLite::instances_ = {nullptr};

std::shared_ptr<DmaLite> DmaLite::GetInstance(Channel channel) {
  size_t index;
  DmaLiteRegisterMap* reg_map = nullptr;

  switch (channel) {
    case Channel::kChannel7:
      index = 0;
      reg_map = REG_DMA_LITE7;
      break;
    default:
      Log(LogLevel::Fatal, "[DmaLite::GetInstance()] Invalid channel.");
      return nullptr;
  }

  if (!IsInitialized()) {
    Log(LogLevel::Error, "[DmaLite::GetInstance()] RPL is not initialized.");
  } else if (instances_[index] == nullptr) {
    instances_[index] =
        std::shared_ptr<DmaLite>(new DmaLite(reg_map, channel));
  }
  return instances_[index];
}

DmaLite::DmaLite(DmaLiteRegisterMap* register_map, Channel channel)
    : register_map_(register_map), channel_(channel) {
  Reset();
}

void DmaLite::SetControlBlockAddress(uint32_t control_block_physical_addr) {
  register_map_->conblk_ad.address = control_block_physical_addr;
}

void DmaLite::ConfigureMemoryToMemory(DmaLiteControlBlock* control_block,
                                      uint32_t src_physical,
                                      uint32_t dest_physical, uint32_t length) {
  if (control_block == nullptr) {
    return;
  }

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wclass-memaccess"
  memset(control_block, 0, sizeof(DmaLiteControlBlock));
  #pragma GCC diagnostic pop

  // Configure transfer info (simplified for DMA Lite)
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

void DmaLite::ConfigureMemoryToPeripheral(
    DmaLiteControlBlock* control_block, uint32_t src_physical,
    uint32_t dest_physical, uint32_t length,
    DmaRegisterMap::TI::PERMAP dreq) {
  if (control_block == nullptr) {
    return;
  }

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wclass-memaccess"
  memset(control_block, 0, sizeof(DmaLiteControlBlock));
  #pragma GCC diagnostic pop

  // Configure transfer info (simplified for DMA Lite)
  control_block->transfer_info.src_inc = DmaRegisterMap::TI::SRC_INC::kEnable;
  control_block->transfer_info.dest_dreq =
      DmaRegisterMap::TI::DEST_DREQ::kEnable;
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

void DmaLite::ConfigurePeripheralToMemory(
    DmaLiteControlBlock* control_block, uint32_t src_physical,
    uint32_t dest_physical, uint32_t length,
    DmaRegisterMap::TI::PERMAP dreq) {
  if (control_block == nullptr) {
    return;
  }

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wclass-memaccess"
  memset(control_block, 0, sizeof(DmaLiteControlBlock));
  #pragma GCC diagnostic pop

  // Configure transfer info (simplified for DMA Lite)
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
