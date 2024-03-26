/* Unique ANSI C Compiler */
/* uacc.h */

#ifndef UACC_H
#define UACC_H

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------*/
/* INCLUDES                                                 */
/*----------------------------------------------------------*/

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*----------------------------------------------------------*/
/* TYPES                                                    */
/*----------------------------------------------------------*/

/*
String buffer.
*/
typedef struct Strbuf {
  char *at;
  int length;
  int capacity;
  int is_inited;
} Strbuf;

/*
Global variables.
*/
typedef struct Globals {
  /* In case you want to get rid of some output. */
  FILE *fnull;
} Globals;

/*----------------------------------------------------------*/
/* FUNCTIONS: MEMORY                                        */
/*----------------------------------------------------------*/

/*
    GLOSSARY
mem_alloc         | Allocate memory
mem_alloc_zeros   | Allocate memory and clear all bytes
mem_clear         | Set all bytes to zero
mem_free          | Free previously allocated memory
mem_realloc       | Reallocate memory
mem_realloc_zeros | Reallocate memory and clear new bytes
*/

/*
Allocate `size` bytes of memory.
*/
void *
mem_alloc(int size);

/*
Allocate `size` bytes of memory and fill it with zeros.
*/
void *
mem_alloc_zeros(int size);

/*
Set `size` bytes by `ptr` to zero.
*/
void
mem_clear(void *ptr, int size);

/*
Free previously allocated memory.
*/
void
mem_free(void *ptr);

/*
Change the size of previously allocated memory.
`ptr` - the pointer to the memory.
`size` - new size.
*/
void *
mem_realloc(void *ptr, int size);

/*
Change the size of previously allocated memory.
`ptr` - the pointer to the memory.
`size` - new size.
`old_size` - the size of the memory.
All new bytes set to 0.
*/
void *
mem_realloc_zeros(void *ptr, int size, int old_size);

/*----------------------------------------------------------*/
/* FUNCTIONS: STRING BUFFER                                 */
/*----------------------------------------------------------*/

/*
    GLOSSARY
sb_append   | Append a string
sb_at       | Reference the character by index
sb_clear    | Remove all characters
sb_copy     | Copy a string
sb_deinit   | Free the memory used by the string buffer
sb_init     | Prepare a string buffer for work
sb_insert   | Insert a string
sb_remove   | Remove characters
sb_replace  | Replace characters with a string
sb_reserve  | Reserve memory for characters
*/

/*
Append a formated string.
*/
void
sb_append(Strbuf *sb, const char *fmt, ...);

/*
Get the pointer to the character in `sb` at `i`.
Negative values of `i` are used as a reverse index.
If `sb->length` is 0 this function returns the pointer
to the null character.
*/
char *
sb_at(Strbuf *sb, int i);

/*
Remove all characters from `sb`.
*/
void
sb_clear(Strbuf *sb);

/*
Replace the content of `sb` with a formated string.
*/
void
sb_copy(Strbuf *sb, const char *fmt, ...);

/*
Deinit `sb`. You cannot use `sb` unless you init it again.
*/
void
sb_deinit(Strbuf *sb);

/*
Init `sb` to an empty string.
You should init `sb` before using it in other functions.
*/
void
sb_init(Strbuf *sb);

/*
Insert a formated string into `sb` at `i`.
Negative values of `i` are used as a reverse index.
If `i` equals `sb->length` then this function appends
the fromated string.
*/
void
sb_insert(Strbuf *sb, int i, const char *fmt, ...);

/*
Remove `n` characters in `sb` at `i`.
Negative values of `i` are used as a reverse index.
If `i` equals `sb->length` then this function does nothing.
*/
void
sb_remove(Strbuf *sb, int i, int n);

/*
Replace `n` characters in `sb` at `i` with a formated string.
Negative values of `i` are used as a reverse index.
If `i` equals `sb->length` then this function appends
the fromated string.
*/
void
sb_replace(Strbuf *sb, int i, int n, const char *fmt, ...);

/*
Prepare `sb` to store at least `cap` characters.
*/
void
sb_reserve(Strbuf *sb, int cap);

/*----------------------------------------------------------*/
/* VARIABLES                                                */
/*----------------------------------------------------------*/

/*
Vector to global variables.
*/
extern Globals *G;

#ifdef __cplusplus
}
#endif

#endif /* UACC_H */

