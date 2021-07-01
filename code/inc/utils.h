#ifndef __UTILS_H__
#define __UTILS_H__

#include "stm32f10x.h"

extern volatile uint32_t sys_tick;

//void pll_init(void);
void delay(uint32_t milliseconds);
void TCST_init(void);

#endif /* __UTILS_H__ */
