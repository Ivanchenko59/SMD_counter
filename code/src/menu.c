#include "menu.h"

#define DATABASE_SIZE 13
#define START_X 4
#define START_Y 4
#define STEP_X 5
#define STEP_Y 16

char str[14];
int8_t pointer = 0;
uint8_t state = 0, event = 0;
uint8_t step_size = 1, db_element = 0;
uint8_t edit_step_flag = 0, continue_counting_flag = 0, edit_db_flag = 0, edit_counter_flag = 0;
uint32_t database[DATABASE_SIZE] = {0};


typedef void (*TRANSITION_FUNC_PTR_t)(void);

TRANSITION_FUNC_PTR_t transition_table[STATE_MAX][EVENT_MAX] = {
	[STATE_MAIN_MENU]		[EVENT_NONE] 		=	main_menu,
	[STATE_MAIN_MENU]		[EVENT_BTN_SHORT] 	=	count_menu,
	[STATE_MAIN_MENU]		[EVENT_BTN_LONG] 	=	setting_menu,
	[STATE_MAIN_MENU]		[EVENT_TIME_OUT] 	=	main_menu,
	
	[STATE_COUNT_MENU]		[EVENT_NONE] 		=	count_menu,
	[STATE_COUNT_MENU]		[EVENT_BTN_SHORT] 	=	count_menu,		
	[STATE_COUNT_MENU]		[EVENT_BTN_LONG] 	=	save_menu,
	[STATE_COUNT_MENU]		[EVENT_TIME_OUT] 	=	save_menu,
	
	[STATE_SAVE_MENU]		[EVENT_NONE] 		=	save_menu,
	[STATE_SAVE_MENU]		[EVENT_BTN_SHORT] 	=	save_menu,		
	[STATE_SAVE_MENU]		[EVENT_BTN_LONG] 	=	save_menu,
	[STATE_SAVE_MENU]		[EVENT_TIME_OUT] 	=	save_menu,
	
	[STATE_SETTING_MENU]	[EVENT_NONE] 		=	setting_menu,
	[STATE_SETTING_MENU]	[EVENT_BTN_SHORT] 	=	setting_menu,	
	[STATE_SETTING_MENU]	[EVENT_BTN_LONG] 	=	setting_menu,
	[STATE_SETTING_MENU]	[EVENT_TIME_OUT] 	=	setting_menu,
	
	[STATE_DATABASE_MENU]	[EVENT_NONE] 		=	database_menu,
	[STATE_DATABASE_MENU]	[EVENT_BTN_SHORT] 	=	database_menu,	
	[STATE_DATABASE_MENU]	[EVENT_BTN_LONG] 	=	setting_menu,
	[STATE_DATABASE_MENU]	[EVENT_TIME_OUT] 	=	database_menu,
};

void welcome_menu(void) {	
	for (uint16_t x = 0; x < 128; x += 2) {
		SSD1306_Fill(SSD1306_COLOR_BLACK);
		SSD1306_GotoXY(2, 0); 
		SSD1306_Puts("SMD Counter", &Font_11x18, SSD1306_COLOR_WHITE);
		SSD1306_GotoXY(0, 52); 
		SSD1306_Puts("Settings - HOLD OK", &Font_7x10, SSD1306_COLOR_WHITE);
		SSD1306_DrawFilledRectangle(x, 25, 60, 20, SSD1306_COLOR_WHITE);
		SSD1306_DrawFilledCircle(10+x, 35, 7, SSD1306_COLOR_BLACK);
		SSD1306_DrawFilledCircle(30+x, 35, 7, SSD1306_COLOR_BLACK);
		SSD1306_DrawFilledCircle(50+x, 35, 7, SSD1306_COLOR_BLACK);
		SSD1306_UpdateScreen();
	}
}
void main_menu(void) {
	state = STATE_MAIN_MENU;
	
	SSD1306_GotoXY(116, 0);
	SSD1306_Puts("~", &Font_11x18, SSD1306_COLOR_WHITE);				//lock bitmap
	sprintf(str,"%u", step_size);
	SSD1306_GotoXY(0,52);
	SSD1306_Puts("Step:x", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_Puts(str, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(22, 20);
	SSD1306_Puts("Press OK for", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(15, 32);
	SSD1306_Puts("start counting", &Font_7x10, SSD1306_COLOR_WHITE);
	
	event = EVENT_NONE;
}

void count_menu(void) {
	state = STATE_COUNT_MENU;

	SSD1306_GotoXY(0,52);
	SSD1306_Puts("Step:x", &Font_7x10, SSD1306_COLOR_WHITE);
	sprintf(str,"%u", step_size);
	SSD1306_Puts(str, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(54, 16);
	sprintf(str, "%u", smd_counter);
	SSD1306_Puts(str, &Font_16x26, SSD1306_COLOR_WHITE);
	if (continue_counting_flag == 1) {
		sprintf(str, "%u", smd_counter_new);
		SSD1306_GotoXY(100, 45);
		SSD1306_Puts(str, &Font_11x18, SSD1306_COLOR_WHITE);
	}
	
	event = EVENT_NONE;
}

void save_menu(void) {
	state = STATE_SAVE_MENU;
	
	if (edit_counter_flag) {
		edit_menu();
	}
	else {
		pointer = constrain(pointer, 0, SAVE_MENU_MAX - 1);
		SSD1306_DrawRectangle(0, 0, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_COLOR_WHITE);					//border line
		SSD1306_DrawFilledRectangle(0, 1 + STEP_Y * pointer, SSD1306_WIDTH, 13, SSD1306_COLOR_WHITE);		//pointer line
		
		SSD1306_GotoXY(START_X, START_Y);
		SSD1306_Puts("Save ", &Font_7x10, (SSD1306_COLOR_t)(pointer != SAVE_TO_DATABASE));
		sprintf(str, "%u", smd_counter);
		SSD1306_Puts(str, &Font_7x10, (SSD1306_COLOR_t)(pointer != SAVE_TO_DATABASE));
		SSD1306_GotoXY(START_X, START_Y + STEP_Y);
		SSD1306_Puts("Edit and save", &Font_7x10, (SSD1306_COLOR_t)(pointer != EDIT_AND_SAVE));
		SSD1306_GotoXY(START_X, START_Y + 2 * STEP_Y);
		SSD1306_Puts("Continue counting", &Font_7x10, (SSD1306_COLOR_t)(pointer != CONTINUE));
		SSD1306_GotoXY(START_X, START_Y	+ 3 * STEP_Y);
		SSD1306_Puts("Exit", &Font_7x10, (SSD1306_COLOR_t)(pointer != CANCEL));

		if (event == EVENT_BTN_SHORT) {
			switch (pointer) {
				case SAVE_TO_DATABASE:
					save_to_data_base();
					state = STATE_MAIN_MENU;
					break;
				case EDIT_AND_SAVE:
					edit_counter_flag = 1;
					break;
				case CONTINUE:
					continue_counting_flag = 1;
					smd_counter_new = 0;
					state = STATE_COUNT_MENU;
					break;
				case CANCEL:
					smd_counter = 0;
					smd_counter_new = 0;
					continue_counting_flag = 0;
					state = STATE_MAIN_MENU;
					break;
			}
		}
	}
	event = EVENT_NONE;
}

void edit_menu(void) {
	SSD1306_GotoXY(30, 10);
	sprintf(str, "< %u >", smd_counter);
	SSD1306_Puts(str, &Font_16x26, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(10, 52);
	SSD1306_Puts("HOLD OK to save", &Font_7x10, SSD1306_COLOR_WHITE);
	if (event == EVENT_BTN_LONG) {
		save_to_data_base();
		state = STATE_MAIN_MENU;
		edit_counter_flag = 0;
	}
}

void setting_menu(void) {
	state = STATE_SETTING_MENU;
	
	pointer = constrain(pointer, 0, SETTING_MENU_MAX - 1);
	SSD1306_DrawRectangle(0, 0, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_COLOR_WHITE);					//border line
	SSD1306_DrawFilledRectangle(0, 1 + STEP_Y * pointer, SSD1306_WIDTH, 13, SSD1306_COLOR_WHITE);		//pointer line
	
	SSD1306_GotoXY(START_X, START_Y);
	SSD1306_Puts("Edit step  x", &Font_7x10, (SSD1306_COLOR_t)(pointer != STEP_COUNTER));
	sprintf(str,"%u",step_size);
	SSD1306_Puts(str, &Font_7x10, (SSD1306_COLOR_t)(pointer != STEP_COUNTER));
	if (edit_step_flag) {
		SSD1306_Puts(" <<",&Font_7x10, SSD1306_COLOR_BLACK);
	}
	SSD1306_GotoXY(START_X, START_Y + STEP_Y);
	SSD1306_Puts("Database", &Font_7x10, (SSD1306_COLOR_t)(pointer != DATABASE));
	SSD1306_GotoXY(START_X, START_Y + 2 * STEP_Y);
	SSD1306_Puts("Export to PC", &Font_7x10, (SSD1306_COLOR_t)(pointer != EXPORT_TO_PC));
	SSD1306_GotoXY(START_X, START_Y	+ 3 * STEP_Y);
	SSD1306_Puts("Exit", &Font_7x10, (SSD1306_COLOR_t)(pointer != EXIT));

	if (event == EVENT_BTN_SHORT) {
		switch (pointer) {
			case STEP_COUNTER:
				edit_step_flag = !edit_step_flag;
				break;
			case DATABASE:
				state = STATE_DATABASE_MENU;
				break;
			case EXPORT_TO_PC:
				break;
			case EXIT:
				state = STATE_MAIN_MENU;
				break;
		}
	}
	
	event = EVENT_NONE;
}

void database_menu(void) {
	state = STATE_DATABASE_MENU;
	
	if (db_element == 0) {
		SSD1306_GotoXY(18, 10);
		SSD1306_Puts("DATABASE", &Font_11x18, SSD1306_COLOR_WHITE);
		SSD1306_GotoXY(15, 30);
		SSD1306_Puts("IS EMPTY!", &Font_11x18, SSD1306_COLOR_WHITE);
	} 
	else {
		pointer = constrain(pointer, 0, db_element - 1);
		SSD1306_DrawRectangle(0, 0, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_COLOR_WHITE);					//border line
		SSD1306_DrawFilledRectangle(0, 1 + STEP_Y * pointer, SSD1306_WIDTH, 13, SSD1306_COLOR_WHITE);		//pointer line
			for(uint8_t i = 0; i < db_element; i++) { 
				SSD1306_GotoXY(START_X, START_Y + i * STEP_Y);
				sprintf(str, "%u. %u", i+1, *(database+i));
				SSD1306_Puts(str, &Font_7x10, (SSD1306_COLOR_t)(pointer != i));
			}
		if( event == EVENT_BTN_SHORT) {
			edit_db_flag = !edit_db_flag;
		}
		if (edit_db_flag) {
			SSD1306_GotoXY(100, START_Y + pointer * STEP_Y);
			SSD1306_Puts(" <<",&Font_7x10, SSD1306_COLOR_BLACK);
		}
	}	
	
	event = EVENT_NONE;
}

void save_to_data_base(void) {
	database[db_element] = smd_counter;
	db_element++;
	smd_counter = 0;
	smd_counter_new = 0;
	continue_counting_flag = 0;
	SSD1306_GotoXY(15, 20);
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_Puts("Saved!", &Font_16x26, SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen();
	delay(500);
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
