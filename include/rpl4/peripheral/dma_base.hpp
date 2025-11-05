#ifndef RPL4_PERIPHERAL_DMA_BASE_HPP_
#define RPL4_PERIPHERAL_DMA_BASE_HPP_

#include <chrono>
#include <cstdint>
#include <thread>

#include "rpl4/registers/registers_dma.hpp"
#include "rpl4/system/log.hpp"

namespace rpl {

// DMA Type Tags for compile-time dispatch
struct StandardDmaTag {};
struct DmaLiteTag {};
struct Dma4Tag {};

// Traits for DMA types - specializations provide type-specific information
template <typename DmaTag>
struct DmaTraits;

// Standard DMA traits
template <>
struct DmaTraits<StandardDmaTag> {
  using RegisterMapType = DmaRegisterMap;
  using ControlBlockType = DmaControlBlock;
  static constexpr const char* kName = "Standard DMA";
};

// DMA Lite traits
template <>
struct DmaTraits<DmaLiteTag> {
  using RegisterMapType = DmaLiteRegisterMap;
  using ControlBlockType = DmaLiteControlBlock;
  static constexpr const char* kName = "DMA Lite";
};

// DMA4 traits
template <>
struct DmaTraits<Dma4Tag> {
  using RegisterMapType = Dma4RegisterMap;
  using ControlBlockType = Dma4ControlBlock;
  static constexpr const char* kName = "DMA4";
};

// CRTP Base class for common DMA functionality
// Derived is the actual DMA class (Dma, DmaLite, or Dma4)
template <typename Derived, typename DmaTag>
class DmaBase {
 public:
  using Traits = DmaTraits<DmaTag>;
  using RegisterMapType = typename Traits::RegisterMapType;
  using ControlBlockType = typename Traits::ControlBlockType;

  /**
   * @brief Enable the DMA channel
   */
  void Enable() {
    uint32_t channel_bit = 1 << static_cast<uint32_t>(GetChannel());
    REG_DMA_ENABLE->enable |= channel_bit;
  }

  /**
   * @brief Disable the DMA channel
   */
  void Disable() {
    uint32_t channel_bit = 1 << static_cast<uint32_t>(GetChannel());
    REG_DMA_ENABLE->enable &= ~channel_bit;
  }

  /**
   * @brief Reset the DMA channel
   */
  void Reset() {
    GetRegisterCS().reset = DmaRegisterMap::CS::RESET::kReset;
    // Wait for reset to complete
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1ms);
  }

  /**
   * @brief Abort the current DMA transfer
   */
  void Abort() {
    GetRegisterCS().abort = DmaRegisterMap::CS::ABORT::kAbort;
  }

  /**
   * @brief Check if DMA transfer is active
   */
  bool IsActive() {
    return GetRegisterCS().active == DmaRegisterMap::CS::ACTIVE::kActive;
  }

  /**
   * @brief Check if DMA transfer has completed
   */
  bool IsComplete() {
    return GetRegisterCS().end == DmaRegisterMap::CS::END::kSet;
  }

  /**
   * @brief Check if DMA has error
   */
  bool HasError() {
    return GetRegisterCS().error == DmaRegisterMap::CS::ERROR::kError;
  }

  /**
   * @brief Clear interrupt flag
   */
  void ClearInterrupt() {
    GetRegisterCS().interrupt = DmaRegisterMap::CS::INT::kSet;
  }

  /**
   * @brief Start DMA transfer
   */
  void Start() {
    GetRegisterCS().active = DmaRegisterMap::CS::ACTIVE::kActive;
  }

  /**
   * @brief Wait for DMA transfer to complete
   */
  bool WaitForCompletion(uint32_t timeout_ms = 0) {
    using namespace std::chrono_literals;
    auto start_time = std::chrono::steady_clock::now();

    while (!IsComplete()) {
      if (HasError()) {
        Log(LogLevel::Error, "[%s] Transfer error on channel %d",
            Traits::kName, static_cast<int>(GetChannel()));
        return false;
      }

      if (timeout_ms > 0) {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                           current_time - start_time)
                           .count();
        if (elapsed >= timeout_ms) {
          Log(LogLevel::Warning, "[%s] Transfer timeout on channel %d",
              Traits::kName, static_cast<int>(GetChannel()));
          return false;
        }
      }

      std::this_thread::sleep_for(10us);
    }

    return true;
  }

  /**
   * @brief Set DMA priority
   */
  void SetPriority(uint8_t priority) {
    if (priority > 15) {
      priority = 15;
    }
    GetRegisterCS().priority =
        static_cast<DmaRegisterMap::CS::PRIORITY>(priority);
  }

  /**
   * @brief Set DMA panic priority
   */
  void SetPanicPriority(uint8_t panic_priority) {
    if (panic_priority > 15) {
      panic_priority = 15;
    }
    GetRegisterCS().panic_priority =
        static_cast<DmaRegisterMap::CS::PANIC_PRIORITY>(panic_priority);
  }

  /**
   * @brief Get the DMA type name
   */
  const char* GetTypeName() const { return Traits::kName; }

 protected:
  DmaBase() = default;
  ~DmaBase() = default;

 private:
  // CRTP: Get the derived class
  Derived& derived() { return static_cast<Derived&>(*this); }
  const Derived& derived() const {
    return static_cast<const Derived&>(*this);
  }

  // Access CS register through derived class
  volatile DmaRegisterMap::CS& GetRegisterCS() {
    return derived().GetRegisterCS();
  }

  // Access channel through derived class
  uint32_t GetChannel() const { return derived().GetChannelNumber(); }
};

}  // namespace rpl

#endif  // RPL4_PERIPHERAL_DMA_BASE_HPP_
