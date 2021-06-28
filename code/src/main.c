#include "main.h"
#define BUTTON_LONG_PRESS_DELAY 800
volatile int8_t pointer = 0;

int main() {
	
	mcu_init();
	
	while(1) {
		SSD1306_Fill(SSD1306_COLOR_BLACK); 
		transition_table[state][event]();
		SSD1306_UpdateScreen();
	}
}

void mcu_init() {
	SysTick_Config(SystemCoreClock/1000);	
	SSD1306_Init();
	button_init();
	tmr_init();
}	

inline void button_timer_start(void) {
	TIM3->CNT = 0;
	TIM3->CR1 |= TIM_CR1_CEN;                     // start timer
}

inline void button_timer_stop(void) {

	TIM3->CR1 &= ~TIM_CR1_CEN;
}

inline uint32_t button_timer_get_value(void){
	return TIM3->CNT;
}


void SysTick_Handler() {
	ticks_delay++;
}

void EXTI9_5_IRQHandler(void) {
	//left button handler 
	if ((EXTI->PR & EXTI_PR_PR8) ==  EXTI_PR_PR8) {
	
		if (state == STATE_SAVE_MENU) {
			pointer--;
		}
		
		EXTI->PR |= EXTI_PR_PR8;
	}
}
void EXTI15_10_IRQHandler(void) {
	
	//OK button handler 
	if ((EXTI->PR & EXTI_PR_PR10) ==  EXTI_PR_PR10) {
		
		if ((GPIOB->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10) { // rising edge
			button_timer_start();
		} 
		else { // falling egde
			button_timer_stop();
			if (TIM3->CNT < BUTTON_LONG_PRESS_DELAY) {
				// short press action here
				event = EVENT_BTN_SHORT;
			} 
			else {
				// long press action here
				event = EVENT_BTN_LONG;
			}
		}
		EXTI->PR |= EXTI_PR_PR10;
	}

	//right button handler 
	else if ((EXTI->PR & EXTI_PR_PR11) ==  EXTI_PR_PR11) {
		
		if (state == STATE_SAVE_MENU) {
			pointer++;
		}
		
		EXTI->PR |= EXTI_PR_PR11;
	}
}
