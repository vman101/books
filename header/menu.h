#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdio.h>

enum menu_types {
    MAIN,
    HEADER,
    OPENFILE,
};

enum submenu_types {
    HEADER_EDIT

};
typedef struct {
    uint32_t *termsize;
    uint32_t defpos[2];
    uint32_t *currpos;
    uint32_t info;
}state_t;

typedef struct submenu_t {
    state_t *state;
    uint8_t **items; //pos[0] is alway TITLE
    uint32_t itemcnt;
    uint32_t itemchoice;
    uint32_t itemdist;
    uint32_t padding;

} submenu_t;

typedef struct menu_t {
    struct menu_t **menus;
    struct submenu_t **submenus;
    state_t *state;
    uint8_t **items; //pos[0] is alway TITLE
    uint32_t itemcnt;
    uint32_t itemchoice;
    uint32_t itemdist;
    uint32_t padding;
}menu_t;

state_t     *update_state();
void        state_close(state_t *state);
submenu_t   *submenu_init(uint32_t submenu_type);
void        submenu_close(submenu_t *submenu);
menu_t      *menu_init(uint32_t menu_type);
void        menu_close(menu_t *menu);
void        menu_print(menu_t *menu);

#endif
