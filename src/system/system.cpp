#include "rpl4/system/system.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

namespace rpl {

GPIO_Typedef* REG_GPIO;
PWM_Typedef*  REG_PWM0;
PWM_Typedef*  REG_PWM1;
CLK_Typedef*  REG_CLK;

uint8_t Init(void){
	int fd;
	if ((fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0) {
        Log(LogLevel::Fatal, "Can't open /dev/mem");
        return -1;
    }

	REG_GPIO = (GPIO_Typedef*) mmap(NULL, GPIO_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	REG_PWM0 = (PWM_Typedef*)  mmap(NULL,  PWM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, PWM0_BASE);
	REG_PWM1 = (PWM_Typedef*)  mmap(NULL,  PWM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, PWM1_BASE);
	REG_CLK  = (CLK_Typedef*)  mmap(NULL,  CLK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, CLK_BASE);
	return 0;
}

uint8_t SetGpioFunction(uint8_t pin, GPIO_Function alt){
    uint32_t bit_mode = static_cast<uint32_t>(alt);

    if (0 <= pin && pin <= 9) {
        REG_GPIO->GPFSEL0 &= ~(0b111 << pin*3);
        REG_GPIO->GPFSEL0 |= bit_mode << pin*3;
    }
    else if (10 <= pin && pin <= 19) {
        REG_GPIO->GPFSEL1 &= ~(0b111 << (pin-10)*3);
        REG_GPIO->GPFSEL1 |= bit_mode << (pin-10)*3;
    }
    else if (20 <= pin && pin <= 29) {
        REG_GPIO->GPFSEL2 &= ~(0b111 << (pin-20)*3);
        REG_GPIO->GPFSEL2 |= bit_mode << (pin-20)*3;
    }
    else if (30 <= pin && pin <= 39) {
        REG_GPIO->GPFSEL3 &= ~(0b111 << (pin-30)*3);
        REG_GPIO->GPFSEL3 |= bit_mode << (pin-30)*3;
    }
    else if (40 <= pin && pin <= 49) {
        REG_GPIO->GPFSEL4 &= ~(0b111 << (pin-40)*3);
        REG_GPIO->GPFSEL4 |= bit_mode << (pin-40)*3;
    }
    else if (50 <= pin && pin <= 57) {
        REG_GPIO->GPFSEL5 &= ~(0b111 << (pin-50)*3);
        REG_GPIO->GPFSEL5 |= bit_mode << (pin-50)*3;
    }

	return bit_mode;
}

}