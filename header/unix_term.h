#ifndef UNIX_TERM_H
#define UNIX_TERM_H
#include <stdint.h>
#include <stdio.h>
#include <termios.h>

static struct termios current, old;

int32_t getch_(uint8_t echo);

/* getchar no echo */
int32_t getch();
/* getchar echo */
int32_t getche();
/*getchar no echo for arrowkeys */
int32_t getcha();

int32_t getchae();
#endif
