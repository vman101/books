#include "../header/menu.h"
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
	current->defpos[1] = current->termsize[1] / 2;
	return current;
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
		break;
	}
	return menu;
}

void menu_close(menu_t *menu) {

	free(menu->state->termsize);
	free(menu->state);
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
			if(highlighted[0] == menu->state->defpos[0] + (menu->padding + (i * 2))) {
				mprintfh((menu->state->defpos[0] + menu->padding) + (i * 2), menu->state->defpos[1] - (strlen((char *)menu->items[0]) / 2), NORM, "%s", (char *)menu->items[i + 1]);
			} else {
				mprintf((menu->state->defpos[0] + menu->padding) + (i * 2), menu->state->defpos[1] - (strlen((char *)menu->items[0]) / 2), NORM, "%s", (char *)menu->items[i + 1]);
			}
		}
		switch (getcha()) {
			case 65:
				if(highlighted[0] == (menu->state->defpos[0] + menu->padding)) {
					break;
				} else {
					highlighted[0] -= 2;
					ch--;
				}
			break;
			case 66: 
				if(highlighted[0] == (menu->state->defpos[0] + menu->padding) + ((menu->itemcnt - 2) * 2)){
					break;
				} else {
					highlighted[0] += 2;
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

void header_menu(FILE *fp, header_t *header, term_t *term) {
	
	uint8_t exitCond = 0, ch = 0;
	menu_t *header_menu = menu_init(HEADER);

	while(1) {

		menu_print(header_menu);
		clear();	
		switch (header_menu->itemchoice) {
			case 0: 
				ch = 0;

				mprintf(header_menu->state->defpos[0], header_menu->state->defpos[1], CEN, "Do you want to edit the Header?\n(y/n)");
				switch (ch = getch()) {
					case 'y': 
						clear();
						memset(header, 0, sizeof(*header));	
						header->magic_num = 0x69133742;
						header->def_col_len = 0x1e;
						header->def_row_len = 0x50;
						printf("\t--EDIT MODE--\n\n");
						printf("\nEnter Author: ");
						header->author_len = read_string(&header->author, CECHO);
						printf("\nEnter Book Title: ");
						header->title_len = read_string(&header->title, CECHO);
						printf("\nEnter Release Date: ");
						header->release_date_len = read_string(&header->release_date,  CECHO);
						printf("\nSuccesfully Edited!\n\nPress Enter to return to Menu...");
						exitCond = 1;
					break;
					case 'n' : 
					break;
					default:
						exitCond = 0;
						mprintf(header_menu->state->termsize[0], 0, NORM, "--Invalid Input--");
					break;
				}
		break;
			case 1:

				ch = 0;
				printf( "Author: %s"
						"Title: %s"
						"Release Date: %s"
						"Magic Number: %#010x",
						 header->author,
						 header->title,
						 header->release_date,
						 header->magic_num);
				printf("Is the Information Correct?\n(y/n)");
				switch (ch = getch()) {
					case 'y': 
						ch = 0;
						printf("\nDo you want to write the header to the file ?\n(y/n)");
						switch(ch = getch()) {
							case 'y':
								exitCond = 1;
								header_file_write(fp, header);
							break;
							case 'n': 
							break;
							default : 
								printf("\n--Invalid Input--\n");
							break;
						}
				
			break;
			case 2:
				mprintf(header_menu->state->defpos[0], header_menu->state->defpos[1], CEN, "Do you want to write the header to the file ? (y/n)");
					switch(ch = getch()) {
						case 'y':
							exitCond = 1;
							header_file_write(fp, header);
						break;
						case 'n': 
						break;
						default : 
							printf("\n--Invalid Input--\n");
							getch();
						break;
						}
					break;
					case 'n': 
					break;
					default :
						printf("Invaild Input!\nPress enter to return to display mode...");
						getch();
					break;
				}
			break;
			case 3:
				header_menu->state = update_state();
				mprintf(header_menu->state->defpos[0], 0, NORM, "Exiting Header Menu");
				if(getch() == '\033')
					break;
				exitCond = 1;
			break;
			default:
				printf("\n\nInvalid Input");
				getch();
			break;
		}
		if (exitCond == 1)
			break;
	}
	menu_close(header_menu);
}
