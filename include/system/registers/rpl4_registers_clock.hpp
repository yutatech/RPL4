#ifndef RPL4_REGISTERS_CLOCK_HPP_
#define RPL4_REGISTERS_CLOCK_HPP_

#include <stdint.h>

namespace rpl {
namespace registers {

const uint32_t CLK_BASE = 0xFE101000;
const uint32_t CLK_SIZE = 0x000001C8;
typedef struct
{
  volatile uint32_t RESERVED_1[2]; // 0x000 ~ 0x007
  volatile uint32_t CM_VPUCTL;		// 0x008
  volatile uint32_t CM_VPUDIV;		// 0x00c
  volatile uint32_t CM_SYSCTL;		// 0x010
  volatile uint32_t CM_SYSDIV;		// 0x014
  volatile uint32_t CM_PERIACTL;	// 0x018
  volatile uint32_t CM_PERIADIV;	// 0x01c
  volatile uint32_t CM_PERIICTL;	// 0x020
  volatile uint32_t CM_PERIIDIV;	// 0x024
  volatile uint32_t CM_H264CTL;	// 0x028
  volatile uint32_t CM_H264DIV;	// 0x02c
  volatile uint32_t CM_ISPCTL;		// 0x030
  volatile uint32_t CM_ISPDIV;		// 0x034
  volatile uint32_t CM_V3DCTL;		// 0x038
  volatile uint32_t CM_V3DDIV;		// 0x03c
  volatile uint32_t CM_CAM0CTL;	// 0x040
  volatile uint32_t CM_CAM0DIV;	// 0x044
  volatile uint32_t CM_CAM1CTL;	// 0x048
  volatile uint32_t CM_CAM1DIV;	// 0x04c
  volatile uint32_t CM_CCP2CTL;	// 0x050
  volatile uint32_t CM_CCP2DIV;	// 0x054
  volatile uint32_t CM_DSI0ECTL;	// 0x058
  volatile uint32_t CM_DSI0EDIV;	// 0x05c
  volatile uint32_t CM_DSI0PCTL;	// 0x060
  volatile uint32_t CM_DSI0PDIV;	// 0x064
  volatile uint32_t CM_DPICTL;		// 0x068
  volatile uint32_t CM_DPIDIV;		// 0x06c
  volatile uint32_t CM_GP0CTL;		// 0x070
  volatile uint32_t CM_GP0DIV;		// 0x074
  volatile uint32_t CM_GP1CTL;		// 0x078
  volatile uint32_t CM_GP1DIV;		// 0x07c
  volatile uint32_t CM_GP2CTL;		// 0x080
  volatile uint32_t CM_GP2DIV;		// 0x084
  volatile uint32_t CM_HSMCTL;		// 0x088
  volatile uint32_t CM_HSMDIV;		// 0x08c
  volatile uint32_t CM_OTPCTL;		// 0x090
  volatile uint32_t CM_OTPDIV;		// 0x094
  volatile uint32_t CM_PCMCTL;		// 0x098
  volatile uint32_t CM_PCMDIV;		// 0x09c
  volatile uint32_t CM_PWMCTL;		// 0x0a0
  volatile uint32_t CM_PWMDIV;		// 0x0a4
  volatile uint32_t CM_SLIMCTL;	// 0x0a8
  volatile uint32_t CM_SLIMDIV;	// 0x0ac
  volatile uint32_t CM_SMICTL;		// 0x0b0
  volatile uint32_t CM_SMIDIV;		// 0x0b4
  volatile uint32_t RESERVED2[2];  // 0x0b8 ~ 0x0bf
  volatile uint32_t CM_TCNTCTL;	// 0x0c0
  volatile uint32_t CM_TCNTCNT;	// 0x0c4
  volatile uint32_t CM_TECCTL;		// 0x0c8
  volatile uint32_t CM_TECDIV;		// 0x0cc
  volatile uint32_t CM_TD0CTL;		// 0x0d0
  volatile uint32_t CM_TD0DIV;		// 0x0d4
  volatile uint32_t CM_TD1CTL;		// 0x0d8
  volatile uint32_t CM_TD1DIV;		// 0x0dc
  volatile uint32_t CM_TSENSCTL;	// 0x0e0
  volatile uint32_t CM_TSENSDIV;	// 0x0e4
  volatile uint32_t CM_TIMERCTL;	// 0x0e8
  volatile uint32_t CM_TIMERDIV;	// 0x0ec
  volatile uint32_t CM_UARTCTL;	// 0x0f0
  volatile uint32_t CM_UARTDIV;	// 0x0f4
  volatile uint32_t CM_VECCTL;		// 0x0f8
  volatile uint32_t CM_VECDIV;		// 0x0fc
  volatile uint32_t RESERVED3[36]; // 0x100 ~ 0x18f
  volatile uint32_t CM_PULSECTL;	// 0x190
  volatile uint32_t CM_PULSEDIV;	// 0x194
  volatile uint32_t RESERVE4[4];   // 0x198 ~ 0x1a7
  volatile uint32_t CM_SDCCTL;		// 0x1a8
  volatile uint32_t CM_SDCDIV;		// 0x1ac
  volatile uint32_t CM_ARMCTL;		// 0x1b0
  volatile uint32_t RESERVED5;     // 0x1b4
  volatile uint32_t CM_AVEOCTL;	// 0x1b8
  volatile uint32_t CM_AVEODIV;	// 0x1bc
  volatile uint32_t CM_EMMCCTL;	// 0x1c0
  volatile uint32_t CM_EMMCDIV;	// 0x1c4
} CLK_Typedef;

}
}

#endif