#include <stdint.h>
#include <termios.h>
#include "../header/unix_term.h"

int32_t getch_(uint8_t echo) {

    int32_t ch;

    tcgetattr(0, &old);    
    current = old;
    current.c_lflag &= ~ICANON;
    if(echo) {
        current.c_lflag |= ECHO;
    } else {
        current.c_lflag &= ~ECHO;
    }
    tcsetattr(0, TCSANOW, &current);
    ch = getchar();
    tcsetattr(0, TCSANOW, &old);
    return ch;
}

int32_t getch() {

    return getch_(0);
}

int32_t getche() {

    return getch_(1);
}

int32_t getcha() {

    int32_t ch;

    if((ch = getch()) != '\033')
        return ch;
    if((ch = getch()) == 13)
        return ch;

    return getch();
}
