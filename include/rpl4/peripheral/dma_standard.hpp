#ifndef RPL4_PERIPHERAL_DMA_STANDARD_HPP_
#define RPL4_PERIPHERAL_DMA_STANDARD_HPP_

#include <array>
#include <cstdint>
#include <cstring>
#include <memory>

#include "rpl4/peripheral/dma_base.hpp"
#include "rpl4/registers/registers_dma.hpp"

namespace rpl {

// Forward declarations
class DmaLite;
class Dma4;

/**
 * @brief Standard DMA class (Channels 0-6, 8-10)
 * @details Provides full-featured DMA with 32-bit addressing
 */
class Dma : public DmaBase<Dma, StandardDmaTag> {
 public:
  friend class DmaBase<Dma, StandardDmaTag>;

  enum class Channel : size_t {
    kChannel0 = 0,
    kChannel1 = 1,
    kChannel2 = 2,
    kChannel3 = 3,
    kChannel4 = 4,
    kChannel5 = 5,
    kChannel6 = 6,
    kChannel8 = 8,
    kChannel9 = 9,
    kChannel10 = 10,
  };

  /**
   * @brief Get the Dma instance of specified channel.
   */
  static std::shared_ptr<Dma> GetInstance(Channel channel);

  Dma(const Dma&) = delete;
  Dma& operator=(const Dma&) = delete;
  Dma(Dma&&) = delete;
  Dma& operator=(Dma&&) = delete;
  ~Dma() = default;

  /**
   * @brief Get the DmaRegisterMap pointer.
   */
  inline DmaRegisterMap* GetRegister() const { return register_map_; }

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
   */
  static void ConfigureMemoryToMemory(DmaControlBlock* control_block,
                                      uint32_t src_physical,
                                      uint32_t dest_physical, uint32_t length);

  /**
   * @brief Configure a memory-to-peripheral transfer
   */
  static void ConfigureMemoryToPeripheral(
      DmaControlBlock* control_block, uint32_t src_physical,
      uint32_t dest_physical, uint32_t length,
      DmaRegisterMap::TI::PERMAP dreq);

  /**
   * @brief Configure a peripheral-to-memory transfer
   */
  static void ConfigurePeripheralToMemory(
      DmaControlBlock* control_block, uint32_t src_physical,
      uint32_t dest_physical, uint32_t length,
      DmaRegisterMap::TI::PERMAP dreq);

 private:
  Dma(DmaRegisterMap* register_map, Channel channel);

  // For base class CRTP access
  volatile DmaRegisterMap::CS& GetRegisterCS() { return register_map_->cs; }
  uint32_t GetChannelNumber() const { return static_cast<uint32_t>(channel_); }

  static constexpr size_t kNumOfInstances = 10;
  static std::array<std::shared_ptr<Dma>, kNumOfInstances> instances_;

  DmaRegisterMap* register_map_;
  Channel channel_;
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_DMA_STANDARD_HPP_
