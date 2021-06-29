#include "utils.h"

volatile uint32_t ticks_delay = 0;

void delay(const uint32_t milliseconds) {
	uint32_t start = ticks_delay;
	while((ticks_delay - start) < milliseconds);
}

void TCST_init() {
	
	//config GPIOB4 to input mode with pull up/pull down
	GPIOB->CRL &= ~GPIO_CRL_MODE4;
	GPIOB->CRL |= GPIO_CRL_CNF4_1;
	GPIOB->CRL &= ~GPIO_CRL_CNF4_0;
	//input with pull up == 1
	GPIOB->ODR |= GPIO_ODR_ODR4; 
	
	//switch EXTI multiplexer to PB4
 	AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI4_PB;
	
	NVIC_EnableIRQ(EXTI4_IRQn);
	EXTI->IMR |= EXTI_IMR_MR4;
	EXTI->RTSR |= EXTI_RTSR_TR4;
	EXTI->FTSR |= EXTI_FTSR_TR4;
	
}

void tmr_init() {
	//timer for long press button 
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = (SystemCoreClock / 100) - 1;
	TIM3->ARR = 10000 - 1;
}
