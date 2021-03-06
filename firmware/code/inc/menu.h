#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f10x.h"
#include "main.h"

typedef void (*TRANSITION_FUNC_PTR_t)(void);

typedef enum {
	STATE_MAIN_MENU,		// 0
	STATE_COUNT_MENU,		// 1
	STATE_SETTING_MENU,		// 2
	STATE_SAVE_MENU,		// 3
	STATE_DATABASE_MENU,	// 4
	STATE_MAX,				// 5
} STATE_t;

typedef enum {
	EVENT_NONE, 			// 0
	EVENT_BTN_SHORT,		// 1
	EVENT_BTN_LONG,			// 2
	EVENT_TIME_OUT, 		// 3
	EVENT_MAX, 				// 4
} EVENT_t;

typedef enum {
	SAVE_TO_DATABASE,		// 0
	EDIT_AND_SAVE,			// 1
	CONTINUE,				// 2
	CANCEL,					// 3
	SAVE_MENU_MAX,			// 4
} SAVE_MENU_t;

typedef enum {
	STEP_COUNTER,			// 0
	DATABASE,				// 1
	EXPORT_TO_PC,			// 2
	EXIT,					// 3
	SETTING_MENU_MAX,		// 4
} SETTING_MENU_t;

extern int8_t pointer;
extern uint8_t state, event, step_size, edit_step_flag, edit_db_flag, edit_counter_flag;
extern uint32_t database[];
extern TRANSITION_FUNC_PTR_t transition_table[STATE_MAX][EVENT_MAX];

uint8_t constrain(int8_t x, uint8_t min, uint8_t max); 		//return a value between min and max
uint8_t fixtext(uint8_t x);
void welcome_menu(void);
void main_menu(void);
void count_menu(void);
void setting_menu(void);
void edit_menu(void);
void save_menu(void);
void database_menu(void);
void save_to_data_base(void);

#endif /* __MENU_H__ */
