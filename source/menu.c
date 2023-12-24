#include "../header/menu.h"
#include "../header/utilites.h"
#include <stdlib.h>
#include <string.h>
#include "../header/book.h"
#include "../header/error_hand.h"

state_t *state_init() {

	state_t *current = malloc(sizeof(*current));
	memset(current, 0, sizeof(*current));	
	current->termsize = malloc(sizeof(*current->termsize));	
	memset(current->termsize, 0, sizeof(*current->termsize));
	current->saveposcnt = 0;
	current->savepos = malloc(sizeof(*current->savepos));
	state_update(current);

	return current;
}
void state_update(state_t *state) {
	
	uint32_t *tmp = NULL;
	state->termsize = get_maxYX();
	state->defpos[0] = state->termsize[0] / 4;
	state->defpos[1] = state->termsize[1] / 4;
	state->savepos[state->saveposcnt] = realloc(tmp, (sizeof(*state->savepos) * state->saveposcnt + 1));	
	state->savepos[state->saveposcnt] = state->currpos = get_YX();
	state->saveposcnt++;
}

void state_close(state_t *state) {

	if(state->saveposcnt && state->savepos[0]){
		for(int i = 0; i < state->saveposcnt; i++) {
			free(state->savepos[i]);
		}
	}
	free(state->savepos);
	if(state->termsize)
		free(state->termsize);
	if(state->currpos)
		free(state->currpos);
	if(state)
		free(state);
}

submenu_t *submenu_init(uint32_t submenu_type) {
	
	submenu_t *submenu = malloc(sizeof(*submenu));
	memset(submenu, 0, sizeof(*submenu));
	submenu->state = state_init(); 

	switch(submenu_type) {
		case 0:
			submenu->state->defpos[1] = (submenu->state->termsize[1] * 0.5);
			submenu->itemcnt = 5;
			if(!(submenu->items = malloc(sizeof(*submenu->items) * submenu->itemcnt)))
				print_error(ERR_MALLOC, "submenu_itmes", EXIT);
			submenu->items[0] = (uint8_t *)strdup("Header Editor");
			submenu->items[1] = (uint8_t *)strdup("Author: ");
			submenu->items[2] = (uint8_t *)strdup("Title: ");
			submenu->items[3] = (uint8_t *)strdup("Relase Date: ");
			submenu->items[4] = (uint8_t *)strdup("Confirm");
			if(!(submenu->input = malloc(sizeof(submenu->input) * submenu->itemcnt - 1)))
				print_error(ERR_MALLOC, "submenu_header_edit_input", EXIT);
			submenu->padding = 4;
			submenu->itemdist = 2;
		break;
	}
	return submenu;
}

void submenu_close(submenu_t *submenu) {

	if(submenu->state)
		state_close(submenu->state);
	if(submenu->items)
		free(submenu->items);
	if(submenu)
		free(submenu);
}

void submenu_input_print(submenu_t *submenu) {

	submenu->state->currpos = submenu->state->defpos;
	mprintf(submenu->state->currpos[0], submenu->state->currpos[1], NORM, (char *)submenu->items[0]);
	for(int i = 0; i < submenu->itemcnt - 1; i++) {
		mprintf(submenu->state->currpos[0] += submenu->itemdist, submenu->state->currpos[1] - strlen((char *)submenu->items[i]), NORM, (char *)submenu->items[i + 1]);
		state_update(submenu->state);
	}
}

void submenu_input_get(submenu_t *input) {
	
	uint32_t c = 0;
	show_cur();	
	submenu_input_print(input);
	while(1) {
		state_update(input->state);
		switch(getcha()) {
			case 'A': 
				if(c > 0) {
					c--;
					move_cursor(input->state->savepos[input->state->saveposcnt - input->itemcnt + c][0], input->state->savepos[input->state->saveposcnt - input->itemcnt + c][1]);	
				}
			break;
			case 'B':
				if(c < input->itemcnt - 2) {
					c++;
					move_cursor(input->state->savepos[input->state->saveposcnt - input->itemcnt + c][0], input->state->savepos[input->state->saveposcnt - input->itemcnt + c][1]);	
				}
			break;
			case 'C':
				read_string(&input->input[c], ECHO); 
			break;
		}
	}
	hide_cur();
}

menu_t *menu_init(uint32_t menu_type) {
	
	menu_t *menu = malloc(sizeof(*menu));
	memset(menu, 0, sizeof(*menu));
	menu->state = state_init();
	menu->submenus = malloc(sizeof(**menu->submenus));
	menu->book = malloc(sizeof(*menu->book));
	switch (menu_type) {
		case MAIN:
			menu->itemcnt = 4;
			if(!(menu->items = malloc(menu->itemcnt * sizeof(char *))))
				print_error(ERR_MALLOC, "menu_items", EXIT);
			menu->items[0] = (uint8_t *)strdup("---Main Menu---");
			menu->items[1] = (uint8_t *)strdup("Open Header Menu");
			menu->items[2] = (uint8_t *)strdup("Open FILE");
			menu->items[3] = (uint8_t *)strdup("Exit");
			menu->padding = 4;
			menu->itemdist = 2;
		break;
		case HEADER:
			menu->itemcnt = 5;
			if(!(menu->items = malloc(menu->itemcnt * sizeof(char *))))
				print_error(ERR_MALLOC, "menu_items", EXIT);
			menu->items[0] = (uint8_t *)strdup("---Header Menu---");
			menu->items[1] = (uint8_t *)strdup("Edit Header");
			menu->items[2] = (uint8_t *)strdup("Display Header");
			menu->items[3] = (uint8_t *)strdup("Write Header To File");
			menu->items[4] = (uint8_t *)strdup("Exit");
			menu->padding = 4;
			menu->itemdist = 2;
		break;
		case OPENFILE:
			menu->itemcnt = 4;
			if(!(menu->items = malloc(menu->itemcnt * sizeof(char *))))
				print_error(ERR_MALLOC, "open_file_menu", EXIT);
			menu->items[0] = (uint8_t *)strdup("---FILE MENU---");
			menu->items[1] = (uint8_t *)strdup("Enter file name");
			menu->items[2] = (uint8_t *)strdup("Browse Directory");
			menu->items[3] = (uint8_t *)strdup("EXIT");
			menu->padding = 4;
			menu->itemdist = 2;
		break;
	}
	return menu;
}

void menu_close(menu_t *menu) {

	state_close(menu->state);
	free(menu->items);	
	free(menu);
}

void menu_print(menu_t *menu) {

	uint8_t ch = 0, cond = 0;
	uint32_t *highlighted = NULL, i = 0;
	
	state_update(menu->state);
	clear();
	highlighted = malloc(sizeof(menu->state->termsize));
	memset(highlighted, 0, sizeof(*highlighted));
	while (1) {
		mprintf(menu->state->defpos[0], menu->state->defpos[1], LEFT, "%s", (char *)menu->items[0]);
		for(i = 0; i < (menu->itemcnt - 1); i++) {
			if(highlighted[0] == 0)
				highlighted[0] = (menu->state->defpos[0] + menu->padding);
			if(highlighted[0] == menu->state->defpos[0] + (menu->padding + (i * menu->itemdist))) {
				mprintfh((menu->state->defpos[0] + menu->padding) + (i * menu->itemdist), menu->state->defpos[1] - (strlen((char *)menu->items[0]) / 2), NORM, "%s", (char *)menu->items[i + 1]);
			} else {
				mprintf((menu->state->defpos[0] + menu->padding) + (i * menu->itemdist), menu->state->defpos[1] - (strlen((char *)menu->items[0]) / 2), NORM, "%s", (char *)menu->items[i + 1]);
			}
		}
		switch (getcha()) {
			case 65:
				if(highlighted[0] == (menu->state->defpos[0] + menu->padding)) {
					highlighted[0] = (menu->state->defpos[0] + menu->padding) + ((menu->itemcnt - 2) * menu->itemdist);
					ch = menu->itemcnt - 2; 
					break;
				} else {
					highlighted[0] -= menu->itemdist;
					ch--;
				}
			break;
			case 66: 
				if(highlighted[0] == (menu->state->defpos[0] + menu->padding) + ((menu->itemcnt - 2) * menu->itemdist)){
					highlighted[0] = (menu->state->defpos[0] + menu->padding);
					ch = 0;
					break;
				} else {
					highlighted[0] += menu->itemdist;
					ch++;
				}
			break;
			case 10:
				cond = 1;
				menu->itemchoice = ch;
			break;
			default : 
			break;
		}
	if (cond == 1) {
		break;
		}
	}
	free(highlighted);
}

