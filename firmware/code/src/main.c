#include "main.h"

volatile uint32_t front_counter = 0, front_counter_continue = 0, smd_counter, smd_counter_continue;
uint8_t skip_first_front = 0;

int main() {
	
	mcu_init();
	welcome_menu();
	
	while(1) {
		if (state == STATE_COUNT_MENU) {
			EXTI->IMR |= EXTI_IMR_MR4;
		}
		else {
			EXTI->IMR &= ~EXTI_IMR_MR4;
		}
		
		SSD1306_Fill(SSD1306_COLOR_BLACK);
		
		transition_table[state][event]();
		button_check();
		
		SSD1306_UpdateScreen();
	}
}

void mcu_init() {
	SysTick_Config(SystemCoreClock/1000);	
	SSD1306_Init();
	button_init();
	TCST_init();
}	

void SysTick_Handler() {
	sys_tick++;
}

void EXTI4_IRQHandler(void) {
	//clear the bit at the beginning, because at the end the bit "does not have time to be cleared"
	EXTI->PR |= EXTI_PR_PR4;
	if (skip_first_front > 0) {
		front_counter++;
		front_counter_continue++;
		smd_counter = front_counter / step_size;
	}
	skip_first_front = 1;
}
