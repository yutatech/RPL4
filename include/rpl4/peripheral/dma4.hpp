#ifndef RPL4_PERIPHERAL_DMA4_HPP_
#define RPL4_PERIPHERAL_DMA4_HPP_

#include <array>
#include <cstdint>
#include <cstring>
#include <memory>

#include "rpl4/peripheral/dma_base.hpp"
#include "rpl4/registers/registers_dma.hpp"

namespace rpl {

/**
 * @brief DMA4 class (Channels 11-14)
 * @details Enhanced DMA with 40-bit addressing for full memory access
 */
class Dma4 : public DmaBase<Dma4, Dma4Tag> {
 public:
  friend class DmaBase<Dma4, Dma4Tag>;

  enum class Channel : size_t {
    kChannel11 = 11,
    kChannel12 = 12,
    kChannel13 = 13,
    kChannel14 = 14,
  };

  /**
   * @brief Get the Dma4 instance of specified channel.
   */
  static std::shared_ptr<Dma4> GetInstance(Channel channel);

  Dma4(const Dma4&) = delete;
  Dma4& operator=(const Dma4&) = delete;
  Dma4(Dma4&&) = delete;
  Dma4& operator=(Dma4&&) = delete;
  ~Dma4() = default;

  /**
   * @brief Get the Dma4RegisterMap pointer.
   */
  inline Dma4RegisterMap* GetRegister() const { return register_map_; }

  /**
   * @brief Get the channel enum
   */
  inline Channel GetChannel() const { return channel_; }

  /**
   * @brief Set control block address (40-bit address)
   * @param control_block_physical_addr Physical address of control block
   * @note Address must be 32-byte aligned (lower 5 bits ignored)
   */
  void SetControlBlockAddress(uint32_t control_block_physical_addr);

  /**
   * @brief Configure a simple memory-to-memory transfer with 40-bit addressing
   * @param control_block Control block to configure
   * @param src_physical Source physical address (40-bit)
   * @param dest_physical Destination physical address (40-bit)
   * @param length Transfer length in bytes
   */
  static void ConfigureMemoryToMemory(Dma4ControlBlock* control_block,
                                      uint64_t src_physical,
                                      uint64_t dest_physical, uint32_t length);

  /**
   * @brief Configure a memory-to-peripheral transfer with 40-bit addressing
   */
  static void ConfigureMemoryToPeripheral(
      Dma4ControlBlock* control_block, uint64_t src_physical,
      uint64_t dest_physical, uint32_t length,
      DmaRegisterMap::TI::PERMAP dreq);

  /**
   * @brief Configure a peripheral-to-memory transfer with 40-bit addressing
   */
  static void ConfigurePeripheralToMemory(
      Dma4ControlBlock* control_block, uint64_t src_physical,
      uint64_t dest_physical, uint32_t length,
      DmaRegisterMap::TI::PERMAP dreq);

  /**
   * @brief Helper to split 40-bit address into 32-bit addr and 8-bit info
   */
  static void SplitAddress40(uint64_t addr, volatile uint32_t& addr_lower,
                             volatile uint32_t& addr_upper) {
    addr_lower = static_cast<uint32_t>(addr & 0xFFFFFFFF);
    addr_upper = static_cast<uint32_t>((addr >> 32) & 0xFF);
  }

  /**
   * @brief Helper to combine 32-bit addr and 8-bit info into 40-bit address
   */
  static uint64_t CombineAddress40(uint32_t addr_lower, uint32_t addr_upper) {
    return static_cast<uint64_t>(addr_lower) |
           (static_cast<uint64_t>(addr_upper & 0xFF) << 32);
  }

 private:
  Dma4(Dma4RegisterMap* register_map, Channel channel);

  // For base class CRTP access
  volatile DmaRegisterMap::CS& GetRegisterCS() { return register_map_->cs; }
  uint32_t GetChannelNumber() const { return static_cast<uint32_t>(channel_); }

  static constexpr size_t kNumOfInstances = 4;
  static std::array<std::shared_ptr<Dma4>, kNumOfInstances> instances_;

  Dma4RegisterMap* register_map_;
  Channel channel_;
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_DMA4_HPP_
