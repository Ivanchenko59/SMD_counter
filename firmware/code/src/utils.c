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
	GPIOB->ODR |= GPIO_ODR_ODR4; 				//pullup
	
 	AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI4_PB;	//switch EXTI multiplexer to PB4
	NVIC_EnableIRQ(EXTI4_IRQn);
	EXTI->RTSR |= EXTI_RTSR_TR4;
	EXTI->IMR |= EXTI_IMR_MR4;
}
