/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Debug.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides macro functions for printing debug messages.
|
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef DEBUG_H
#define DEBUG_H

#include <Drivers/Console.h>
#include <Sys.h>

/*-------------------------------------------------------------------------
| Debug Logging
|--------------------------------------------------------------------------
|
| PARAM:        M       printf template(refer to Console.h)
|               ...     printf tokens
|
| EXAMPLE:      Debug_logInfo("%s%d", "Debug info test", 0x1);
\------------------------------------------------------------------------*/
#define Debug_logError(M, ...)  Console_setColor(CONSOLE_ERROR); \
                                Console_printf("%s%s%c%d%s"M, "[ERROR] ", __FILE__, ':', __LINE__, " -> ", ##__VA_ARGS__); \
                                Console_setColor(CONSOLE_NORMAL); \
                                Console_printChar('\n')

#define Debug_logWarning(M, ...)    Console_setColor(CONSOLE_WARN); \
                                    Console_printf("%s%s%c%d%s"M, "[WARN] ", __FILE__, ':', __LINE__, " -> ", ##__VA_ARGS__); \
                                    Console_setColor(CONSOLE_NORMAL); \
                                    Console_printChar('\n')

#define Debug_logInfo(M, ...) Console_setColor(CONSOLE_INFO); \
                              Console_printf("%s%s%c%d%s"M, "[INFO] ", __FILE__, ':', __LINE__, " -> ", ##__VA_ARGS__); \
                              Console_setColor(CONSOLE_NORMAL); \
                              Console_printChar('\n')

/*-------------------------------------------------------------------------
| Assertion
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Kernel panic if the condition "C" is NOT true
|
| PARAM:        C       condition
|
| EXAMPLE:      Debug_assert(index <= lastIndex)
\------------------------------------------------------------------------*/
#ifdef NO_DEBUG
    #define Debug_assert(C) ((void) 0)
#else
    #define Debug_assert(C) if(!(C)) {Debug_logError(""); Sys_panic("Assert failure!");}
#endif

#endif
