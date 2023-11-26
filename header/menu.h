#ifndef MENU_H
#define MENU_H

#include "book.h"
#include <stdint.h>

enum menu_types {
    MAIN,
    HEADER,
};

typedef struct {
    uint32_t *termsize;
    uint32_t defpos[2];
    uint32_t info;
}state_t;

typedef struct menu_t{
    struct menu_t **menus;
    uint32_t padding;
    state_t *state;
    uint8_t **items;
    uint32_t itemcnt;
    uint32_t itemchoice;
}menu_t;

menu_t      *menu_init(uint32_t menu_type);
void        menu_close(menu_t *menu);
state_t     *update_state();
void        menu_print(menu_t *menu);
void        header_menu(FILE *fp, header_t *header, term_t *term);
#endif
