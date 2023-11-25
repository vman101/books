#include "../header/book.h"
#include "../header/error_hand.h"
#include "../header/escape.h"
#include "../header/unix_term.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

uint32_t read_string(uint8_t **string, uint32_t *len, uint8_t echo) {
	
	uint8_t *tmp = NULL, *tmp2 = NULL;
	uint32_t i = 0, ch = EOF;
	
	while(ch) {
		
		if(echo)
			ch = getche(); 
		else
			ch = getch();

		if(ch == EOF || ch == '\n')
			ch = 0;

		tmp = (uint8_t *)realloc(tmp2, i +1);

		if(!tmp) {
			free(tmp2);
			print_error(ERR_MALLOC, "tmp", EXIT);
			return 1;
		}
		tmp2 = tmp;
		tmp2[i++] = ch;
		if(tmp2[0] == '\x1b' && ch == 'R')
			ch = 0;
	}
	*string = tmp2;
	*len = i;
	return 0;
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

void header_menu(FILE *fp, header_t *header, term_t *term) {
	
	uint8_t exitCond = 0, ch = 0, menuChoice = 0;
	state *header_s = update_state();

	while(1) {

		clear();	
		header_s = header_menu_print();
		clear();
		switch (header_s->info) {
			case 1: 
				mprintf(header_s->defpos[0], header_s->defpos[1], CEN, "Do you want to write the header to the file ? (y/n)");
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
			case 2:

				ch = 0;

				mprintf(header_s->defpos[0], header_s->defpos[1], CEN, "Do you want to edit the Header?\n(y/n)");
				switch (ch = getch()) {
					case 'y': 
						clear();
						memset(header, 0, sizeof(*header));	
						header->magic_num = 0x69133742;
						header->def_col_len = 0x1e;
						header->def_row_len = 0x50;
						printf("\t--EDIT MODE--\n\n");
						printf("\nEnter Author: ");
						read_string(&header->author, &header->author_len, CECHO);
						printf("\nEnter Book Title: ");
						read_string(&header->title, &header->title_len, CECHO);
						printf("\nEnter Release Date: ");
						read_string(&header->release_date, &header->release_date_len, CECHO);
						printf("\nSuccesfully Edited!\n\nPress Enter to return to Menu...");
						exitCond = 1;
					break;
					case 'n' : 
					break;
					default:
						exitCond = 0;
						mprintf(header_s->termsize[0], 0, NORM, "--Invalid Input--");
					break;
				}
			break;
			case 3:
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
					case 'n': 
					break;
					default :
						printf("Invaild Input!\nPress enter to return to display mode...");
						getch();
					break;
				}
			break;
			default:
				printf("\n\nInvalid Input");
			break;
		}
		if (exitCond == 1)
			break;
	}
	free(header_s);
}

state *header_menu_print() {

	uint8_t ch = 0;
	uint8_t cond = 0;
	uint32_t opts = 4;
	uint32_t *maxYX = NULL, *YX = NULL, *saveYX = NULL, *highlighted = NULL, len = 0;
	state *state = update_state();

	maxYX = get_maxYX();
	YX = malloc(sizeof(*maxYX) * opts);
	saveYX = malloc(sizeof(maxYX));
	highlighted = malloc(sizeof(maxYX));
	memset(highlighted, 0, sizeof(*highlighted));
	saveYX[0] = maxYX[0] / 4;
	saveYX[1] = maxYX[1] / 2;
	YX[0] = saveYX[0] + 4;
	YX[1] = saveYX[0] + 6;
	YX[2] = saveYX[0] + 8;
	YX[3] = saveYX[0] + 10;

	while (1) {


		len = mprintf(saveYX[0], saveYX[1], CEN, "---Add a Book to your Collection---");
		if(highlighted[0] == 0)
			highlighted[0] = saveYX[0] + 4;
		if(highlighted[0] == (saveYX[0] + 4)) {
			mprintfh(YX[0], saveYX[1] - (len / 2), NORM, "Edit header Information");
		} else {
			mprintf(YX[0], saveYX[1] - (len / 2), NORM, "Edit header Information");
		}
		if(highlighted[0] == (saveYX[0] + 6)) {
			mprintfh(YX[1], saveYX[1] - (len / 2), NORM, "Edit header Information");
		} else {
			mprintf(YX[1], saveYX[1] - (len / 2), NORM, "Edit header Information");
		}
		if(highlighted[0] == (saveYX[0] + 8)) {
			mprintfh(YX[2], saveYX[1] - (len / 2), NORM, "Edit header Information");
		} else {
			mprintf(YX[2], saveYX[1] - (len / 2), NORM, "Edit header Information");
		}
		if(highlighted[0] == (saveYX[0] + 10)) {
			mprintfh(YX[3], saveYX[1] - (len / 2), NORM,"Edit header Information");
		} else {
			mprintf(YX[3], saveYX[1] - (len / 2), NORM,"Edit header Information");
		}
		switch (getcha()) {
			case 65:
				if(highlighted[0] == saveYX[0] + 4) {
					break;
				} else {
					highlighted[0] -= 2;
				}
			break;
			case 66: 
				if(highlighted[0] == saveYX[0] + 10) {
					break;
				} else {
					highlighted[0] += 2;
				}
			break;
			case 67:
			break;
			case 68:
			break;
			case 10:
				cond = 1;
				if(highlighted[0] == YX[0])
					state->info = 1;
				if(highlighted[0] == YX[1])
					state->info = 2;
				if(highlighted[0] == YX[2])
					state->info = 3;
				if(highlighted[0] == YX[3])
					state->info = 4;
			break;
			default : 
				cond = 0;

				mprintf(maxYX[0] -2 , 1, NORM, "Invalid Input!\nPress Enter to restart\n");
				getch();		
				move_cursor(saveYX[0], saveYX[1]);
			break;
		}
		if (cond == 1) {
			break;
		}
	}
	free(saveYX);
	free(maxYX);
	free(highlighted);
	return state;
}

bool book_file_read(FILE **fp, book_t **book, char *path, bool file_open) {

	if((strstr(path, ".book")) == NULL) {
		print_error(ERR_FNAME, path, EXIT);
	}
	if((*fp = fopen(path, "r")) == NULL) {
		*fp = fopen(path, "w+");
		return true;
	}
	else {
		fclose(*fp);
		if((*fp = fopen(path, "r+")) == NULL)
			print_error(ERR_FOPEN, path, EXIT);
	}
	(*book)->header->release_date = malloc(sizeof((*book)->header->release_date_len));
	(*book)->header->author = malloc(sizeof((*book)->header->author_len)); 
	(*book)->header->title = malloc(sizeof((*book)->header->title_len));	
	if(!(fread((*book)->header, OFFSET_READ, 1, *fp)))
		print_error(ERR_FWRITE, "book->header", EXIT);
	if(!(fread((*book)->header->release_date, (*book)->header->release_date_len, 1, *fp)))
		print_error(ERR_FWRITE, "book->header->rel", EXIT);
	if(!(fread((*book)->header->author, (*book)->header->author_len, 1, *fp)))
		print_error(ERR_FWRITE, "book->header->author", EXIT);
	if(!(fread((*book)->header->title, (*book)->header->title_len, 1, *fp)))
		print_error(ERR_FWRITE, "book->header->title", EXIT);
	clear();
	printf("File Opened Succesfully! Press any key to continue...");
	getch();
	file_open = true;
	
	return file_open;
}

void clear() {

	printf("\033[2J\033[H");
}

bool quit(book_t **book, term_t **term) {

	free((*book)->header->release_date);
	free((*book)->header->author);
	free((*book)->header->title);
	free((*book)->header);
	free((*book)->chapters);
	free(*book);
	free((*term)->t_echo);
	free((*term)->t_noecho);
	free(*term);

	show_cur();
	printf("\033[1049l");

	return EXIT_SUCCESS;
}

bool init(book_t **book, term_t **term) {
	
	//hide_cur();
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

	return EXIT_SUCCESS;
}

void print_main_menu() {

	uint32_t *maxYX = get_maxYX();
	uint32_t midY = maxYX[0] / 4;
	uint32_t midX = maxYX[1] / 2;

	move_cursor(midY, midX);
	mprintf(midY, midX, NORM, "---Main Menu---");
}

uint32_t mprintf(uint32_t y, uint32_t x, uint8_t pos, char *str, ...) {

	uint32_t len = strlen((char *)str);
	switch (pos) {
		case 0:
			x -= len / 2;
		break;
		case 1:
			x -= len;
		break;
		case 2: 
			x += len;
		default: 
		break;
	}
	move_cursor(y, x);
	printf("%s", str);

	return len;
}

uint32_t mprintfh(uint32_t y, uint32_t x, uint8_t pos, char *str, ...) {

	uint32_t len = strlen((char *)str);
	switch (pos) {
		case 0:
			x -= len / 2;
		break;
		case 1:
			x -= len;
		break;
		case 2: 
			x += len;
		default: 
		break;
	}
	move_cursor(y, x);
	printf("\033[38;2;0;0;0m");
	printf("\033[48;2;255;255;255m");
	printf("%s", str);
	printf("\033[0m");

	return len;
}

void hide_cur() {

	printf("\033[?25l");
}

void show_cur() {

	printf("\033[?25h");
}
void reset_term(){

	printf("\033[c");
}
struct termios t_save;

void save_t_attributes() {

	tcgetattr (STDIN_FILENO, &t_save);
	atexit (reset_input_mode);
}

void reset_input_mode () {

	reset_term();
	tcsetattr (STDIN_FILENO, TCSANOW, &t_save);
}

void set_input_mode (term_t *term, uint32_t flag) {

	/* Make sure stdin is a terminal. */
	if (!isatty (STDIN_FILENO)) {
		  fprintf (stderr, "Not a terminal.\n");
		  exit (EXIT_FAILURE);
		}
	switch(flag) {
		case 0:
			tcgetattr (STDIN_FILENO, term->t_noecho);
			term->t_noecho->c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
			term->t_noecho->c_cc[VMIN] = 1;
			term->t_noecho->c_cc[VTIME] = 0;
			tcsetattr (STDIN_FILENO, TCSAFLUSH, term->t_noecho);
		break;
		case 1:
			tcgetattr (STDIN_FILENO, term->t_echo);
			term->t_echo->c_lflag |= (ICANON|ECHO|ECHOE); 
			term->t_echo->c_cc[VMIN] = 1;
			term->t_echo->c_cc[VTIME] = 0;
			tcsetattr (STDIN_FILENO, TCSAFLUSH, term->t_echo);
		break;
		default: 
			printf("Failed to Set Term Flags");
		break;
	}
}
