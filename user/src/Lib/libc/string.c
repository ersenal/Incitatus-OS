#include <Lib/libc/string.h>
#include <Lib/libc/limits.h>
#include <Lib/libc/stdint.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wpointer-sign"

void *memset(void *dest, int c, size_t n)
{
    unsigned char *s = dest;
    c = (unsigned char)c;
    for (; ((uintptr_t)s & (sizeof(size_t)-1)) && n; n--) *s++ = c;
    if (n) {
        size_t *w, k = ((size_t)-1/UCHAR_MAX) * c;
        for (w = (void *)s; n>=(sizeof(size_t)); n-=(sizeof(size_t)), w++) *w = k;
        for (s = (void *)w; n; n--, s++) *s = c;
    }
    return dest;
}

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    if (((uintptr_t)d & (sizeof(size_t)-1)) != ((uintptr_t)s & (sizeof(size_t)-1)))
        goto misaligned;

    for (; ((uintptr_t)d & (sizeof(size_t)-1)) && n; n--) *d++ = *s++;
    if (n) {
        size_t *wd = (void *)d;
        const size_t *ws = (const void *)s;

        for (; n>=(sizeof(size_t)); n-=(sizeof(size_t))) *wd++ = *ws++;
        d = (void *)wd;
        s = (const void *)ws;
misaligned:
        for (; n; n--) *d++ = *s++;
    }
    return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
    char *d = dest;
    const char *s = src;

    if (d==s) return d;
    if (s+n <= d || d+n <= s) return memcpy(d, s, n);

    if (d<s) {
        if ((uintptr_t)s % (sizeof(size_t)) == (uintptr_t)d % (sizeof(size_t))) {
            while ((uintptr_t)d % (sizeof(size_t))) {
                if (!n--) return dest;
                *d++ = *s++;
            }
            for (; n>=(sizeof(size_t)); n-=(sizeof(size_t)), d+=(sizeof(size_t)), s+=(sizeof(size_t))) *(size_t *)d = *(size_t *)s;
        }
        for (; n; n--) *d++ = *s++;
    } else {
        if ((uintptr_t)s % (sizeof(size_t)) == (uintptr_t)d % (sizeof(size_t))) {
            while ((uintptr_t)(d+n) % (sizeof(size_t))) {
                if (!n--) return dest;
                d[n] = s[n];
            }
            while (n>=(sizeof(size_t))) n-=(sizeof(size_t)), *(size_t *)(d+n) = *(size_t *)(s+n);
        }
        while (n) n--, d[n] = s[n];
    }

    return dest;
}

size_t strlen(const char *s)
{
    const char *a = s;
    const size_t *w;
    for (; (uintptr_t)s % (sizeof(size_t)); s++) if (!*s) return s-a;
    for (w = (const void *)s; !((*w)-((size_t)-1/UCHAR_MAX) & ~(*w) & (((size_t)-1/UCHAR_MAX) * (UCHAR_MAX/2+1))); w++);
    for (s = (const void *)w; *s; s++);
    return s-a;
}

int strcmp(const char *l, const char *r)
{
    for (; *l==*r && *l && *r; l++, r++);
    return *(unsigned char *)l - *(unsigned char *)r;
}

char *strcpy(char *restrict dest, const char *restrict src)
{
    const unsigned char *s = src;
    unsigned char *d = dest;
    while ((*d++ = *s++));
    return dest;
}

char *strcat(char *restrict dest, const char *restrict src)
{
    strcpy(dest + strlen(dest), src);
    return dest;
}

#pragma GCC diagnostic pop