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
Unsigned 64-bit integer.
*/
typedef unsigned long uint64;

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
String view.
*/
typedef struct Strview {
  const char *at;
  int length;
} Strview;

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
sb_view     | Make a string view
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

/*
String view from `sb`.
*/
Strview
sb_view(Strbuf *sb);

/*----------------------------------------------------------*/
/* FUNCTIONS: STRING VIEW                                   */
/*----------------------------------------------------------*/

/*
    GLOSSARY

sv_array          | View an array
sv_compare        | Compare two strings
sv_contains_char  | Check if the string contains the character
sv_contains_sv    | Check if the string contains another
sv_cstr           | View a zero terminated string
sv_cut            | Remove the first characters
sv_cut_end        | Remove the last characters
sv_equal          | Check if two strings are equal
sv_equal_no_case  | Check if two strings are equal
sv_filter         | Count from the start while filter
sv_filter_end     | Count from the end while filter
sv_filter_not     | Count from the start until filter
sv_filter_not_end | Count from the end until filter
sv_find_char      | Find the first such character
sv_find_char_end  | Find the last such character
sv_find_sv        | Find the first such string
sv_find_sv_end    | Find the last such string
sv_get            | Take the first characters
sv_get_end        | Take the last characters
sv_prefix         | Check if the string starts with prefix
sv_span           | Count from the start while in the sample
sv_span_end       | Count from the end while in the sample
sv_span_not       | Count from the start until in the sample
sv_span_not_end   | Count from the end until in the sample
sv_substr         | Get the sub-string from the string
sv_suffix         | Check if the string ends with suffix

*/

/*
String view from an array of characters.
*/
Strview
sv_array(const char *chars, int length);

/*
Compare `string` to `another`.
*/
int
sv_compare(Strview string, Strview another);

/*
Check if `string` contains `ch`.
*/
int
sv_contains_char(Strview string, char ch);

/*
Check if `string` contains `substr`.
*/
int
sv_contains_sv(Strview string, Strview substr);

/*
String view from a zero terminated string.
*/
Strview
sv_cstr(const char *cstr);

/*
Remove the first `count` characters from `string`.
*/
Strview
sv_cut(Strview string, int count);

/*
Remove the last `count` characters from `string`.
*/
Strview
sv_cut_end(Strview string, int count);

/*
Check if `string` equal `another`.
*/
int
sv_equal(Strview string, Strview another);

/*
Check if `string` equal `another`. Ignore the differnce
between upper case and lower case.
*/
int
sv_equal_no_case(Strview string, Strview another);

/*
Count the first characters in `string` that pass `filter`.
*/
int
sv_filter(Strview string, int(*filter)(int));

/*
Count the last characters in `string` that pass `filter`.
*/
int
sv_filter_end(Strview string, int(*filter)(int));

/*
Count the first characters in `string` that did not
pass `filter`.
*/
int
sv_filter_not(Strview string, int(*filter)(int));

/*
Count the last characters in `string` that did not
pass `filter`.
*/
int
sv_filter_not_end(Strview string, int(*filter)(int));

/*
Find the first apearance of `ch` in `string`.
*/
int
sv_find_char(Strview string, char ch);

/*
Find the last apearance of `ch` in `string`.
*/
int
sv_find_char_end(Strview string, char ch);

/*
Find the first apearance of `substr` in `string`.
*/
int
sv_find_sv(Strview string, Strview substr);

/*
Find the last apearance of `substr` in `string`.
*/
int
sv_find_sv_end(Strview string, Strview substr);

/*
Get the first `count` characters from `string`.
*/
Strview
sv_get(Strview string, int count);

/*
Get the last `count` characters from `string`.
*/
Strview
sv_get_end(Strview string, int count);

/*
Check if `string` starts with `prefix`.
*/
int
sv_prefix(Strview string, Strview prefix);

/*
Count the first characters from `string` that are in `sample`.
*/
int
sv_span(Strview string, Strview sample);

/*
Count the last characters from `string` that are in `sample`.
*/
int
sv_span_end(Strview string, Strview sample);

/*
Count the first characters from `string` that are not
in `sample`.
*/
int
sv_span_not(Strview string, Strview sample);

/*
Count the last characters from `string` that are not
in `sample`.
*/
int
sv_span_not_end(Strview string, Strview sample);

/*
Get `length` characters from `start` in `string`.
*/
Strview
sv_substr(Strview string, int start, int length);

/*
Check if `string` ends with `suffix`.
*/
int
sv_suffix(Strview string, Strview suffix);

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

