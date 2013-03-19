#include <Lib/Incitatus.h>


//TODO: exec param, cat param etc.
int main(void) {

    cls();

    char entry[64];
    char c;
    int i = 0;

    while(1) {

        c = getch();

        if(c != -1) {
            putc(c);

            /* Handle backspace */
            if(c == '\b') {

                i--;
                entry[i] = '\0';

            } else {

                entry[i] = c;
                i++;

            }

        }

        if(c == '\n') {

            entry[i - 1] = '\0';
            i = 0;
            if(!spawn(entry))
                puts("Couldn't find that binary\n");

        }

    }

    while(1);

}