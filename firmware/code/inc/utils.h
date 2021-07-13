#ifndef __UTILS_H__
#define __UTILS_H__

#include "stm32f10x.h"

extern volatile uint32_t sys_tick;

//void pll_init(void);
void delay(uint32_t milliseconds);
void TCST_init(void);
void ADC1_init(void);
void TMR4_init(void);
uint16_t get_battery_voltage(void);
uint16_t expRunningAverage(uint16_t newVal);
#endif /* __UTILS_H__ */
