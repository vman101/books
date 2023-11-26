#include "../header/term.h"
#include <stdio.h>


struct termios t_save;

void save_t_attributes() {

	tcgetattr (STDIN_FILENO, &t_save);
	atexit (reset_input_mode);
}

void reset_input_mode () {

	reset_term();
	tcsetattr (STDIN_FILENO, TCSANOW, &t_save);
}

void set_input_mode (term_t *term, uint32_t flag) {

	/* Make sure stdin is a terminal. */
	if (!isatty (STDIN_FILENO)) {
		  fprintf (stderr, "Not a terminal.\n");
		  exit (EXIT_FAILURE);
		}
	switch(flag) {
		case 0:
			tcgetattr (STDIN_FILENO, term->t_noecho);
			term->t_noecho->c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
			term->t_noecho->c_cc[VMIN] = 1;
			term->t_noecho->c_cc[VTIME] = 0;
			tcsetattr (STDIN_FILENO, TCSAFLUSH, term->t_noecho);
		break;
		case 1:
			tcgetattr (STDIN_FILENO, term->t_echo);
			term->t_echo->c_lflag |= (ICANON|ECHO|ECHOE); 
			term->t_echo->c_cc[VMIN] = 1;
			term->t_echo->c_cc[VTIME] = 0;
			tcsetattr (STDIN_FILENO, TCSAFLUSH, term->t_echo);
		break;
		default: 
			printf("Failed to Set Term Flags");
		break;
	}
}
