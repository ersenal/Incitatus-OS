#include <Lib/Incitatus.h>
#include <Lib/libc/string.h>

static void parseInput(char* in);
static void cat(const char* path);

int main(void) {

    cls();
    cat("logo.ascii");
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
            parseInput(entry);

        }

    }

    while(1);

}

static void parseInput(char* in) {

    char* command = strtok(in, " ");
    char* param = strtok(NULL, " ");

    if(command == NULL)
        return;

    if(strcmp(command, "exec") == 0) { /* Execute binary */

        if(param == NULL) {
            puts("No parameter given\n");
            return;
        }

        if(!spawn(param))
            puts("Couldn't find that binary\n");

    } else if(strcmp(command, "cat") == 0) { /* Display file contents */

        if(param == NULL) {
            puts("No parameter given\n");
            return;
        }

        cat(param);

    } else {

        puts("Unknown command\n");

    }



}

static void cat(const char* path) {

    FILE* file = open(path, "r");
    if(file == NULL) {
        puts("Couldn't find that file\n");
        return;
    }

    struct stat fileStat;
    fstat(file, &fileStat);
    char buf[fileStat.fileSize + 1];
    read(file, 0, fileStat.fileSize, buf);
    puts(buf);
    close(file);

}