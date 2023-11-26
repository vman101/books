#include "../header/menu.h"
#include "../header/utilites.h"
#include <stdlib.h>
#include <string.h>
#include "../header/book.h"
#include "../header/error_hand.h"

state_t *update_state() {

	state_t *current = malloc(sizeof(*current)); 
	memset(current, 0, sizeof(*current));	
	current->termsize = malloc(sizeof(*current->termsize));	
	memset(current->termsize, 0, sizeof(*current->termsize));
	current->termsize = get_maxYX();
	current->defpos[0] = current->termsize[0] / 4;
	current->defpos[1] = current->termsize[1] / 4;
	current->currpos = get_YX();
	return current;
}

void state_close(state_t *state) {

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
	submenu->state = update_state();

	switch(submenu_type) {
		case 0:
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
menu_t *menu_init(uint32_t menu_type) {
	
	menu_t *menu = malloc(sizeof(*menu));
	memset(menu, 0, sizeof(*menu));
	menu->state = update_state();

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
			if(!(menu->items = malloc(menu->itemcnt * sizeof(char*))))
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

void open_file_menu(FILE **fp, book_t **book) {

	menu_t *file_menu = menu_init(OPENFILE);		
	menu_print(file_menu);
			
	uint8_t *path;
	mvread_string(&path, CECHO, file_menu->state->currpos);
	book_file_read(book, path);
}


