#ifndef RPL4_PERIPHERAL_DMA_HPP_
#define RPL4_PERIPHERAL_DMA_HPP_

#include <array>
#include <cstdint>
#include <memory>

#include "rpl4/registers/registers_dma.hpp"

namespace rpl {

class Dma {
 public:
  enum class Channel : size_t {
    kChannel0 = 0,
    kChannel1 = 1,
    kChannel2 = 2,
    kChannel3 = 3,
    kChannel4 = 4,
    kChannel5 = 5,
    kChannel6 = 6,
    kChannel7 = 7,
    kChannel8 = 8,
    kChannel9 = 9,
    kChannel10 = 10,
    kChannel11 = 11,
    kChannel12 = 12,
    kChannel13 = 13,
    kChannel14 = 14,
  };

  /**
   * @brief Get the Dma instance of specified channel.
   * @details To save memory, only the channel instance obtained with
   *          GetInstance() is created. If a channel instance has already
   *          been created, the same instance will be returned.
   *
   * @param channel DMA channel
   * @return std::shared_ptr<Dma>
   */
  static std::shared_ptr<Dma> GetInstance(Channel channel);

  Dma(const Dma&) = delete;
  Dma& operator=(const Dma&) = delete;
  Dma(Dma&&) = delete;
  Dma& operator=(Dma&&) = delete;
  ~Dma() = default;

  /**
   * @brief Get the DmaRegisterMap pointer.
   *
   * @return DmaRegisterMap*
   */
  inline DmaRegisterMap* GetRegister() const { return register_map_; }

  /**
   * @brief Get the channel number
   *
   * @return Channel number
   */
  inline Channel GetChannel() const { return channel_; }

  /**
   * @brief Enable the DMA channel
   */
  void Enable();

  /**
   * @brief Disable the DMA channel
   */
  void Disable();

  /**
   * @brief Reset the DMA channel
   */
  void Reset();

  /**
   * @brief Abort the current DMA transfer
   */
  void Abort();

  /**
   * @brief Check if DMA transfer is active
   *
   * @return true if active, false otherwise
   */
  bool IsActive();

  /**
   * @brief Check if DMA transfer has completed
   *
   * @return true if completed, false otherwise
   */
  bool IsComplete();

  /**
   * @brief Check if DMA has error
   *
   * @return true if error, false otherwise
   */
  bool HasError();

  /**
   * @brief Clear interrupt flag
   */
  void ClearInterrupt();

  /**
   * @brief Set control block address
   *
   * @param control_block_physical_addr Physical address of control block
   * @note Dma::Reset() clears this setting, so if Reset() is called,
   *       this method needs to be called again before starting transfer.
   */
  void SetControlBlockAddress(uint32_t control_block_physical_addr);

  /**
   * @brief Start DMA transfer
   */
  void Start();

  /**
   * @brief Wait for DMA transfer to complete
   *
   * @param timeout_ms Timeout in milliseconds (0 = no timeout)
   * @return true if completed, false if timeout
   */
  bool WaitForCompletion(uint32_t timeout_ms = 0);

  /**
   * @brief Set DMA priority
   *
   * @param priority Priority level (0-15, higher is more important)
   */
  void SetPriority(uint8_t priority);

  /**
   * @brief Set DMA panic priority
   *
   * @param panic_priority Panic priority level (0-15)
   */
  void SetPanicPriority(uint8_t panic_priority);

  /**
   * @brief Configure a simple memory-to-memory transfer
   *
   * @param control_block Control block to configure
   * @param src_physical Source physical address
   * @param dest_physical Destination physical address
   * @param length Transfer length in bytes
   */
  static void ConfigureMemoryToMemory(DmaControlBlock* control_block,
                                      uint32_t src_physical,
                                      uint32_t dest_physical,
                                      uint32_t length);

  /**
   * @brief Configure a memory-to-peripheral transfer
   *
   * @param control_block Control block to configure
   * @param src_physical Source physical address
   * @param dest_physical Destination physical address (peripheral register)
   * @param length Transfer length in bytes
   * @param dreq DREQ signal mapping
   */
  static void ConfigureMemoryToPeripheral(
      DmaControlBlock* control_block, uint32_t src_physical,
      uint32_t dest_physical, uint32_t length,
      DmaRegisterMap::TI::PERMAP dreq);

  /**
   * @brief Configure a peripheral-to-memory transfer
   *
   * @param control_block Control block to configure
   * @param src_physical Source physical address (peripheral register)
   * @param dest_physical Destination physical address
   * @param length Transfer length in bytes
   * @param dreq DREQ signal mapping
   */
  static void ConfigurePeripheralToMemory(
      DmaControlBlock* control_block, uint32_t src_physical,
      uint32_t dest_physical, uint32_t length,
      DmaRegisterMap::TI::PERMAP dreq);

 private:
  Dma(DmaRegisterMap* register_map, Channel channel);

  static constexpr size_t kNumOfInstances = 15;
  static std::array<std::shared_ptr<Dma>, kNumOfInstances> instances_;

  DmaRegisterMap* register_map_;
  Channel channel_;
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_DMA_HPP_
