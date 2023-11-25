#ifndef ESCAPE_H
#define ESCAPE_H
#include <stdio.h>
#include <stdint.h>

void move_cursor(uint32_t rows, uint32_t cols);
uint32_t *get_maxYX();
uint32_t *get_YX();

#endif
