/* Unique ANSI C Compiler */
/* uacc_lib.c - Library */

/*----------------------------------------------------------*/
/* INCLUDES                                                 */
/*----------------------------------------------------------*/

#include "uacc.h"

/*----------------------------------------------------------*/
/* STATIC FUNCTIONS                                         */
/*----------------------------------------------------------*/

/*
Returns `i` if it's in range `[0, n - 1]`.
If it's not then wrapped around.
*/
static int
normalize_index(int i, int n);

/*----------------------------------------------------------*/
/* STATIC FUNCTIONS: STRING BUFFER                          */
/*----------------------------------------------------------*/

/*
Replace `n` characters in `sb` at `i` with a formated string.
Negative values of `i` are used as a reverse index.
If `i` equals `sb->length` then this function appends
the fromated string.
`args1` and `args2` are two equal va_lists.
*/
static void
sb_vreplace(Strbuf *sb, int i, int n,
            const char *fmt, va_list args1, va_list args2);

/*----------------------------------------------------------*/
/* IMPLEMENTATION: MEMORY                                   */
/*----------------------------------------------------------*/

/*----------------------------------------------------------*/
void *
mem_alloc(int size)
{
  void *ptr = NULL;
  /**/
  assert(size > 0);
  /**/
  ptr = malloc(size);
  if (ptr == NULL) {
    fprintf(stderr, "%s",
      "uacc: error: sorry, not enough memory\n"
    );
    exit(EXIT_FAILURE);
  }
  return ptr;
}

/*----------------------------------------------------------*/
void *
mem_alloc_zeros(int size)
{
  void *ptr = NULL;
  /**/
  assert(size > 0);
  /**/
  ptr = mem_alloc(size);
  mem_clear(ptr, size);
  return ptr;
}

/*----------------------------------------------------------*/
void
mem_clear(void *ptr, int size)
{
  assert(size > 0);
  /**/
  memset(ptr, 0, size);
}

/*----------------------------------------------------------*/
void
mem_free(void *ptr)
{
  assert(ptr != NULL);
  /**/
  free(ptr);
}

/*----------------------------------------------------------*/
void *
mem_realloc(void *ptr, int size)
{
  void *new_ptr = NULL;
  /**/
  assert(ptr != NULL);
  assert(size > 0);
  /**/
  new_ptr = realloc(ptr, size);
  if (new_ptr == NULL) {
    fprintf(stderr, "%s",
      "uacc: error: sorry, not enough memory\n"
    );
    exit(EXIT_FAILURE);
  }
  return new_ptr;
}

/*----------------------------------------------------------*/
void *
mem_realloc_zeros(void *ptr, int size, int old_size)
{
  void *new_ptr = NULL;
  int num_new_bytes = 0;
  /**/
  assert(ptr != NULL);
  assert(size > 0);
  assert(old_size > 0);
  /**/
  if (size <= old_size) {
    return ptr;
  }
  num_new_bytes = size - old_size;
  new_ptr = mem_realloc(ptr, size);
  mem_clear((char *)new_ptr + old_size, num_new_bytes);
  return new_ptr;
}

/*----------------------------------------------------------*/
/* IMPLEMENTATION: STRING BUFFER                            */
/*----------------------------------------------------------*/

/*----------------------------------------------------------*/
void
sb_append(Strbuf *sb, const char *fmt, ...)
{
  va_list args1;
  va_list args2;
  /**/
  assert(sb != NULL);
  assert(sb->is_inited);
  /**/
  va_start(args1, fmt);
  va_start(args2, fmt);
  sb_vreplace(sb, sb->length, 0, fmt, args1, args2);
  va_end(args1);
  va_end(args2);
}

/*----------------------------------------------------------*/
char *
sb_at(Strbuf *sb, int i)
{
  assert(sb != NULL);
  assert(sb->is_inited);
  /**/
  if (sb->length == 0) {
    return sb->at;
  }
  i = normalize_index(i, sb->length);
  return &sb->at[i];
}

/*----------------------------------------------------------*/
void
sb_clear(Strbuf *sb)
{
  assert(sb != NULL);
  assert(sb->is_inited);
  /**/
  sb->length = 0;
  mem_clear(sb->at, sb->capacity);
}

/*----------------------------------------------------------*/
void
sb_copy(Strbuf *sb, const char *fmt, ...)
{
  va_list args1;
  va_list args2;
  /**/
  assert(sb != NULL);
  assert(sb->is_inited);
  /**/
  va_start(args1, fmt);
  va_start(args2, fmt);
  sb_vreplace(sb, 0, sb->length, fmt, args1, args2);
  va_end(args1);
  va_end(args2);
}

/*----------------------------------------------------------*/
void
sb_deinit(Strbuf *sb)
{
  assert(sb != NULL);
  assert(sb->is_inited);
  /**/
  mem_free(sb->at);
  mem_clear(sb, sizeof(*sb));
}

/*----------------------------------------------------------*/
void
sb_init(Strbuf *sb)
{
  assert(sb != NULL);
  assert(!sb->is_inited);
  /**/
  sb->length = 0;
  sb->capacity = 16;
  sb->at = mem_alloc_zeros(sb->capacity);
  sb->is_inited = 1;
}

/*----------------------------------------------------------*/
void
sb_insert(Strbuf *sb, int i, const char *fmt, ...)
{
  va_list args1;
  va_list args2;
  /**/
  assert(sb != NULL);
  assert(sb->is_inited);
  /**/
  va_start(args1, fmt);
  va_start(args2, fmt);
  sb_vreplace(sb, i, 0, fmt, args1, args2);
  va_end(args1);
  va_end(args2);
}

/*----------------------------------------------------------*/
void
sb_remove(Strbuf *sb, int i, int n)
{
  assert(sb != NULL);
  assert(sb->is_inited);
  /**/
  sb_replace(sb, i, n, "");
}

/*----------------------------------------------------------*/
void
sb_replace(Strbuf *sb, int i, int n, const char *fmt, ...)
{
  va_list args1;
  va_list args2;
  /**/
  assert(sb != NULL);
  assert(sb->is_inited);
  /**/
  va_start(args1, fmt);
  va_start(args2, fmt);
  sb_vreplace(sb, i, n, fmt, args1, args2);
  va_end(args1);
  va_end(args2);
}

/*----------------------------------------------------------*/
void
sb_reserve(Strbuf *sb, int cap)
{
  assert(sb != NULL);
  assert(sb->is_inited);
  /**/
  if (cap <= sb->capacity) {
    return;
  }
  sb->at = mem_realloc_zeros(sb->at, cap, sb->capacity);
  sb->capacity = cap;
}

/*----------------------------------------------------------*/
void
sb_vreplace(Strbuf *sb, int i, int n,
            const char *fmt, va_list args1, va_list args2)
{
  char *src = NULL;
  char *dst = NULL;
  int placed = 0;
  int removed = 0;
  int moved = 0;
  int pos = 0;
  int new_length = 0;
  int new_size = 0;
  char temp_char = 0;
  /**/
  assert(sb != NULL);
  assert(sb->is_inited);
  assert(n >= 0);
  /**/
  pos = i;
  if (i != sb->length) {
    pos = normalize_index(i, sb->length);
  }
  removed = n;
  if (removed > sb->length) {
    removed = sb->length;
  }
  placed = vfprintf(G->fnull, fmt, args1);
  new_length = sb->length - removed + placed;
  new_size = new_length + 1;
  if (new_size > sb->capacity) {
    sb_reserve(sb, new_size * 2);
  }
  /**/
  moved = sb->length - pos - removed;
  if (moved > 0) {
    src = sb->at + pos + removed;
    dst = sb->at + pos + placed;
    memmove(dst, src, moved);
  }
  if (placed != 0) {
    temp_char = sb->at[pos + placed];
    vsprintf(sb->at + pos, fmt, args2);
    sb->at[pos + placed] = temp_char;
  }
  /**/
  sb->length = new_length;
  sb->at[sb->length] = '\0';
}

/*----------------------------------------------------------*/
/* IMPLEMENTATION:                                          */
/*----------------------------------------------------------*/

/*----------------------------------------------------------*/
int
normalize_index(int i, int n)
{
  while (i < 0) {
    i += n;
  }
  while (i >= n) {
    i -= n;
  }
  return i;
}

