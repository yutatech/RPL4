#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include "rpl4/peripheral/dma.hpp"
#include "rpl4/rpl4.hpp"
#include "rpl4/system/dma_memory.hpp"

int main(void) {
  rpl::Init();

  std::cout << "DMA Example - Memory to Memory Transfer" << std::endl;

  // Get DMA instance for channel 0
  auto dma = rpl::Dma::GetInstance(rpl::Dma::Channel::kChannel0);
  if (dma == nullptr) {
    std::cerr << "Failed to get DMA instance" << std::endl;
    return 1;
  }

  // Get DMA memory allocator
  auto& dma_memory = rpl::DmaMemory::GetInstance();

  // Allocate source and destination buffers
  constexpr size_t kBufferSize = 256;
  uint32_t* src_buffer =
      static_cast<uint32_t*>(dma_memory.Allocate(kBufferSize * sizeof(uint32_t)));
  uint32_t* dst_buffer =
      static_cast<uint32_t*>(dma_memory.Allocate(kBufferSize * sizeof(uint32_t)));

  if (src_buffer == nullptr || dst_buffer == nullptr) {
    std::cerr << "Failed to allocate DMA memory" << std::endl;
    return 1;
  }

  for (size_t i = 0; i < kBufferSize; i++) {
    // Initialize source buffer with test pattern
    src_buffer[i] = i * i;
    // Clear destination buffer
    dst_buffer[i] = 0;
  }

  // Get physical addresses
  uint32_t src_physical = dma_memory.GetPhysicalAddress(src_buffer);
  uint32_t dst_physical = dma_memory.GetPhysicalAddress(dst_buffer);

  std::cout << "Source virtual: " << std::hex << src_buffer
            << ", physical: 0x" << src_physical << std::endl;
  std::cout << "Dest virtual: " << std::hex << dst_buffer
            << ", physical: 0x" << dst_physical << std::endl;

  // Allocate and configure DMA control block
  auto* control_block =
      dma_memory.AllocateObject<rpl::DmaControlBlock>();
  if (control_block == nullptr) {
    std::cerr << "Failed to allocate control block" << std::endl;
    dma_memory.Free(src_buffer);
    dma_memory.Free(dst_buffer);
    return 1;
  }

  rpl::Dma::ConfigureMemoryToMemory(control_block, src_physical, dst_physical,
                                    kBufferSize * sizeof(uint32_t));

  uint32_t cb_physical = dma_memory.GetPhysicalAddress(control_block);
  std::cout << "Control block physical: 0x" << std::hex << cb_physical
            << std::endl;

  // Enable DMA channel
  dma->Enable();
  dma->Reset();

  // Set control block address and start transfer
  dma->SetControlBlockAddress(cb_physical);
  dma->SetPriority(8);

  std::cout << "Starting DMA transfer..." << std::endl;
  dma->Start();

  // Wait for completion
  if (dma->WaitForCompletion(1000)) {
    std::cout << "DMA transfer completed successfully!" << std::endl;

    // Verify data
    bool success = true;
    for (size_t i = 0; i < kBufferSize; i++) {
      if (dst_buffer[i] != src_buffer[i]) {
        std::cerr << "Mismatch at index " << std::dec << i << ": expected 0x"
                  << std::hex << src_buffer[i] << ", got 0x" << dst_buffer[i]
                  << std::endl;
        success = false;
        break;
      }
    }

    if (success) {
      std::cout << "Data verification passed!" << std::endl;
      std::cout << "First few values: " << std::endl;
      std::cout << "src \tdst" << std::endl;
      for (size_t i = 0; i < kBufferSize; i++) {
        std::cout << std::hex << src_buffer[i] << "\t" << dst_buffer[i]
                  << std::endl;
      }
      std::cout << std::endl;
    }
  } else {
    std::cerr << "DMA transfer failed or timed out" << std::endl;
  }

  // Clean up
  dma_memory.FreeObject(control_block);
  dma_memory.Free(src_buffer);
  dma_memory.Free(dst_buffer);
  dma->Disable();

  return 0;
}
