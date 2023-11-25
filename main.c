#include "header/book.h"
#include "header/error_hand.h"
#include "header/escape.h"
#include <bits/types/FILE.h>
#include <iso646.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <wchar.h>


int main(int argc, char **argv)
{
	book_t *book;
	FILE *fp;
	term_t *term;
	state *state;
	bool file_open = false;
	
	clear();
	atexit(reset_term);
	init(&book, &term);

	if(!file_open && argc >= 2) 
		file_open = book_file_read(&fp, &book, argv[1], file_open);

	while(1) {
	
		clear();
		print_main_menu();

		switch(getch()) {
			
			case '+': 
				header_menu(fp, book->header, term);
			break;
		}
	}
	fclose(fp);
	quit(&book, &term);
	return EXIT_SUCCESS;
}

state *update_state() {

	state *current = malloc(sizeof(*current)); 
	
	current->termsize = get_maxYX();
	current->defpos[0] = current->termsize[0] / 4;
	current->defpos[1] = current->termsize[1] / 2;

	return current;
}
