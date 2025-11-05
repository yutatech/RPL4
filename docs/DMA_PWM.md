# DMA and PWM Implementation

This document describes the DMA (Direct Memory Access) and PWM (Pulse Width Modulation) implementations in RPL4.

## DMA (Direct Memory Access)

The DMA implementation provides hardware-accelerated data transfer without CPU intervention.

### DMA Engine Types

The BCM2711 (Raspberry Pi 4) has three types of DMA engines, each optimized for different use cases:

#### 1. Standard DMA (Channels 0-6, 8-10)
- Full-featured DMA with 32-bit addressing
- Supports complex transfers, 2D mode, peripheral mapping
- Best for general-purpose DMA operations
- Class: `rpl::Dma`

#### 2. DMA Lite (Channel 7)
- Simplified DMA with reduced features
- Lower resource usage for basic operations
- Best for simple memory-to-memory transfers
- Class: `rpl::DmaLite`

#### 3. DMA4 (Channels 11-14)
- Enhanced DMA with 40-bit addressing
- Supports full 4GB+ memory access
- Best for systems with >1GB RAM
- Class: `rpl::Dma4`

### Features

- Support for all 15 DMA channels (0-14)
- Memory-to-memory transfers
- Memory-to-peripheral transfers
- Peripheral-to-memory transfers
- Physical memory allocation and management
- Control block chains for complex transfers
- Priority and panic priority configuration
- Compile-time dispatch (no virtual functions)
- Shared common functionality via CRTP base class

### Usage Examples

#### Standard DMA (32-bit addressing)

```cpp
#include "rpl4/peripheral/dma.hpp"
#include "rpl4/system/dma_memory.hpp"

// Get Standard DMA instance (channels 0-6, 8-10)
auto dma = rpl::Dma::GetInstance(rpl::Dma::Channel::kChannel0);

// Allocate physical memory
auto& dma_memory = rpl::DmaMemory::GetInstance();
void* src_buffer = dma_memory.Allocate(size);
void* dst_buffer = dma_memory.Allocate(size);

// Get physical addresses
uint32_t src_phys = dma_memory.GetPhysicalAddress(src_buffer);
uint32_t dst_phys = dma_memory.GetPhysicalAddress(dst_buffer);

// Create control block
auto* cb = dma_memory.AllocateObject<rpl::DmaControlBlock>();
rpl::Dma::ConfigureMemoryToMemory(cb, src_phys, dst_phys, size);

// Start transfer
dma->Enable();
dma->SetControlBlockAddress(dma_memory.GetPhysicalAddress(cb));
dma->Start();
dma->WaitForCompletion();

// Clean up
dma_memory.FreeObject(cb);
dma_memory.Free(src_buffer);
dma_memory.Free(dst_buffer);
```

#### DMA Lite (simplified)

```cpp
// Get DMA Lite instance (channel 7)
auto dma = rpl::DmaLite::GetInstance(rpl::DmaLite::Channel::kChannel7);

// Same API as Standard DMA, but with reduced features
auto* cb = dma_memory.AllocateObject<rpl::DmaLiteControlBlock>();
rpl::DmaLite::ConfigureMemoryToMemory(cb, src_phys, dst_phys, size);
```

#### DMA4 (40-bit addressing)

```cpp
// Get DMA4 instance (channels 11-14)
auto dma = rpl::Dma4::GetInstance(rpl::Dma4::Channel::kChannel11);

// DMA4 uses 64-bit addresses for 40-bit addressing support
auto* cb = dma_memory.AllocateObject<rpl::Dma4ControlBlock>();
rpl::Dma4::ConfigureMemoryToMemory(
    cb,
    static_cast<uint64_t>(src_phys),  // 40-bit source
    static_cast<uint64_t>(dst_phys),  // 40-bit destination
    size);
```

### DMA Memory Management

The `DmaMemory` class manages physical memory allocation:

- Uses the mailbox interface to allocate uncached memory
- Provides physical address translation
- Supports typed object allocation with `AllocateObject<T>()`
- Automatic 32-byte alignment for DMA control blocks

### DMA Channel Assignment

| Channel(s) | Type          | Class           | Features                                    |
|------------|---------------|-----------------|---------------------------------------------|
| 0-6        | Standard DMA  | `Dma`           | Full-featured, 32-bit addressing            |
| 7          | DMA Lite      | `DmaLite`       | Simplified, reduced features                |
| 8-10       | Standard DMA  | `Dma`           | Full-featured, 32-bit addressing            |
| 11-14      | DMA4          | `Dma4`          | Enhanced, 40-bit addressing, >1GB RAM       |
| 15         | DMA Lite      | Not mapped      | Reserved/not available in current impl.     |

**Notes:**
- Channel 4 is often used by the firmware
- DMA4 channels (11-14) are recommended for systems with >1GB RAM
- All types share common interface via `DmaBase` CRTP template
- No virtual functions - all dispatch at compile time

## PWM (Pulse Width Modulation)

The PWM implementation provides hardware PWM output with DMA support.

### Features

- Two PWM ports (PWM0, PWM1)
- Two channels per port
- M/S (Mark/Space) and PWM algorithm modes
- FIFO support for continuous output
- DMA integration for CPU-free operation
- Configurable frequency and duty cycle
- Polarity control

### GPIO Pin Mapping

| GPIO Pin | PWM Port | Channel | Alt Function |
|----------|----------|---------|--------------|
| 12       | PWM0     | 1       | ALT0         |
| 13       | PWM0     | 2       | ALT0         |
| 18       | PWM0     | 1       | ALT5         |
| 19       | PWM0     | 2       | ALT5         |
| 40       | PWM1     | 1       | ALT0         |
| 41       | PWM1     | 2       | ALT0         |
| 45       | PWM0     | 2       | ALT0         |

### Basic PWM Usage

```cpp
#include "rpl4/peripheral/pwm.hpp"

// Configure GPIO pin
rpl::Pwm::ConfigureGpioPin(18);

// Get PWM instance
auto pwm = rpl::Pwm::GetInstance(rpl::Pwm::Port::kPwm0);

// Initialize clock (25 MHz)
pwm->InitializeClock(25000000);

// Set frequency (1 kHz)
pwm->SetFrequency(rpl::Pwm::Channel::kChannel1, 1000.0);

// Set duty cycle (50%)
pwm->SetDutyCycle(rpl::Pwm::Channel::kChannel1, 0.5);

// Enable M/S mode for stable frequency
pwm->SetMSMode(rpl::Pwm::Channel::kChannel1, true);

// Enable PWM output
pwm->Enable(rpl::Pwm::Channel::kChannel1);
```

### PWM with DMA

For continuous, CPU-free PWM pattern generation:

```cpp
// Enable FIFO
pwm->EnableFifo(rpl::Pwm::Channel::kChannel1);

// Enable DMA
pwm->EnableDma(7, 7);

// Configure DMA transfer to PWM FIFO
uint32_t pwm_fifo_phys = pwm->GetFifoPhysicalAddress();
rpl::Dma::ConfigureMemoryToPeripheral(
    control_block, pattern_phys, pwm_fifo_phys, size,
    rpl::DmaRegisterMap::TI::PERMAP::kPwm0);

// Start DMA and PWM
dma->Start();
pwm->Enable(rpl::Pwm::Channel::kChannel1);
```

### Clock Configuration

PWM uses the CM_PWM clock manager:

- Default source: 54 MHz oscillator
- Configurable divider for target frequency
- MASH filter support for frequency smoothing

```cpp
// Set PWM clock to 25 MHz
pwm->InitializeClock(25000000);
```

## Examples

### dma_example.cpp

Demonstrates basic DMA memory-to-memory transfer with data verification using Standard DMA.

### dma_types_example.cpp

**NEW:** Comprehensive example demonstrating all three DMA types:
- Standard DMA (Channel 0) - Full-featured 32-bit DMA
- DMA Lite (Channel 7) - Simplified DMA for basic operations
- DMA4 (Channel 11) - Enhanced DMA with 40-bit addressing

Each type performs a memory-to-memory transfer with:
- Performance measurement and throughput calculation
- Data verification
- Comparison of different DMA engine capabilities

### pwm_dma_example.cpp

Shows PWM output with DMA to generate a sine wave pattern without CPU intervention.

### pwm_example.cpp

Basic PWM usage with manual duty cycle control (backward compatible).

## Hardware References

- [BCM2711 Peripherals Datasheet](https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf)
- [rpi_ws281x DMA Example](https://github.com/jgarff/rpi_ws281x)

## Architecture Notes

### CRTP Design Pattern

The DMA implementation uses the Curiously Recurring Template Pattern (CRTP) to:
- Share common functionality across DMA types
- Avoid virtual function overhead
- Enable compile-time dispatch
- Maintain type safety

```cpp
// Base class uses CRTP
template <typename Derived, typename DmaTag>
class DmaBase { ... };

// Derived classes inherit from base
class Dma : public DmaBase<Dma, StandardDmaTag> { ... };
class DmaLite : public DmaBase<DmaLite, DmaLiteTag> { ... };
class Dma4 : public DmaBase<Dma4, Dma4Tag> { ... };
```

### Type Traits

Each DMA type has associated traits:
```cpp
template <typename DmaTag> struct DmaTraits;
// Provides: RegisterMapType, ControlBlockType, kName
```

This allows compile-time customization without runtime overhead.

## General Notes

- DMA requires physical memory addresses, not virtual addresses
- Control blocks must be 32-byte aligned
- DMA4 control block addresses are shifted right by 5 (32-byte alignment)
- PWM FIFO can hold up to 16 32-bit values
- DMA channel availability depends on system configuration
- Root privileges required for /dev/mem and /dev/vcio access
- Backward compatibility maintained - existing code continues to work
