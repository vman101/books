#include "../header/utilites.h"
#include "../header/error_hand.h"
#include "../header/escape.h"
#include "../header/menu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdarg.h>

uint32_t read_stringesc(uint8_t **string, uint8_t echo) {
	
	uint8_t *tmp = NULL, *tmp2 = NULL;
	uint32_t ch = EOF;
	uint32_t i = 0;
	show_cur();	
	
	while(ch) {
		
		ch = getch();
		
		if(ch == EOF || ch == '\n')
			ch = 0;
		if(ch == '\b') {
			printf("bla");	
			uint32_t *YX = get_YX();
			mprintf(YX[0], YX[1] - 1, NORM, "%c ½c", ch, ch);
			i--;
			free(YX);
			continue;
		}
		tmp = (uint8_t *)realloc(tmp2, i +1);

		if(!tmp) {
			free(tmp2);
			print_error(ERR_MALLOC, "tmp", EXIT);
			return 1;
		}
		if(echo)
			printf("%c", ch);
		tmp2 = tmp;
		tmp2[i++] = ch;
		if(tmp2[0] == '\x1b' && ch == 'R')
			ch = 0;
	}
	*string = tmp2;
	hide_cur();

	return i;
}

uint32_t read_string(uint8_t **string, uint8_t echo) {
	
	uint8_t *tmp = NULL, *tmp2 = NULL;
	uint32_t ch = EOF;
	uint32_t i = 0;
	show_cur();	
	
	while(ch) {
		
		ch = getcha();
		
		if(ch == EOF || ch == '\n')
			ch = 0;
		if(ch == 127 && i != 0) {
			printf("\b \b");
			i--;
			continue;
		}
		tmp = (uint8_t *)realloc(tmp2, i +1);

		if(!tmp) {
			free(tmp2);
			print_error(ERR_MALLOC, "tmp", EXIT);
			return 1;
		}
		if(echo)
			printf("%c", ch);
		tmp2 = tmp;
		tmp2[i++] = ch;
		if(tmp2[0] == '\x1b' && ch == 'R')
			ch = 0;
	}
	*string = tmp2;
	hide_cur();

	return i;
}

uint32_t mvread_string(uint8_t **string, uint8_t echo, uint32_t *YX) {
	
	uint8_t *tmp = NULL, *tmp2 = NULL;
	uint32_t ch = EOF;
	uint32_t i = 0;
	state_t *tmpst = update_state();

	show_cur();	
	
	while(ch) {
		
		ch = getcha();
		
		if(ch == EOF || ch == '\n')
			ch = 0;
		if(ch == 127 && i != 0) {
			printf("\b \b");
			i--;
			continue;
		}
		tmp = (uint8_t *)realloc(tmp2, i +1);

		if(!tmp) {
			free(tmp2);
			print_error(ERR_MALLOC, "tmp", EXIT);
			return 1;
		}
		if(echo)
			printf("%c", ch);

		tmpst->currpos[1] += i;
		tmp2 = tmp;
		tmp2[i++] = ch;
		if(tmp2[0] == '\x1b' && ch == 'R')
			ch = 0;
	}
	*string = tmp2;
	hide_cur();
	

	return i;
}

void clear() {

	printf("\033[2J\033[H");
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


