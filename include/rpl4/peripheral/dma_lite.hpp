#ifndef RPL4_PERIPHERAL_DMA_LITE_HPP_
#define RPL4_PERIPHERAL_DMA_LITE_HPP_

#include <array>
#include <cstdint>
#include <cstring>
#include <memory>

#include "rpl4/peripheral/dma_base.hpp"
#include "rpl4/registers/registers_dma.hpp"

namespace rpl {

/**
 * @brief DMA Lite class (Channel 7)
 * @details Provides simplified DMA with reduced features for lighter workloads
 */
class DmaLite : public DmaBase<DmaLite, DmaLiteTag> {
 public:
  friend class DmaBase<DmaLite, DmaLiteTag>;

  enum class Channel : size_t {
    kChannel7 = 7,
    // Note: Channel 15 is not mapped in current BCM2711 implementation
  };

  /**
   * @brief Get the DmaLite instance of specified channel.
   */
  static std::shared_ptr<DmaLite> GetInstance(Channel channel);

  DmaLite(const DmaLite&) = delete;
  DmaLite& operator=(const DmaLite&) = delete;
  DmaLite(DmaLite&&) = delete;
  DmaLite& operator=(DmaLite&&) = delete;
  ~DmaLite() = default;

  /**
   * @brief Get the DmaLiteRegisterMap pointer.
   */
  inline DmaLiteRegisterMap* GetRegister() const { return register_map_; }

  /**
   * @brief Get the channel enum
   */
  inline Channel GetChannel() const { return channel_; }

  /**
   * @brief Set control block address
   */
  void SetControlBlockAddress(uint32_t control_block_physical_addr);

  /**
   * @brief Configure a simple memory-to-memory transfer
   * @note DMA Lite has reduced functionality - avoid complex transfers
   */
  static void ConfigureMemoryToMemory(DmaLiteControlBlock* control_block,
                                      uint32_t src_physical,
                                      uint32_t dest_physical, uint32_t length);

  /**
   * @brief Configure a memory-to-peripheral transfer
   * @note DMA Lite has reduced functionality compared to standard DMA
   */
  static void ConfigureMemoryToPeripheral(
      DmaLiteControlBlock* control_block, uint32_t src_physical,
      uint32_t dest_physical, uint32_t length,
      DmaRegisterMap::TI::PERMAP dreq);

  /**
   * @brief Configure a peripheral-to-memory transfer
   * @note DMA Lite has reduced functionality compared to standard DMA
   */
  static void ConfigurePeripheralToMemory(
      DmaLiteControlBlock* control_block, uint32_t src_physical,
      uint32_t dest_physical, uint32_t length,
      DmaRegisterMap::TI::PERMAP dreq);

 private:
  DmaLite(DmaLiteRegisterMap* register_map, Channel channel);

  // For base class CRTP access
  volatile DmaRegisterMap::CS& GetRegisterCS() { return register_map_->cs; }
  uint32_t GetChannelNumber() const { return static_cast<uint32_t>(channel_); }

  static constexpr size_t kNumOfInstances = 1;
  static std::array<std::shared_ptr<DmaLite>, kNumOfInstances> instances_;

  DmaLiteRegisterMap* register_map_;
  Channel channel_;
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_DMA_LITE_HPP_
