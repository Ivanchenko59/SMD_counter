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

void ADC1_init() {
	//ADCPRE in system_stm32f10x.c, config to 12 MHz
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_ADC1EN;
	GPIOA->CRL &= ~GPIO_CRL_MODE2;
	GPIOA->CRL &= ~GPIO_CRL_CNF2;

	// Независимый режим работы
	ADC1->CR1 &= ~ADC_CR1_DUALMOD;
	// Устанавливаем количество каналов (1)
	ADC1->CR1 &= ~ADC_CR1_DUALMOD;
	// Однократный режим
	ADC1->CR1 &= ~ADC_CR1_DISCEN;
	// Отключаем многоканальный режим
	ADC1->CR1 &= ~ADC_CR1_SCAN;
	// Однократный режим
	ADC1->CR2 &= ~ADC_CR2_CONT;
	// Выравнивание к правой стороне
	ADC1->CR2 &= ~ADC_CR2_ALIGN;
	// Отключаем преобразование по триггеру
	ADC1->CR2 &= ~ADC_CR2_EXTTRIG;
	// Регулярные каналы, последовательность из 1 канала
	ADC1->SQR1 &= ~ADC_SQR1_L;
	// Настройка регулярного канала
	ADC1->SMPR2 &= ~ADC_SMPR2_SMP0;
	ADC1->SMPR2 |= ADC_SMPR2_SMP0_0;
	ADC1->SQR3 |= ADC_SQR3_SQ1_0; // one channel
	// Включаем АЦП
	ADC1->CR2 |= ADC_CR2_ADON;

	// Сбрасываем калибровку
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	while((ADC1->CR2 & ADC_CR2_RSTCAL) == ADC_CR2_RSTCAL);

	// Запускаем калибровку
	ADC1->CR2 |= ADC_CR2_CAL;
	while((ADC1->CR2 & ADC_CR2_CAL) != ADC_CR2_CAL);

}

uint16_t get_battery_voltage(void) {
    // Запуск преобразования и ожидание его завершения
	ADC1->CR2 |= ADC_CR2_ADON;
	while((ADC1->CR2 & ADC_SR_EOC) == ADC_SR_EOC);

	return ADC1->DR;
}

