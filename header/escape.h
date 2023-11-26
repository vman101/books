#ifndef ESCAPE_H
#define ESCAPE_H
#include <stdio.h>
#include <stdint.h>
#include <termios.h>

static struct termios current, old;

int32_t     getch_(uint8_t echo);
/* getchar no echo */
int32_t     getch();
/* getchar echo */
int32_t     getche();
/*getchar no echo for arrowkeys */
int32_t     getcha();

int32_t     getchae();

void        move_cursor(uint32_t rows, uint32_t cols);

uint32_t    *get_maxYX();

uint32_t    *get_YX();

void        clear();

void        hide_cur();

void        show_cur();
#endif
