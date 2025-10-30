#ifndef RPL4_SYSTEM_DMA_MEMORY_HPP_
#define RPL4_SYSTEM_DMA_MEMORY_HPP_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace rpl {

/**
 * @brief DMA Physical Memory Allocator
 * @details Manages allocation of physical memory for DMA operations.
 *          Uses mailbox interface to allocate uncached memory and get
 *          physical addresses.
 * @note Due to strict memory alignment requirements, do not use memset to
 *       assign values. Misaligned memory access may cause a bus error.
 */
class DmaMemory {
 public:
  /**
   * @brief Get the singleton instance of DmaMemory
   * @return Reference to the DmaMemory instance
   */
  static DmaMemory& GetInstance();

  DmaMemory(const DmaMemory&) = delete;
  DmaMemory& operator=(const DmaMemory&) = delete;
  DmaMemory(DmaMemory&&) = delete;
  DmaMemory& operator=(DmaMemory&&) = delete;

  /**
   * @brief Allocate physical memory for DMA
   * @param size Size in bytes to allocate
   * @return Pointer to allocated virtual address, nullptr on failure
   */
  void* Allocate(size_t size);

  /**
   * @brief Free previously allocated DMA memory
   * @param ptr Pointer to memory to free
   */
  void Free(void* ptr);

  /**
   * @brief Get physical address from virtual address
   * @param virtual_addr Virtual address
   * @return Physical address, 0 on failure
   */
  uint32_t GetPhysicalAddress(void* virtual_addr);

  /**
   * @brief Allocate and construct an object in DMA memory
   * @tparam T Type of object to allocate
   * @tparam Args Constructor argument types
   * @param args Constructor arguments
   * @return Pointer to allocated object, nullptr on failure
   */
  template <typename T, typename... Args>
  T* AllocateObject(Args&&... args) {
    void* ptr = Allocate(sizeof(T));
    if (ptr == nullptr) { return nullptr; }
    return new (ptr) T(std::forward<Args>(args)...);
  }

  /**
   * @brief Free object allocated in DMA memory
   * @tparam T Type of object
   * @param obj Pointer to object to free
   */
  template <typename T>
  void FreeObject(T* obj) {
    if (obj != nullptr) {
      obj->~T();
      Free(obj);
    }
  }

 private:
  DmaMemory();
  ~DmaMemory();

  struct MemoryBlock {
    void* virtual_addr;
    uint32_t physical_addr;
    size_t size;
    uint32_t handle;  // Mailbox handle
    bool in_use;
  };

  bool InitializeMailbox();
  void CloseMailbox();
  bool AllocateBlock(size_t size, MemoryBlock& block);
  void FreeBlock(MemoryBlock& block);

  int mailbox_fd_;
  std::vector<MemoryBlock> blocks_;
  static constexpr size_t kDefaultBlockSize = 4096;  // 4KB blocks
  static constexpr size_t kAlignment =
      32;  // 32-byte alignment for DMA control blocks
};

}  // namespace rpl

#endif  // RPL4_SYSTEM_DMA_MEMORY_HPP_
