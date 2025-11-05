#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include "rpl4/peripheral/dma.hpp"
#include "rpl4/rpl4.hpp"
#include "rpl4/system/dma_memory.hpp"

/**
 * @brief DMA Types Example
 * @details Demonstrates the three types of DMA available on BCM2711:
 *          1. Standard DMA (Channels 0-6, 8-10): Full-featured 32-bit DMA
 *          2. DMA Lite (Channel 7): Simplified DMA for lighter workloads
 *          3. DMA4 (Channels 11-14): Enhanced DMA with 40-bit addressing
 * 
 * This example performs memory-to-memory transfers with each DMA type
 * to show their usage and differences.
 */

// Test data size
constexpr size_t kBufferSize = 256;

// Helper to perform and verify a DMA transfer
template <typename DmaType, typename ControlBlockType>
bool TestDmaTransfer(std::shared_ptr<DmaType> dma, 
                     rpl::DmaMemory& dma_memory,
                     const char* dma_name) {
  std::cout << "\n=== Testing " << dma_name << " ===" << std::endl;
  std::cout << "Channel: " << static_cast<int>(dma->GetChannel()) << std::endl;
  std::cout << "Type: " << dma->GetTypeName() << std::endl;

  // Allocate source and destination buffers
  uint32_t* src_buffer = static_cast<uint32_t*>(
      dma_memory.Allocate(kBufferSize * sizeof(uint32_t)));
  uint32_t* dst_buffer = static_cast<uint32_t*>(
      dma_memory.Allocate(kBufferSize * sizeof(uint32_t)));

  if (src_buffer == nullptr || dst_buffer == nullptr) {
    std::cerr << "Failed to allocate DMA memory" << std::endl;
    return false;
  }

  // Initialize buffers
  for (size_t i = 0; i < kBufferSize; i++) {
    src_buffer[i] = 0xDEAD0000 + i;  // Test pattern
    dst_buffer[i] = 0;
  }

  // Get physical addresses
  uint32_t src_physical = dma_memory.GetPhysicalAddress(src_buffer);
  uint32_t dst_physical = dma_memory.GetPhysicalAddress(dst_buffer);

  std::cout << "Source: virtual=0x" << std::hex << src_buffer
            << ", physical=0x" << src_physical << std::endl;
  std::cout << "Dest:   virtual=0x" << std::hex << dst_buffer
            << ", physical=0x" << dst_physical << std::endl;

  // Allocate and configure control block
  auto* control_block = dma_memory.AllocateObject<ControlBlockType>();
  if (control_block == nullptr) {
    std::cerr << "Failed to allocate control block" << std::endl;
    dma_memory.Free(src_buffer);
    dma_memory.Free(dst_buffer);
    return false;
  }

  // Configure the transfer based on DMA type
  if constexpr (std::is_same_v<DmaType, rpl::Dma4>) {
    // DMA4 uses 40-bit addresses
    DmaType::ConfigureMemoryToMemory(
        control_block,
        static_cast<uint64_t>(src_physical),
        static_cast<uint64_t>(dst_physical),
        kBufferSize * sizeof(uint32_t));
  } else {
    // Standard DMA and DMA Lite use 32-bit addresses
    DmaType::ConfigureMemoryToMemory(
        control_block, src_physical, dst_physical,
        kBufferSize * sizeof(uint32_t));
  }

  uint32_t cb_physical = dma_memory.GetPhysicalAddress(control_block);
  std::cout << "Control block physical: 0x" << std::hex << cb_physical
            << std::endl;

  // Enable and configure DMA
  dma->Enable();
  dma->Reset();
  dma->SetControlBlockAddress(cb_physical);
  dma->SetPriority(8);

  // Measure transfer time
  auto start_time = std::chrono::high_resolution_clock::now();

  // Start transfer
  std::cout << "Starting transfer..." << std::endl;
  dma->Start();

  // Wait for completion
  bool success = dma->WaitForCompletion(1000);
  
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);

  if (success) {
    std::cout << "Transfer completed in " << std::dec << duration.count()
              << " microseconds" << std::endl;

    // Verify data
    bool verify_success = true;
    for (size_t i = 0; i < kBufferSize; i++) {
      if (dst_buffer[i] != src_buffer[i]) {
        std::cerr << "Data mismatch at index " << std::dec << i << ": "
                  << "expected 0x" << std::hex << src_buffer[i]
                  << ", got 0x" << dst_buffer[i] << std::endl;
        verify_success = false;
        break;
      }
    }

    if (verify_success) {
      std::cout << "✓ Data verification passed!" << std::endl;
      std::cout << "Transferred " << std::dec 
                << (kBufferSize * sizeof(uint32_t)) << " bytes" << std::endl;
      
      // Calculate throughput
      double bytes_per_sec = (kBufferSize * sizeof(uint32_t) * 1000000.0) / 
                             duration.count();
      std::cout << "Throughput: " << (bytes_per_sec / (1024.0 * 1024.0))
                << " MB/s" << std::endl;
    } else {
      std::cerr << "✗ Data verification failed!" << std::endl;
      success = false;
    }
  } else {
    std::cerr << "✗ Transfer failed or timed out!" << std::endl;
  }

  // Clean up
  dma->Disable();
  dma_memory.FreeObject(control_block);
  dma_memory.Free(src_buffer);
  dma_memory.Free(dst_buffer);

  return success;
}

int main(void) {
  rpl::Init();

  std::cout << "========================================" << std::endl;
  std::cout << "  BCM2711 DMA Types Demonstration" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "\nThis example demonstrates the three types of DMA" << std::endl;
  std::cout << "available on the Raspberry Pi 4 (BCM2711):" << std::endl;
  std::cout << "\n1. Standard DMA (Channels 0-6, 8-10):" << std::endl;
  std::cout << "   - Full-featured DMA with 32-bit addressing" << std::endl;
  std::cout << "   - Supports complex transfers, 2D mode, peripheral mapping"
            << std::endl;
  std::cout << "\n2. DMA Lite (Channel 7):" << std::endl;
  std::cout << "   - Simplified DMA for basic operations" << std::endl;
  std::cout << "   - Reduced features, suitable for simple transfers"
            << std::endl;
  std::cout << "\n3. DMA4 (Channels 11-14):" << std::endl;
  std::cout << "   - Enhanced DMA with 40-bit addressing" << std::endl;
  std::cout << "   - Supports full 4GB+ memory access" << std::endl;
  std::cout << "   - Best for systems with >1GB RAM" << std::endl;
  std::cout << "\n========================================\n" << std::endl;

  auto& dma_memory = rpl::DmaMemory::GetInstance();
  bool all_tests_passed = true;

  // Test 1: Standard DMA (Channel 0)
  auto dma_standard = rpl::Dma::GetInstance(rpl::Dma::Channel::kChannel0);
  if (dma_standard) {
    all_tests_passed &= TestDmaTransfer<rpl::Dma, rpl::DmaControlBlock>(
        dma_standard, dma_memory, "Standard DMA");
  } else {
    std::cerr << "Failed to get Standard DMA instance" << std::endl;
    all_tests_passed = false;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Test 2: DMA Lite (Channel 7)
  auto dma_lite = rpl::DmaLite::GetInstance(rpl::DmaLite::Channel::kChannel7);
  if (dma_lite) {
    all_tests_passed &= TestDmaTransfer<rpl::DmaLite, rpl::DmaLiteControlBlock>(
        dma_lite, dma_memory, "DMA Lite");
  } else {
    std::cerr << "Failed to get DMA Lite instance" << std::endl;
    all_tests_passed = false;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Test 3: DMA4 (Channel 11)
  auto dma4 = rpl::Dma4::GetInstance(rpl::Dma4::Channel::kChannel11);
  if (dma4) {
    all_tests_passed &= TestDmaTransfer<rpl::Dma4, rpl::Dma4ControlBlock>(
        dma4, dma_memory, "DMA4 (40-bit addressing)");
  } else {
    std::cerr << "Failed to get DMA4 instance" << std::endl;
    all_tests_passed = false;
  }

  // Summary
  std::cout << "\n========================================" << std::endl;
  std::cout << "  Test Summary" << std::endl;
  std::cout << "========================================" << std::endl;
  if (all_tests_passed) {
    std::cout << "✓ All DMA types tested successfully!" << std::endl;
    std::cout << "\nKey Observations:" << std::endl;
    std::cout << "- All three DMA types share common interface via base class"
              << std::endl;
    std::cout << "- No virtual functions - all dispatch at compile time"
              << std::endl;
    std::cout << "- DMA4 supports 40-bit addresses for full memory access"
              << std::endl;
    std::cout << "- Performance varies by hardware and system load"
              << std::endl;
  } else {
    std::cerr << "✗ Some tests failed!" << std::endl;
  }
  std::cout << "========================================\n" << std::endl;

  return all_tests_passed ? 0 : 1;
}
