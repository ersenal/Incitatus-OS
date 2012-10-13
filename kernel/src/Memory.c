#include <Memory.h>
#include <Debug.h>

PUBLIC void Memory_set(void* ptrToMem, char valueToSet, u32int length) {

    Debug_assert(ptrToMem != NULL);

    char* ptr = (char*) ptrToMem;

    for(u32int i = 0; i < length; i++) {

        *ptr = valueToSet;
        ptr++;

    }

}

PUBLIC void Memory_copy(void* destination,const void* source, u32int sourceLength) {

    Debug_assert(destination != NULL);
    Debug_assert(source != NULL);

    char* dest = (char*) destination;
    char* src  = (char*) source;

    for(u32int i = 0; i < sourceLength; i++) {

        *(dest + i) = *(src + i);

    }

}