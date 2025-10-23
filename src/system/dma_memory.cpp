#include "rpl4/system/dma_memory.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstring>

#include "rpl4/system/log.hpp"

namespace rpl {

// Mailbox interface definitions for Raspberry Pi
constexpr uint32_t kMailboxDevice = 0x3F00B880;  // BCM2711 mailbox base
constexpr uint32_t kMemFlagDirectAlloc = 1 << 2;
constexpr uint32_t kMemFlagL1Nonallocating = kMemFlagDirectAlloc << 2;

struct MailboxProperty {
  uint32_t size;
  uint32_t request_response;
  uint32_t tags[0];
};

constexpr uint32_t kMailboxRequestCode = 0x00000000;
constexpr uint32_t kMailboxResponseSuccess = 0x80000000;
constexpr uint32_t kMailboxResponseError = 0x80000001;
constexpr uint32_t kMailboxTagAllocateMemory = 0x0003000c;
constexpr uint32_t kMailboxTagLockMemory = 0x0003000d;
constexpr uint32_t kMailboxTagUnlockMemory = 0x0003000e;
constexpr uint32_t kMailboxTagReleaseMemory = 0x0003000f;

DmaMemory& DmaMemory::GetInstance() {
  static DmaMemory instance;
  return instance;
}

DmaMemory::DmaMemory() : mailbox_fd_(-1) { InitializeMailbox(); }

DmaMemory::~DmaMemory() {
  for (auto& block : blocks_) {
    if (block.in_use) {
      FreeBlock(block);
    }
  }
  CloseMailbox();
}

bool DmaMemory::InitializeMailbox() {
  mailbox_fd_ = open("/dev/vcio", O_RDWR);
  if (mailbox_fd_ < 0) {
    Log(LogLevel::Error, "[DmaMemory] Failed to open /dev/vcio");
    return false;
  }
  return true;
}

void DmaMemory::CloseMailbox() {
  if (mailbox_fd_ >= 0) {
    close(mailbox_fd_);
    mailbox_fd_ = -1;
  }
}

bool DmaMemory::AllocateBlock(size_t size, MemoryBlock& block) {
  if (mailbox_fd_ < 0) {
    Log(LogLevel::Error, "[DmaMemory] Mailbox not initialized");
    return false;
  }

  // Align size to page boundary
  size_t aligned_size = (size + 4095) & ~4095;

  // Allocate memory using mailbox
  uint32_t message[32] __attribute__((aligned(16)));
  memset(message, 0, sizeof(message));

  message[0] = 9 * 4;  // size
  message[1] = kMailboxRequestCode;
  message[2] = kMailboxTagAllocateMemory;
  message[3] = 12;  // value buffer size
  message[4] = 12;  // request size
  message[5] = aligned_size;
  message[6] = 4096;  // alignment
  message[7] = kMemFlagDirectAlloc | kMemFlagL1Nonallocating;
  message[8] = 0;  // end tag

  if (ioctl(mailbox_fd_, _IOWR(100, 0, char*), message) < 0) {
    Log(LogLevel::Error, "[DmaMemory] ioctl failed for memory allocation");
    return false;
  }

  if (message[1] != kMailboxResponseSuccess) {
    Log(LogLevel::Error, "[DmaMemory] Mailbox response error");
    return false;
  }

  block.handle = message[5];
  block.size = aligned_size;

  // Lock memory to get physical address
  memset(message, 0, sizeof(message));
  message[0] = 7 * 4;
  message[1] = kMailboxRequestCode;
  message[2] = kMailboxTagLockMemory;
  message[3] = 4;
  message[4] = 4;
  message[5] = block.handle;
  message[6] = 0;

  if (ioctl(mailbox_fd_, _IOWR(100, 0, char*), message) < 0) {
    Log(LogLevel::Error, "[DmaMemory] ioctl failed for memory lock");
    return false;
  }

  if (message[1] != kMailboxResponseSuccess) {
    Log(LogLevel::Error, "[DmaMemory] Mailbox lock response error");
    return false;
  }

  block.physical_addr = message[5] & 0x3FFFFFFF;  // Remove VC/ARM bit

  // Map physical memory to user space
  int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (mem_fd < 0) {
    Log(LogLevel::Error, "[DmaMemory] Failed to open /dev/mem");
    return false;
  }

  block.virtual_addr =
      mmap(NULL, block.size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
           block.physical_addr);
  close(mem_fd);

  if (block.virtual_addr == MAP_FAILED) {
    Log(LogLevel::Error, "[DmaMemory] mmap failed");
    return false;
  }

  block.in_use = true;
  return true;
}

void DmaMemory::FreeBlock(MemoryBlock& block) {
  if (!block.in_use) {
    return;
  }

  if (block.virtual_addr != nullptr && block.virtual_addr != MAP_FAILED) {
    munmap(block.virtual_addr, block.size);
  }

  if (mailbox_fd_ >= 0 && block.handle != 0) {
    // Unlock memory
    uint32_t message[32] __attribute__((aligned(16)));
    memset(message, 0, sizeof(message));
    message[0] = 7 * 4;
    message[1] = kMailboxRequestCode;
    message[2] = kMailboxTagUnlockMemory;
    message[3] = 4;
    message[4] = 4;
    message[5] = block.handle;
    message[6] = 0;
    ioctl(mailbox_fd_, _IOWR(100, 0, char*), message);

    // Release memory
    memset(message, 0, sizeof(message));
    message[0] = 7 * 4;
    message[1] = kMailboxRequestCode;
    message[2] = kMailboxTagReleaseMemory;
    message[3] = 4;
    message[4] = 4;
    message[5] = block.handle;
    message[6] = 0;
    ioctl(mailbox_fd_, _IOWR(100, 0, char*), message);
  }

  block.in_use = false;
  block.virtual_addr = nullptr;
  block.physical_addr = 0;
  block.handle = 0;
}

void* DmaMemory::Allocate(size_t size) {
  if (size == 0) {
    return nullptr;
  }

  // Align to kAlignment
  size_t aligned_size = (size + kAlignment - 1) & ~(kAlignment - 1);

  // Try to find an existing free block that can accommodate the request
  for (auto& block : blocks_) {
    if (!block.in_use && block.size >= aligned_size) {
      block.in_use = true;
      return block.virtual_addr;
    }
  }

  // Allocate new block
  MemoryBlock new_block;
  size_t block_size =
      aligned_size > kDefaultBlockSize ? aligned_size : kDefaultBlockSize;

  if (!AllocateBlock(block_size, new_block)) {
    return nullptr;
  }

  blocks_.push_back(new_block);
  return new_block.virtual_addr;
}

void DmaMemory::Free(void* ptr) {
  if (ptr == nullptr) {
    return;
  }

  for (auto& block : blocks_) {
    if (block.virtual_addr == ptr && block.in_use) {
      block.in_use = false;
      return;
    }
  }

  Log(LogLevel::Warning, "[DmaMemory] Attempted to free unknown pointer");
}

uint32_t DmaMemory::GetPhysicalAddress(void* virtual_addr) {
  if (virtual_addr == nullptr) {
    return 0;
  }

  for (const auto& block : blocks_) {
    if (block.in_use && block.virtual_addr <= virtual_addr &&
        static_cast<uint8_t*>(virtual_addr) <
            static_cast<uint8_t*>(block.virtual_addr) + block.size) {
      size_t offset =
          static_cast<uint8_t*>(virtual_addr) -
          static_cast<uint8_t*>(block.virtual_addr);
      return block.physical_addr + offset;
    }
  }

  Log(LogLevel::Error,
      "[DmaMemory] Virtual address not found in allocated blocks");
  return 0;
}

}  // namespace rpl
