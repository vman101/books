#include "../header/utilites.h"
#include "../header/book.h"
#include "../header/term.h"
#include "../header/error_hand.h"
#include <stdio.h>
#include <stdlib.h>

bool book_init(book_t **book, term_t **term, uint32_t argc, uint8_t **argv) {
	
	hide_cur();
	printf("\033[1049h");
	printf("\033[5=h");
	save_t_attributes();
	*book = malloc(sizeof(**book));
	memset(*book, 0, sizeof(**book));
	(*book)->chapters = malloc(sizeof(*(*book)->chapters));
	memset((*book)->chapters, 0, sizeof(*(*book)->chapters));
	(*book)->header = malloc(sizeof(*(*book)->header));
	memset((*book)->header, 0, sizeof(*(*book)->header));
	*term = malloc(sizeof(**term));
	memset(*term, 0, sizeof(**term));
	(*term)->t_echo = malloc(sizeof(*(*term)->t_echo));
	memset((*term)->t_echo, 0, sizeof(*(*term)->t_echo));
	(*term)->t_noecho = malloc(sizeof(*(*term)->t_noecho));
	memset((*term)->t_noecho, 0, sizeof(*(*term)->t_noecho));
	atexit (reset_input_mode);
	atexit(reset_term);
	atexit(clear);

	if(argc >= 2) 
		book_file_read(book, argv[1]);
	return EXIT_SUCCESS;
}

bool book_quit(book_t **book, term_t **term) {

	if((*book)->header->release_date)
		free((*book)->header->release_date);
	if((*book)->header->author)
		free((*book)->header->author);
	if((*book)->header->title)
		free((*book)->header->title);
	if((*book)->header)
		free((*book)->header);
	if((*book)->chapters)
		free((*book)->chapters);
	if(*book)
		free(*book);
	if((*term)->t_echo)
		free((*term)->t_echo);
	if((*term)->t_noecho)
		free((*term)->t_noecho);
	if(*term)
		free(*term);
	if((*book)->bp)
		fclose((*book)->bp);
	show_cur();
	printf("\033[1049l");
	clear();

	return EXIT_SUCCESS;
}

bool book_file_read(book_t **book, uint8_t *path) {

	state_t *tmpst;
	if((strstr((char *)path, ".book")) == NULL) {
		print_error(ERR_FNAME, (char *)path, EXIT);
	}
	if(((*book)->bp = fopen((char *)path, "r")) == NULL) {
		(*book)->bp = fopen((char *)path, "w+");
		return true;
	}
	else {
		fclose((*book)->bp);
		if(((*book)->bp = fopen((char *)path, "r+")) == NULL)
			print_error(ERR_FOPEN, (char *)path, EXIT);
	}
	(*book)->header->release_date = malloc(sizeof((*book)->header->release_date_len));
	(*book)->header->author = malloc(sizeof((*book)->header->author_len)); 
	(*book)->header->title = malloc(sizeof((*book)->header->title_len));	
	if(!(fread((*book)->header, OFFSET_READ, 1, (*book)->bp)))
		print_error(ERR_FWRITE, "book->header", EXIT);
	if(!(fread((*book)->header->release_date, (*book)->header->release_date_len, 1, (*book)->bp)))
		print_error(ERR_FWRITE, "book->header->rel", EXIT);
	if(!(fread((*book)->header->author, (*book)->header->author_len, 1, (*book)->bp)))
		print_error(ERR_FWRITE, "book->header->author", EXIT);
	if(!(fread((*book)->header->title, (*book)->header->title_len, 1, (*book)->bp)))
		print_error(ERR_FWRITE, "book->header->title", EXIT);
	clear();
	printf("File Opened Succesfully! Press any key to continue...");
	getch();
	
	return true;
}

void header_file_write(FILE *fp, header_t *header) {
		
	uint8_t err = 0;

	fseek(fp, 0, SEEK_SET);
	clear();
	if((fwrite(header, OFFSET_READ, 1, fp)) == 0) {
		err = 1;
		print_error(ERR_FWRITE, "header", EXIT);
	}
	if((fwrite(header->release_date, 1, header->release_date_len, fp)) == 0) {
		err = 1;
		print_error(ERR_FWRITE, "release_date", EXIT);
	}
	if((fwrite(header->author, 1, header->author_len, fp)) == 0) {
		err = 1;
		print_error(ERR_FWRITE, "author", EXIT);
	}
	if((fwrite(header->title, 1, header->title_len, fp)) == 0) {
		err = 1;
		print_error(ERR_FWRITE, "title", EXIT);
	}
	if(err == 0) {
		printf("\nSuccesfully Written to FILE!\n\n Press any Key to Return to Menu...");
		getch();
	}
}

bool main_menu(book_t **book) {

	menu_t *main_menu;
	
	clear();
	if(!(main_menu = menu_init(MAIN)))
		print_error(ERR_MALLOC, "main_menu", EXIT);	
	menu_print(main_menu);
	switch(main_menu->itemchoice) {
		
		case 0: 
			header_menu((*book)->bp, (*book)->header);
		break;
		case 1:
			file_menu(book);
		break;
		case 2:
			menu_close(main_menu);
			return true;
		break;
	}
	return false;
}

void header_menu(FILE *fp, header_t *header) {
	
	uint8_t exitCond = 0, ch = 0;
	menu_t *header_menu = menu_init(HEADER);

	while(1) {

		menu_print(header_menu);
		clear();	
		switch (header_menu->itemchoice) {
			case 0: 
				ch = 0;
				//edit menu
				mprintf(header_menu->state->defpos[0], header_menu->state->defpos[1], CEN, "Do you want to edit the Header?\n(y/n)");
				switch (ch = getch()) {
					case 'y': 
						clear();
						memset(header, 0, sizeof(*header));	
						header->magic_num = 0x69133742;
						header->def_col_len = 0x1e;
						header->def_row_len = 0x50;
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
						break;
						}
					break;
					case 'n': 
					break;
					default :
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
			break;
		}
		if (exitCond == 1)
			break;
	}
	menu_close(header_menu);
}

void file_menu(book_t **book) {

	menu_t *file_menu = menu_init(OPENFILE);		
	menu_print(file_menu);
			
	uint8_t *path;
	mvread_string(&path, CECHO, file_menu->state->currpos);
	book_file_read(book, path);
}
