#include "../header/menu.h"
#include "../header/book.h"
#include "../header/escape.h"
#include "../header/utilites.h"
#include "../header/error_hand.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main() {

	uint8_t **buffer;

	clear();
	mprintf(1, 50, NORM, "hello: ");
	mprintf(2, 50, NORM, "hey: ");
	while(1) {	
		switch(getcha()) {
			case 'A':
				move_cursor(1, 50 + strlen("hello: "));
			break;
			case 'B':
				move_cursor(2, 50 + strlen("hey: "));
			break;
			case 'C':
			default:
			break;
		}
	}
	show_cur();
}
