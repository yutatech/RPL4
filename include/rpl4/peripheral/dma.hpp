#ifndef RPL4_PERIPHERAL_DMA_HPP_
#define RPL4_PERIPHERAL_DMA_HPP_

// This header provides backward compatibility and convenience
// Include all DMA types
#include "rpl4/peripheral/dma4.hpp"
#include "rpl4/peripheral/dma_lite.hpp"
#include "rpl4/peripheral/dma_standard.hpp"

// For backward compatibility, Dma class is now in dma_standard.hpp
// Users can continue to use #include "rpl4/peripheral/dma.hpp" and get
// all DMA types (Dma, DmaLite, Dma4)

#endif  // RPL4_PERIPHERAL_DMA_HPP_
