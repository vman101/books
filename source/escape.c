#include "../header/escape.h"
#include "../header/utilites.h"
#include "../header/book.h"
#include <stdio.h>
#include <stdlib.h>
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
    uint8_t *buf = NULL;

    memset(YX, 0, sizeof(*YX));
    printf("\033[6n");
    read_stringesc(&buf, NOCECHO); 
    sscanf((char *)buf, "\033[%d;%dR", &YX[0], &YX[1]); 
    return YX;
}

int32_t getch_(uint8_t echo) {

    int32_t ch;

    tcgetattr(0, &old);    
    current = old;
    current.c_lflag &= ~ICANON;
    if(echo) {
        current.c_lflag |= ECHO;
    } else {
        current.c_lflag &= ~ECHO;
    }
    tcsetattr(0, TCSANOW, &current);
    ch = getchar();
    tcsetattr(0, TCSANOW, &old);
    return ch;
}

int32_t getch() {

    return getch_(0);
}

int32_t getche() {

    return getch_(1);
}

int32_t getcha() {

    int32_t ch;

    if((ch = getch()) != '\033')
        return ch;
    ch = 0;
    ch = getch();
    if(ch == 13 || ch == 8)
        return ch;
    ch = 0;
    if((ch = getch()) == 3 && (ch = getch()) == '~')
        return 127;
    return ch;
}

//syntax = (Y, X)
void move_cursor(uint32_t rows, uint32_t cols) {

	printf("\033[%d;%dH", rows, cols);	
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
