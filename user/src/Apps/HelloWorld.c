#include <Lib/Incitatus.h>

int main(void) {

    puts("Hello from User space!");
    char buf[10];
    char c;
    int i = 0;

    while(1) {

        c = getch();

        if(c != -1) {

            putc(c);
            buf[i] = c;
            i++;
        }

        if(c == '\n') {

            buf[i - 1] = '\0';
            i = 0;
            spawn(buf);

        }

    }

    while(1);

}