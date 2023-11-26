#include "header/book.h"
#include "header/error_hand.h"
#include "header/escape.h"
#include "header/menu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>

void open_file_menu(FILE **fp, book_t **book) {

	uint8_t *path;
	read_string(&path, CECHO);
	book_file_read(fp, book, (char *)path);

}
int main(int argc, char **argv)
{

	book_t *book;
	FILE *fp;
	term_t *term;
	menu_t *main_menu;
	bool file_open = false;
	
	if(!(main_menu = menu_init(MAIN)))
		print_error(ERR_MALLOC, "main_menu", EXIT);	
	main_menu->itemcnt = 4;


	clear();
	atexit(reset_term);
	book_init(&book, &term);

	if(!file_open && argc >= 2) 
		file_open = book_file_read(&fp, &book, argv[1]);

	while(1) {
	
		clear();
		menu_print(main_menu);

		switch(main_menu->itemchoice) {
			
			case 0: 
				header_menu(fp, book->header, term);
			break;
			case 1:
				open_file_menu(&fp, &book);
			break;
		}
		break;
	}
	fclose(fp);
	menu_close(main_menu);
	quit(&book, &term);

	return EXIT_SUCCESS;
}
