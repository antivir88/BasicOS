#ifndef STRING_H
#define STRING_H

/**
 * Для работы с различными видами строк.
 */

// DEFINITIONS
#define NULL ((void*)0)

#ifdef __x86_64__
    typedef uint64 size_t;
#else
    typedef uint32 size_t;
#endif

/* не от сюда
typedef unsigned char uint8;
typedef signed char int8;

typedef unsigned short uint16;
typedef signed short int16;

typedef unsigned long uint32;
typedef signed long int32;

typedef unsigned long long uint64;
typedef signed long long int64;

typedef enum { false = 0, true = 1 } bool;
*/

// METHODS

void *memcpy(void *dest, const void *src, size_t n); // memory copy
void *memmove(void *dest, const void *src, size_t n); // memory move
void *memchr(const char *s, char c, size_t n); // memory char
void *memcmp(const char *s1, const char *s2, size_t  n); // memory compare
void *memset(char *s, int z, size_t n); // memory set


char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);

char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);

int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);

int strcoll(const char *, const char *);

char *strcpy(char *toHere, const char *fromHere);
char *strncpy(char *toHere, const char *fromHere, size_t n);

char *strerror(int);
size_t strlen(const char *);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
char *strpbrk(const char *s, const char *accept);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *, const char *);
size_t strxfrm(char *dest, const char *src, size_t n);

#endif
