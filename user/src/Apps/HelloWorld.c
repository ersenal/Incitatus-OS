#include <Lib/Incitatus.h>

int main(void) {

    puts("Hello from User space!");
    spawn("/ls");

    while(1);

}