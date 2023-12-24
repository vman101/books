//
// Created by UrMom on 04.11.2023.
//

#ifndef BOOK_H

#define BOOK_H
#include "term.h"
#include "escape.h"
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
    FILE *bp;
}book_t;

//header manip functions
bool        main_menu(book_t **book);
void        header_menu(header_t *header);
void        header_menu_edit(header_t *header);
void        file_menu(book_t **book);
void        header_file_write(FILE *fp, header_t *header);
bool        book_init(book_t **book, term_t **term, uint32_t argc, uint8_t **argv);
bool        book_quit(book_t **book, term_t **term);

/* Read .BOOK from CMD_LINE_ARGV */
bool        book_file_read(book_t **book, uint8_t *path);
#endif
