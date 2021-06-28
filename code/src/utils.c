#include "utils.h"

volatile uint32_t ticks_delay = 0;

void delay(const uint32_t milliseconds) {
	uint32_t start = ticks_delay;
	while((ticks_delay - start) < milliseconds);
}


void button_init() {
	//clocking PORT B and AFIO for enable AFIO multiplexer 
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	
	//http://easyelectronics.ru/arm-uchebnyj-kurs-vneshnie-preryvaniya.html
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PA | AFIO_EXTICR3_EXTI10_PB | AFIO_EXTICR3_EXTI11_PB;
	
	//OK button
	GPIOB->CRH &= ~GPIO_CRH_MODE10;
	GPIOB->CRH |= GPIO_CRH_CNF10_1;
	GPIOB->CRH &= ~GPIO_CRH_CNF10_0;
	EXTI->IMR  |= EXTI_IMR_MR10;
	EXTI->RTSR |= EXTI_RTSR_TR10;
	EXTI->FTSR |= EXTI_FTSR_TR10;
	//RIGHT button
	GPIOB->CRH &= ~GPIO_CRH_MODE11;
	GPIOB->CRH |= GPIO_CRH_CNF11_1;
	GPIOB->CRH &= ~GPIO_CRH_CNF11_0;
	EXTI->IMR  |= EXTI_IMR_MR11;
	EXTI->RTSR |= EXTI_RTSR_TR11;
	//LEFT button
	GPIOA->CRH &= ~GPIO_CRH_MODE8;
	GPIOA->CRH |= GPIO_CRH_CNF8_1;
	GPIOA->CRH &= ~GPIO_CRH_CNF8_0;
	EXTI->IMR  |= EXTI_IMR_MR8;
	EXTI->RTSR |= EXTI_RTSR_TR8;
	
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void tmr_init() {
	//timer for long press button 
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = (SystemCoreClock / 2) / 1000 - 1; 
	TIM3->ARR = 4000 - 1;
}
