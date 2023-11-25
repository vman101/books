//
// Created by UrMom on 04.11.2023.
//

#ifndef BOOK_H

#define BOOK_H
#include "escape.h"
#include "unix_term.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <termios.h>
#include <unistd.h>
#include <wchar.h>
#include <dirent.h>

#define OFFSET_READ 20 
#define OFFSET_RELDAT 24
#define OFFSET_LEN_RELDAT 8
#define OFFSET_LEN_TITLE 12
#define OFFSET_LEN_AUTHOR 16

#define MAXROW  9999
#define MAXCOL  9999

enum mprintf {
    CEN,
    LEFT,
    RIGHT,
    NORM,
};
enum TERM_bools {
    NOCECHO,
    CECHO,
};

typedef struct {
    uint32_t *termsize;
    uint32_t defpos[2];
    uint32_t info;
}state;
typedef struct {
    uint32_t magic_num;
    uint8_t def_row_len; // from 10 to 100
    uint8_t def_col_len; // from 10 to 40
    uint32_t release_date_len;
    uint32_t author_len;
    uint32_t title_len;
    uint8_t *release_date;
    uint8_t *author;
    uint8_t *title;
    uint64_t filesize;
}header_t;

typedef struct {
    size_t chapter_len;
    char *text;
}chapter_t;

typedef struct {
    header_t *header;
    size_t chapter_num;
    chapter_t *chapters;
}book_t;

typedef struct {
    struct termios *t_echo;
    struct termios *t_noecho;
} term_t;

//term functions
void        reset_term();
void        save_t_attributes();
void        reset_input_mode ();
void        set_input_mode (term_t *term, uint32_t flag);
void        clear();
void        hide_cur();
void        show_cur();
state       *update_state();
//tools
uint32_t    mprintf(uint32_t y, uint32_t x, uint8_t pos, char *str, ...);
uint32_t    mprintfh(uint32_t y, uint32_t x, uint8_t pos, char *str, ...);
uint32_t    read_string(uint8_t **string, uint32_t *len, uint8_t echo);
void        print_main_menu();

//header manip functions
void        header_menu(FILE *fp, header_t *header, term_t *term);
state       *header_menu_print();
void        header_file_write(FILE *fp, header_t *header);
bool        init(book_t **book, term_t **term);
bool        quit(book_t **book, term_t **term);

/* Read .BOOK from CMD_LINE_ARGV */
bool        book_file_read(FILE **fp, book_t **book, char *argv, bool FOFLAG);
#endif // BOOK_H
