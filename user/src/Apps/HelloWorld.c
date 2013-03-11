#include <Lib/Incitatus.h>


int main(void) {

    FILE cwd = fgetcwd();
    FILE file = 0;
    int i = 0;
    struct stat buf;

    while((file = readdir(cwd, i))) {

        fstat(file, &buf);
        puts(buf.fileName);
        putc('\n');
        i++;

    }

    while(1);

}