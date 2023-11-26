#include "header/book.h"

int main(int argc, char **argv)
{

	book_t *book;
	term_t *term;
	
	clear();
	book_init(&book, &term, argc, (uint8_t **)argv);

	while(!main_menu(&book)) {}

	book_quit(&book, &term);

	return EXIT_SUCCESS;
}
