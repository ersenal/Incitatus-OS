#include <Lib/Incitatus.h>


int main(void) {

    puts("Hello from User space!");
    puts("Spawning ls...");
    spawn("/ls");
    while(1);


}