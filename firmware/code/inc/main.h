#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "menu.h"
#include "utils.h"
#include "button.h"
#include "fonts.h"
#include "ssd1306_i2c.h"
#include "ssd1306.h"


#define MIN_BATTERY_VOLTAGE 1200
#define MAX_BATTERY_VOLTAGE 3200
#define BATTERY_PERCENT ((3300*battery_voltage)/4096  - MIN_BATTERY_VOLTAGE) * (100 - 0) / (MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE)

extern volatile uint32_t front_counter, front_counter_continue, smd_counter, smd_counter_continue; 
extern uint8_t skip_first_front;
extern uint16_t battery_voltage;
void mcu_init(void);
#endif /* __MAIN_H__ */
