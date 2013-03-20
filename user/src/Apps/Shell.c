#include <Lib/Incitatus.h>
#include <Lib/libc/string.h>

static void parseInput(char* in);
static void cat(const char* path);
static void ls(void);
static void help(void);

int main(void) {

    char entry[64];
    char cwd[64];
    char c = 0;
    int i = 0;

    cls();
    cat("logo.ascii");
    puts(getcwd(cwd));
    putc('>');

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
            puts(getcwd(cwd));
            putc('>');

        }

    }

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

    } else if(strcmp(command, "cd") == 0) { /* change directory */

        if(param == NULL) {
            puts("No parameter given\n");
            return;
        }

        if(chdir(param) == NULL)
            puts("No such directory\n");

    } else if(strcmp(command, "ls") == 0) { /* list directory */

        ls();

    } else if(strcmp(command, "cls") == 0) { /* clear console */

        cls();

    } else if(strcmp(command, "restart") == 0) { /* restart machine */

        restart();

    } else if(strcmp(command, "help") == 0) { /* list valid commands */

        help();

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

static void ls(void) {

    FILE* cwd = fgetcwd();
    FILE* file = 0;
    int i = 0;
    struct stat buf;

    while((file = readdir(cwd, i))) {

        fstat(file, &buf);
        puts(buf.fileName);
        putc('\n');
        i++;

    }

}

static void help(void) {

    puts(

        "ls - list files inside current working directory\n"
        "cd [dir] - change working directory\n"
        "cls - clear console\n"
        "cat [file] - display file contents\n"
        "restart - restart machine\n"
        "exec [file] - execute binary file\n"

        );


}