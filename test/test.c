#include "../header/menu.h"
#include "../header/book.h"
#include "../header/escape.h"
#include "../header/unix_term.h"
#include "../header/error_hand.h"
#include <stdio.h>
#include <wchar.h>
#include <unistd.h>

int main() {

	while(1) {
		int ch = getchae();
		if(ch == 127) {

			printf("\b  \b \b\b");
			continue;
		}
		printf("%c", ch);
	}	
}
