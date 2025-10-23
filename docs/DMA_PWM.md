# DMA and PWM Implementation

This document describes the DMA (Direct Memory Access) and PWM (Pulse Width Modulation) implementations in RPL4.

## DMA (Direct Memory Access)

The DMA implementation provides hardware-accelerated data transfer without CPU intervention.

### Features

- Support for all 15 DMA channels (0-14)
- Memory-to-memory transfers
- Memory-to-peripheral transfers
- Peripheral-to-memory transfers
- Physical memory allocation and management
- Control block chains for complex transfers
- Priority and panic priority configuration

### Usage Example

```cpp
#include "rpl4/peripheral/dma.hpp"
#include "rpl4/system/dma_memory.hpp"

// Get DMA instance
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

### DMA Memory Management

The `DmaMemory` class manages physical memory allocation:

- Uses the mailbox interface to allocate uncached memory
- Provides physical address translation
- Supports typed object allocation with `AllocateObject<T>()`
- Automatic 32-byte alignment for DMA control blocks

### DMA Channels

- Channels 0-14 are available
- Channel 15 is reserved for the system
- Lite channels (7-10) have reduced features
- Channel 4 is often used by the firmware

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

Demonstrates basic DMA memory-to-memory transfer with data verification.

### pwm_dma_example.cpp

Shows PWM output with DMA to generate a sine wave pattern without CPU intervention.

### pwm_example.cpp

Basic PWM usage with manual duty cycle control (backward compatible).

## Hardware References

- [BCM2711 Peripherals Datasheet](https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf)
- [rpi_ws281x DMA Example](https://github.com/jgarff/rpi_ws281x)

## Notes

- DMA requires physical memory addresses, not virtual addresses
- Control blocks must be 32-byte aligned
- PWM FIFO can hold up to 16 32-bit values
- DMA channel availability depends on system configuration
- Root privileges required for /dev/mem and /dev/vcio access
