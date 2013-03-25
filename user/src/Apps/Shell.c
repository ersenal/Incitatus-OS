#include <Lib/Incitatus.h>
#include <Lib/libc/string.h>
#include <Lib/libc/stdio.h>

static char workingDirectory[64];

static void suicide(void);
static void parseInput(char* in);
static void cat(const char* path);
static void ls(void);
static void help(void);

//TODO: comments
int main(void) {

    char entry[64];
    char c = 0;
    int i = 0;

    cls();
    color(WHITE);
    cat("logo.ascii");
    color(LGREY);
    getcwd(workingDirectory);
    printf("%s%c", workingDirectory, '>');

    while(1) {

        while((c = getch()) != '\n') {

            putc(c); /* Echo input */

            /* Handle backspace */
            if(c == '\b') {

                i--;
                entry[i] = '\0';

            } else {

                entry[i] = c;
                i++;

            }

        }

        /* Handle enter */
        putc('\n');
        entry[i] = '\0';
        i = 0;
        parseInput(entry);
        printf("%s%c", workingDirectory, '>');

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

        int pid = spawn(param);

        if(!pid)
            puts("Couldn't find that binary\n");
        else
            waitpid(pid);

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
        else
            getcwd(workingDirectory);

    } else if(strcmp(command, "ls") == 0) { /* list directory */

        ls();

    } else if(strcmp(command, "cls") == 0) { /* clear console */

        cls();

    } else if(strcmp(command, "restart") == 0) { /* restart machine */

        restart();

    } else if(strcmp(command, "shutdown") == 0) { /* shut down machine */

        poweroff();
        suicide(); /* panic if not successful */

    } else if(strcmp(command, "help") == 0) { /* list valid commands */

        help();

    } else if(strcmp(command, "suicide") == 0) { /* kills the shell */

        suicide();

    } else {

        puts("Unknown command\n");

    }

}

static void suicide(void) {

    /* raise a page fault */
    char* f = 0;
    *f = 0;

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
    printf("%s%c", buf, '\n');
    close(file);

}

static void ls(void) {

    FILE* cwd = fgetcwd();
    FILE* file = 0;
    int i = 0;
    struct stat buf;

    while((file = readdir(cwd, i))) {

        fstat(file, &buf);
        printf("%s%s%d%s", buf.fileName, ", " ,buf.fileSize, "bytes\n");
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
        "suicide - kills the shell\n"
        "shutdown - shuts down the machine\n"
        );

}