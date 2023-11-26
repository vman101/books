#ifndef TERM_H
#define TERM_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

enum TERM_bools {
    NOCECHO,
    CECHO,
};


typedef struct {
    struct termios *t_echo;
    struct termios *t_noecho;
} term_t;

void        reset_term();
void        save_t_attributes();
void        reset_input_mode ();
void        set_input_mode (term_t *term, uint32_t flag);

#endif
