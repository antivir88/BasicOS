#include "string.h"

void *memmove(void *dest, const void *src, size_t n)
{
    char *d = dest;
    const char *s = src;

    if (d < s) {
        while (n--) {
            *d++ = *s++;
        }
    } else {
        char *lasts = s + (len-1);
        char *lastd = d + (len-1);

        while (n--) {
            *lastd-- = *lasts--;
        }
    }

    return dest;
}
