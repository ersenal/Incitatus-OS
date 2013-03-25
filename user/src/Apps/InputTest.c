#include <Lib/Incitatus.h>
#include <Lib/libc/stdio.h>

int main(void) {

    char c = 0;
    char buf[32];
    int i = 0;

    while((c = getch()) != '\n') {

        putc(c); /* Echo input */

        /* Handle backspace */
        if(c == '\b') {

            i--;
            buf[i] = '\0';

        } else {

            buf[i] = c;
            i++;

        }

    }

    /* Handle enter */
    putc('\n');
    buf[i] = '\0';

    printf("%s%s%c", "You typed: ", buf, '\n');
    exit(0);
}