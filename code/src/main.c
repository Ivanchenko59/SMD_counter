#include "main.h"

volatile uint32_t smd_counter = 0, smd_counter_new = 0;

int main() {
	
	mcu_init();
	welcome_menu();
	
	while(1) {
		if (state == STATE_COUNT_MENU) {
			NVIC_EnableIRQ(EXTI4_IRQn);
		}
		else {
			NVIC_DisableIRQ(EXTI4_IRQn);
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

	smd_counter++;
	smd_counter_new++;
	
	EXTI->PR |= EXTI_PR_PR4;
}
