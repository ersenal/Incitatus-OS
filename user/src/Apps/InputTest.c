#include <Lib/Incitatus.h>
#include <Lib/libc/stdio.h>

int main(void) {

    char c = 0;
    char buf[32];
    int i = 0;

    while((c = getch()) != '\n') {

        buf[i] = c;
        i++;

    }

    buf[i] = '\0';

    printf("%s%s%c", "You typed: ", buf, '\n');
    exit(0);
}