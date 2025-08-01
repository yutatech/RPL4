#!/usr/bin/env python3

import matplotlib.pyplot as plt

map = [
    ["Aux", 0xFE215000, 0xFE215100, "Aux"],
    #
    ["Spi1", 0xFE215080, 0xFE2150C0, "AuxSpi"],
    ["Spi2", 0xFE2150C0, 0xFE215100, "AuxSpi"],
    #
    ["Bsc0", 0xFE205000, 0xFE205020, "Bsc"],
    ["Bsc1", 0xFE804000, 0xFE804020, "Bsc"],
    ["Bsc3", 0xFE205600, 0xFE205620, "Bsc"],
    ["Bsc4", 0xFE205800, 0xFE205820, "Bsc"],
    ["Bsc5", 0xFE205A80, 0xFE205AA0, "Bsc"],
    ["Bsc6", 0xFE205C00, 0xFE205C20, "Bsc"],
    #
    ["Clk", 0xFE101000, 0xFE1011C8, "Clk"],
    #
    ["Dma0", 0xFE007000, 0xFE007020, "Dma"],
    ["Dma1", 0xFE007100, 0xFE007120, "Dma"],
    ["Dma2", 0xFE007200, 0xFE007220, "Dma"],
    ["Dma3", 0xFE007300, 0xFE007320, "Dma"],
    ["Dma4", 0xFE007400, 0xFE007420, "Dma"],
    ["Dma5", 0xFE007500, 0xFE007520, "Dma"],
    ["Dma6", 0xFE007600, 0xFE007620, "Dma"],
    ["Dma7", 0xFE007700, 0xFE007720, "Dma"],
    ["Dma8", 0xFE007800, 0xFE007820, "Dma"],
    ["Dma9", 0xFE007900, 0xFE007920, "Dma"],
    ["Dma10", 0xFE007A00, 0xFE007A20, "Dma"],
    ["Dma11", 0xFE007B00, 0xFE007B20, "Dma"],
    ["Dma12", 0xFE007C00, 0xFE007C20, "Dma"],
    ["Dma13", 0xFE007D00, 0xFE007D20, "Dma"],
    ["Dma14", 0xFEE05000, 0xFEE05020, "Dma"],
    #
    ["Gpio", 0xFE200000, 0xFE2000F4, "Gpio"],
    #
    ["Pwm0", 0xFE20C000, 0xFE20C028, "Pwm"],
    ["Pwm1", 0xFE20C800, 0xFE20C828, "Pwm"],
    #
    ["Spi0", 0xFE204000, 0xFE204018, "Spi"],
    ["Spi3", 0xFE204600, 0xFE204618, "Spi"],
    ["Spi4", 0xFE204800, 0xFE204818, "Spi"],
    ["Spi5", 0xFE204A00, 0xFE204A18, "Spi"],
    ["Spi6", 0xFE204C00, 0xFE204C18, "Spi"],
    #
    ["Uart0", 0xFE201000, 0xFE201090, "Uart"],
    ["Uart2", 0xFE201400, 0xFE201490, "Uart"],
    ["Uart3", 0xFE201600, 0xFE201690, "Uart"],
    ["Uart4", 0xFE201800, 0xFE201890, "Uart"],
    ["Uart5", 0xFE201A00, 0xFE201A90, "Uart"],
    #
    ["Gic400", 0xFF840000, 0xFF8C0000, "Gic400"],
]

# for idx in map:
#   print(idx[0] + ', {:X}, {:X}'.format(idx[1], idx[2]))

# 第二要素（index 1）を基準にソート
sorted_map = sorted(map, key=lambda x: x[1])

region_list = []
for i in range(0xFF000):
    idx = []
    for reg in map:
        if i * 0x1000 <= reg[1] and reg[1] < (i + 1) * 0x1000:
            idx.append(reg)
    if len(idx) != 0:
        region_list.append([i, idx, 1])

for idx in region_list:
    print(idx)

i = 1
while True:
    if i == len(region_list):
        break
    if region_list[i][0] - region_list[i - 1][0] == 1:
        region_list[i - 1][1] += region_list[i][1]
        region_list[i - 1][2] += region_list[i][2]
        region_list.pop(i)
    else:
        i += 1
print("---")
for idx in region_list:
    print(idx)
print("---")

counter = 0
for idx in region_list:
    print(
        "constexpr static uint32_t region"
        + str(counter)
        + "_base = "
        + hex(idx[0] * 0x1000)
        + ";"
    )
    print(
        "constexpr static uint32_t region"
        + str(counter)
        + "_size = "
        + hex(idx[2] * 0x1000)
        + ";"
    )
    print(
        "uint32_t* region"
        + str(counter)
        + " = reinterpret_cast<uint32_t*>(mmap(NULL, region"
        + str(counter)
        + "_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, region"
        + str(counter)
        + "_base));"
    )
    for reg in idx[1]:
        print(
            "REG_"
            + reg[0].upper()
            + " = reinterpret_cast<"
            + reg[3]
            + "RegisterMap*>(region"
            + str(counter)
            + " + (k"
            + reg[0]
            + "AddressBase - region"
            + str(counter)
            + "_base) / 4);"
        )

    print()
    counter += 1
