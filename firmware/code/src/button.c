#include "button.h"

#define BUTTON_SHORT_PRESS_DELAY 150
#define BUTTON_LONG_PRESS_DELAY 1000
#define STEP_SIZE_MAX 8

uint32_t ok_time = 0;
uint32_t left_time = 0;
uint32_t right_time = 0;

typedef struct {   
	__IO uint8_t ok_state : 1;
	__IO uint8_t ok_short_state : 1;
	__IO uint8_t ok_long_state : 1;
	__IO uint8_t left_press  : 1;
	__IO uint8_t right_press : 1;
} bit_fied_TypeDef;

bit_fied_TypeDef btn = {0, 0, 0, 1, 1};

void button_init() {
	//clocking PORT A, PORT B and AFIO for enable AFIO multiplexer 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN 
				 | RCC_APB2ENR_IOPBEN 
				 | RCC_APB2ENR_AFIOEN;
	
	//http://easyelectronics.ru/arm-uchebnyj-kurs-vneshnie-preryvaniya.html
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PA 
					| AFIO_EXTICR3_EXTI10_PB 
					| AFIO_EXTICR3_EXTI11_PB;
	
	//Init OK button
	GPIOB->CRH &= ~GPIO_CRH_MODE10;
	GPIOB->CRH |= GPIO_CRH_CNF10_1;
	GPIOB->CRH &= ~GPIO_CRH_CNF10_0;
		
	//Init RIGHT button
	GPIOB->CRH &= ~GPIO_CRH_MODE11;
	GPIOB->CRH |= GPIO_CRH_CNF11_1;
	GPIOB->CRH &= ~GPIO_CRH_CNF11_0;
	
	//Init LEFT button
	GPIOA->CRH &= ~GPIO_CRH_MODE8;
	GPIOA->CRH |= GPIO_CRH_CNF8_1;
	GPIOA->CRH &= ~GPIO_CRH_CNF8_0;
}

void button_check() {
	
	btn.ok_state = ((GPIOB->IDR & GPIO_IDR_IDR10) == GPIO_IDR_IDR10);
	
	if (btn.ok_state && !btn.ok_short_state && ((sys_tick - ok_time) > BUTTON_SHORT_PRESS_DELAY)) {
		btn.ok_short_state = 1;
		btn.ok_long_state = 0;
		ok_time = sys_tick;
	}
	else if (btn.ok_state && !btn.ok_long_state && (sys_tick - ok_time) > BUTTON_LONG_PRESS_DELAY) {
		btn.ok_long_state = 1;
		event = EVENT_BTN_LONG;
	}
	else if (!btn.ok_state && btn.ok_short_state && (sys_tick - ok_time) > BUTTON_SHORT_PRESS_DELAY) {
		btn.ok_short_state = 0;
		ok_time = sys_tick;
		if(!btn.ok_long_state) {
			event = EVENT_BTN_SHORT;
		}
	}
	
	//if we are not in the line menu, do not check the left and right buttons
	if (state == STATE_SAVE_MENU || state == STATE_SETTING_MENU || state == STATE_DATABASE_MENU) {

		//LEFT button
		if ((GPIOA->IDR & GPIO_IDR_IDR8) == GPIO_IDR_IDR8 && btn.left_press) {
			btn.left_press = 0;
			if (edit_step_flag){
				step_size = constrain(step_size - 1, 1, STEP_SIZE_MAX);
			}
			else if (edit_counter_flag) {
				if (smd_counter > 0) {
					smd_counter--;
				}
			}
			else if (edit_db_flag) {
				if (database[pointer] > 0) {
					database[pointer]--;
				}
			}
			else {
				pointer--;
			}
			left_time = sys_tick;
		}
		if (!btn.left_press && (sys_tick - left_time) > BUTTON_SHORT_PRESS_DELAY) {
			btn.left_press = 1;
		}
		
		//RIGHT button
		if ((GPIOB->IDR & GPIO_IDR_IDR11) == GPIO_IDR_IDR11 && btn.right_press) {
			btn.right_press = 0;
			if (edit_step_flag){
				step_size = constrain(step_size + 1, 1, STEP_SIZE_MAX);
			}
			else if (edit_counter_flag) {
				smd_counter++;
			}
			else if (edit_db_flag) {
				database[pointer]++;
			}
			else {
				pointer++;
			}
			right_time = sys_tick;
		}
		if (!btn.right_press && (sys_tick - right_time) > BUTTON_SHORT_PRESS_DELAY) {
			btn.right_press = 1;
		}
	}
	
	else {
		pointer = 0;
	}
}
