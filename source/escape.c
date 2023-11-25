#include "../header/escape.h"
#include "../header/book.h"
#include "../header/error_hand.h"
#include <stdio.h>
#include <string.h>

uint32_t *get_maxYX() {
	
	uint32_t *oldYX, *maxYX;
	uint32_t len = 0;
	
	oldYX = get_YX();
	move_cursor(MAXROW, MAXCOL);
	maxYX = get_YX();	
	move_cursor(oldYX[0], oldYX[1]);
	free(oldYX);

	return maxYX;
}


uint32_t *get_YX() {

	uint32_t *YX = malloc(sizeof(*YX) * 2);
	memset(YX, 0, sizeof(*YX));
	uint8_t *buf;
	uint32_t len;
	printf("\033[6n");
	read_string(&buf, &len, NOCECHO); 
	sscanf((char *)buf, "\033[%d;%dR", &YX[0], &YX[1]); 
	return YX;
}
/* uint32_t len = 0;
uint8_t *real = NULL;
uint32_t *YX = malloc(sizeof(typeof(uint32_t)) * 2);
uint8_t *buf = malloc(10);
uint8_t *buf2 = malloc(sizeof(buf));

printf("\033[6n");
read_string(&buf, &len, NOCECHO); 

switch (len) {
	
	case 6:
		for(int i = 0, j = 1; i < 2; i++, j++) {
			buf2[i] = buf[j];
			buf2 = realloc(real, sizeof(*buf2) + 1);
		}
		YX[0] = atoi((char *)buf2);
		memset(buf2, 0, sizeof(*buf2));
		for(int i = 0, j = 5, n = 2; i < n; i++, j++) {
			buf2[i] = buf[j];
			buf2 = realloc(real, sizeof(*buf2) + 1);
		}
		YX[1] = atoi((char *)buf2);
		memset(buf, 0, sizeof(*buf));
	break;
	case 7:
	
	break;
	case 8:
		for(int i = 0, j = 2; i < 2; i++, j++) {
			buf2[i] = buf[j];
			buf2 = realloc(real, sizeof(*buf2) + 1);
		}
		YX[0] = atoi((char *)buf2);
		memset(buf2, 0, sizeof(*buf2));
		for(int i = 0, j = 5, n = 2; i < n; i++, j++) {
			buf2[i] = buf[j];
			buf2 = realloc(real, sizeof(*buf2) + 1);
		}
		YX[1] = atoi((char *)buf2);
		memset(buf, 0, sizeof(*buf));
	break;
	case 9:
		if(buf[4] == ';') {
			for(int i = 0, j = 2; i < 2; i++, j++) {
				buf2[i] = buf[j];
				buf2 = realloc(real, sizeof(*buf2) + 1);
			}
			YX[0] = atoi((char *)buf2);
			memset(buf2, 0, sizeof(*buf2));
			for(int i = 0, j = 5, n = 2; i < n; i++, j++) {
				buf2[i] = buf[j];
				buf2 = realloc(real, sizeof(*buf2) + 1);
			}
			YX[1] = atoi((char *)buf2);
			memset(buf, 0, sizeof(*buf));
			break;
		}
		if(buf[5] == ';') {
			for(int i = 0, j = 2; i < 3; i++, j++) {
				buf2[i] = buf[j];
				buf2 = realloc(real, sizeof(*buf2) + 1);
			}
			YX[0] = atoi((char *)buf2);
			memset(buf2, 0, sizeof(*buf2));
			for(int i = 0, j = 6, n = 2; i < n; i++, j++) {
				buf2[i] = buf[j];
				buf2 = realloc(real, sizeof(*buf2) + 1);
			}
			YX[1] = atoi((char *)buf2);
			memset(buf, 0, sizeof(*buf));
			break;
		}
	break;
	case 10:
		for(int i = 0, j = 2; i < 3; i++, j++) {
			buf2[i] = buf[j];
			buf2 = realloc(real, sizeof(*buf2) + 1);
		}
		YX[0] = atoi((char *)buf2);
		memset(buf2, 0, sizeof(*buf2));
		for(int i = 0, j = 5, n = 3; i < n; i++, j++) {
			buf2[i] = buf[j];
			buf2 = realloc(real, sizeof(*buf2) + 1);
		}
		YX[1] = atoi((char *)buf2);
		memset(buf, 0, sizeof(*buf));
	break;
	default:
		print_error(ERR_GET, "YX", EXIT);
	break;
}
free(buf2);
free(buf);

return YX;
} */



//syntax = (Y, X)
void move_cursor(uint32_t rows, uint32_t cols) {
	printf("\033[%d;%dH", rows, cols);	

}
