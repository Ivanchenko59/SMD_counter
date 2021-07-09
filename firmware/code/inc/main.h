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

extern volatile uint32_t front_counter, front_counter_continue, smd_counter, smd_counter_continue; 

void mcu_init(void);
#endif /* __MAIN_H__ */
