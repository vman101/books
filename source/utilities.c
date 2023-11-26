#include "../header/book.h"
#include "../header/error_hand.h"
#include "../header/escape.h"
#include "../header/unix_term.h"
#include <SDL2/SDL.h>
#include <fenv.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

uint32_t read_string(uint8_t **string, uint8_t echo) {
	
	uint8_t *tmp = NULL, *tmp2 = NULL;
	uint32_t i = 0, ch = EOF;
	show_cur();	
	while(ch) {
		
		if(echo)
			ch = getchae(); 
		else
			ch = getch();
		if(ch == EOF || ch == '\n')
			ch = 0;
		if(ch == '\b') {
			tmp[i] = '\0';
			i--;
			continue;
		}
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
	hide_cur();

	return i;
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

bool book_file_read(FILE **fp, book_t **book, char *path) {

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
	
	return true;
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

bool book_init(book_t **book, term_t **term) {
	
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

	return EXIT_SUCCESS;
}

uint32_t mprintf(uint32_t y, uint32_t x, uint8_t pos, char *str, ...) {

	char buffer[2048];

	va_list args;
	va_start(args, str);
	vsprintf(buffer, str, args);

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
	printf("%s", buffer);

	return len;
}

uint32_t mprintfh(uint32_t y, uint32_t x, uint8_t pos, char *str, ...) {

	va_list args;
	char buffer[2048];

	va_start(args, str);
	vsprintf(buffer, str, args);
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
		break;
		default: 
		break;
	}
	move_cursor(y, x);
	printf("\033[38;2;0;0;0m");
	printf("\033[48;2;255;255;255m");
	printf("%s", buffer);
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
