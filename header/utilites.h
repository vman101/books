#ifndef UTILITIES_H
#define UTILITIES_H

#include "book.h"
#include "escape.h"
#include <stdint.h>
#include "../header/term.h"
#include "../header/menu.h"

enum mprintf {
    CEN,
    LEFT,
    RIGHT,
    NORM,
};

uint32_t    mprintf(uint32_t y, uint32_t x, uint8_t pos, char *str, ...);
uint32_t    mprintfh(uint32_t y, uint32_t x, uint8_t pos, char *str, ...);
uint32_t    read_string(uint8_t **string, uint8_t echo);
uint32_t    read_stringesc(uint8_t **string, uint8_t echo);
uint32_t    mvread_string(uint8_t **string, uint8_t echo, uint32_t *YX);

#endif
