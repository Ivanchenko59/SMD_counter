#include "utils.h"

volatile uint32_t sys_tick = 0;

void delay(const uint32_t milliseconds) {
	uint32_t start = sys_tick;
	while((sys_tick - start) < milliseconds);
}

void TCST_init() {
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	
	//config GPIOB4 to input mode with pull up/pull down
	GPIOB->CRL &= ~GPIO_CRL_MODE4;
	GPIOB->CRL |= GPIO_CRL_CNF4_1;
	GPIOB->CRL &= ~GPIO_CRL_CNF4_0;
	//input with pull up == 1
	GPIOB->ODR |= GPIO_ODR_ODR4; 
	
	//switch EXTI multiplexer to PB4
 	AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI4_PB;
	EXTI->IMR |= EXTI_IMR_MR4;
	EXTI->RTSR |= EXTI_RTSR_TR4;
	EXTI->FTSR |= EXTI_FTSR_TR4;
}
