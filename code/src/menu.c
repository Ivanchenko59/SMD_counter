#include "menu.h"

#define START_X 4
#define START_Y 4
#define STEP_X 5
#define STEP_Y 16

char str[14];
uint8_t state = 0, event = 0;


typedef void (*TRANSITION_FUNC_PTR_t)(void);

TRANSITION_FUNC_PTR_t transition_table[STATE_MAX][EVENT_MAX] = {
	[STATE_MAIN_MENU]		[EVENT_NONE] 		=	main_menu,
	[STATE_MAIN_MENU]		[EVENT_BTN_SHORT] 	=	count_menu,
	[STATE_MAIN_MENU]		[EVENT_BTN_LONG] 	=	setting_menu,
	[STATE_MAIN_MENU]		[EVENT_TIME_OUT] 	=	main_menu,		//nothing to do
	
	[STATE_COUNT_MENU]		[EVENT_NONE] 		=	count_menu,
	[STATE_COUNT_MENU]		[EVENT_BTN_SHORT] 	=	count_menu,		//nothing to do
	[STATE_COUNT_MENU]		[EVENT_BTN_LONG] 	=	save_menu,
	[STATE_COUNT_MENU]		[EVENT_TIME_OUT] 	=	save_menu,
	
	[STATE_SAVE_MENU]		[EVENT_NONE] 		=	save_menu,
	[STATE_SAVE_MENU]		[EVENT_BTN_SHORT] 	=	save_menu,		//SOMETHING TO DO??
	[STATE_SAVE_MENU]		[EVENT_BTN_LONG] 	=	save_menu,
	[STATE_SAVE_MENU]		[EVENT_TIME_OUT] 	=	save_menu,		//nothing to do
	
	[STATE_SETTING_MENU]	[EVENT_NONE] 		=	setting_menu,
	[STATE_SETTING_MENU]	[EVENT_BTN_SHORT] 	=	setting_menu,	//SOMETHING TO DO??
	[STATE_SETTING_MENU]	[EVENT_BTN_LONG] 	=	main_menu,
	[STATE_SETTING_MENU]	[EVENT_TIME_OUT] 	=	setting_menu,	//SOMETHING TO DO??	
};

 
void main_menu(void) {
	state = STATE_MAIN_MENU;
	SSD1306_GotoXY(30, 2);
    SSD1306_Puts("Main menu", &Font_7x10, SSD1306_COLOR_WHITE);
	
	sprintf(str, "%u", smd_counter);
	SSD1306_GotoXY(24, 16);
	SSD1306_Puts(str, &Font_16x26, SSD1306_COLOR_WHITE);
	
	SSD1306_GotoXY(2, 50);
	SSD1306_Puts("Settings", &Font_7x10,SSD1306_COLOR_WHITE);
	
	event = EVENT_NONE;
}

void count_menu(void) {
	state = STATE_COUNT_MENU;
	SSD1306_GotoXY(1, 10);
    SSD1306_Puts("Count menu", &Font_11x18, SSD1306_COLOR_WHITE);
	event = EVENT_NONE;
}

void save_menu(void) {
	state = STATE_SAVE_MENU;
	pointer = constrain(pointer, 0, SAVE_MENU_MAX - 1);
	SSD1306_DrawRectangle(0, 0, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_COLOR_WHITE);					//border line
	SSD1306_DrawFilledRectangle(0, 1 + STEP_Y * pointer, SSD1306_WIDTH, 13, SSD1306_COLOR_WHITE);		//pointer line
	
	SSD1306_GotoXY(START_X, START_Y);
	SSD1306_Puts("Save to database", &Font_7x10, (SSD1306_COLOR_t)(pointer != SAVE_TO_DATABASE));
	SSD1306_GotoXY(START_X, START_Y + STEP_Y);
	SSD1306_Puts("Edit and save", &Font_7x10, (SSD1306_COLOR_t)(pointer != EDIT_AND_SAVE));
	SSD1306_GotoXY(START_X, START_Y + 2 * STEP_Y);
	SSD1306_Puts("Continue counting", &Font_7x10, (SSD1306_COLOR_t)(pointer != CONTINUE));
	SSD1306_GotoXY(START_X, START_Y	+ 3 * STEP_Y);
	SSD1306_Puts("Exit", &Font_7x10, (SSD1306_COLOR_t)(pointer != CANCEL));

	if (event == EVENT_BTN_SHORT) {
		switch (pointer) {
			case SAVE_TO_DATABASE:
				break;
			case EDIT_AND_SAVE:
				break;
			case CONTINUE:
				break;
			case CANCEL:
				state = STATE_MAIN_MENU;
				break;
		}
	}
	event = EVENT_NONE;
	
}

void edit_menu(void) {
	state = STATE_COUNT_MENU;
	SSD1306_GotoXY(1, 10);
	SSD1306_Puts("Edit menu", &Font_11x18, SSD1306_COLOR_WHITE);
	//edit LFT RGHT
	// << N >>
	//press ok to save
	//print("N save to database");
	//delay(1000);

	event = EVENT_NONE;
}

void setting_menu(void) {
	SSD1306_GotoXY(1, 10);
    SSD1306_Puts("Settings", &Font_11x18, SSD1306_COLOR_WHITE);
	state = STATE_SETTING_MENU;
	event = EVENT_NONE;
}

uint8_t constrain(int8_t x, uint8_t min, uint8_t max) {
	if (x >= max) {
		return max;
	}
	else if (x < min) {
		return min;
	} 
	else {
		return x;
	}
}
