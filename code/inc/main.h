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
#include "ssd1306.h"
#include "ssd1306_i2c.h"

extern volatile int8_t pointer;
extern volatile uint32_t smd_counter;

void mcu_init(void);
void button_timer_start(void);
void button_timer_stop(void);
uint32_t button_timer_get_value(void);
#endif /* __MAIN_H__ */
