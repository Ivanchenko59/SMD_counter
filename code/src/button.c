#include "button.h"

#define BUTTON_SHORT_PRESS_DELAY 150
#define BUTTON_LONG_PRESS_DELAY 1000

uint8_t short_state = 0;
uint8_t long_state = 0;
uint32_t time_key1 = 0;

uint8_t flag_key2_press = 1;
uint32_t time_key2_press = 0;

uint8_t flag_key3_press = 1;
uint32_t time_key3_press = 0;

void button_init() {
	//clocking PORT A, PORT B and AFIO for enable AFIO multiplexer 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	
	//http://easyelectronics.ru/arm-uchebnyj-kurs-vneshnie-preryvaniya.html
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PA | AFIO_EXTICR3_EXTI10_PB | AFIO_EXTICR3_EXTI11_PB;
	
	//OK button
	GPIOB->CRH &= ~GPIO_CRH_MODE10;
	GPIOB->CRH |= GPIO_CRH_CNF10_1;
	GPIOB->CRH &= ~GPIO_CRH_CNF10_0;
	//EXTI->IMR  |= EXTI_IMR_MR10;
	//EXTI->RTSR |= EXTI_RTSR_TR10;
	//EXTI->FTSR |= EXTI_FTSR_TR10;
	//RIGHT button
	GPIOB->CRH &= ~GPIO_CRH_MODE11;
	GPIOB->CRH |= GPIO_CRH_CNF11_1;
	GPIOB->CRH &= ~GPIO_CRH_CNF11_0;
	//EXTI->IMR  |= EXTI_IMR_MR11;
	//EXTI->RTSR |= EXTI_RTSR_TR11;
	//LEFT button
	GPIOA->CRH &= ~GPIO_CRH_MODE8;
	GPIOA->CRH |= GPIO_CRH_CNF8_1;
	GPIOA->CRH &= ~GPIO_CRH_CNF8_0;
	//EXTI->IMR  |= EXTI_IMR_MR8;
	//EXTI->RTSR |= EXTI_RTSR_TR8;
	
	//NVIC_EnableIRQ(EXTI9_5_IRQn);
	//NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void button_check() {
	uint32_t ms = ticks_delay;
	uint8_t key1_state = ((GPIOB->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10);
	
	if(key1_state == 1 && !short_state && ((ms - time_key1) > BUTTON_SHORT_PRESS_DELAY)) 
	{
		short_state = 1;
		long_state = 0;
		time_key1 = ms;
	}
	else if(key1_state == 1 && !long_state && (ms - time_key1) > BUTTON_LONG_PRESS_DELAY) 
	{
		long_state = 1;
		event = EVENT_BTN_LONG;
	}
	else if(key1_state == 0 && short_state && (ms - time_key1) > BUTTON_SHORT_PRESS_DELAY) {
		short_state = 0;
		time_key1 = ms;

		if(!long_state) {
			event = EVENT_BTN_SHORT;
		}
	}

	
	if(((GPIOB->IDR & GPIO_IDR_IDR11) == GPIO_IDR_IDR11) == 1 && flag_key2_press) {
		flag_key2_press = 0;
		// действие на нажатие
		event = pointer++;
		time_key2_press = ticks_delay;
	}

	if(!flag_key2_press && (ticks_delay - time_key2_press) > BUTTON_SHORT_PRESS_DELAY) {
		flag_key2_press = 1;
	}
	
	
	if(((GPIOA->IDR & GPIO_IDR_IDR8) == GPIO_IDR_IDR8) == 1 && flag_key3_press) {
		flag_key3_press = 0;
		// действие на нажатие
		event = pointer--;
		time_key3_press = ticks_delay;
	}

	if(!flag_key3_press && (ticks_delay - time_key3_press) > BUTTON_SHORT_PRESS_DELAY) {
		flag_key3_press = 1;
	}
}
