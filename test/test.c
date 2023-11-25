#include <stdio.h>
#include <wchar.h>
#include <unistd.h>

int main() {

    for(int i = 0; i < 10; i++) {
        printf("%c", 0xac);
        printf("\033[5;%dH", i);
    }
}
