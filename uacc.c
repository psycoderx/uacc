/* Unique ANSI C Compiler */
/* uacc.c */

/*----------------------------------------------------------*/
/* INCLUDES                                                 */
/*----------------------------------------------------------*/

#include "uacc.h"

/*----------------------------------------------------------*/
/* DEFINES                                                  */
/*----------------------------------------------------------*/

/*
Version string in format a.b.c where
a - 1 if ANSI C is implemented and 0 if not yet,
b - release number (unique for each a),
c - bugfix number (unique for each b).
*/
#define UACC_VERSION "0.1.0"

/*----------------------------------------------------------*/
/* STATIC FUNCTIONS                                         */
/*----------------------------------------------------------*/

/*
Print the help message to `stdout`.
*/
static void
print_help(void);

/*----------------------------------------------------------*/
/* VARIABLES                                                */
/*----------------------------------------------------------*/

/*
The actual location of global variables.
*/
static Globals static_G;

/*
Vector to global variables.
*/
Globals *G = &static_G;

/*----------------------------------------------------------*/
/* IMPLEMENTATION                                           */
/*----------------------------------------------------------*/

/*----------------------------------------------------------*/
int
main(int argc, char *argv[])
{
  int i = 0;
  const char *fnull_name = "/dev/null";
  /**/
  if (argc < 2) {
    print_help();  
    exit(EXIT_SUCCESS);
  }
  /**/
  G->fnull = fopen(fnull_name, "wb");
  if (G->fnull == NULL) {
    fprintf(stderr, "%s%s%s%s",
      fnull_name, ": ", strerror(errno), "\n"
    );
    exit(EXIT_FAILURE);
  }
  /**/
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0) {
      print_help();  
      exit(EXIT_SUCCESS);
    }
  }
  printf("Hello, World!!!\n");
  exit(EXIT_SUCCESS);
  return 0;
}

/*----------------------------------------------------------*/
void
print_help(void)
{
  printf("%s",
    "      VERSION\n"
    "uacc "UACC_VERSION"\n"
    "Copyright (C) 2024 Artem Pirunov\n"
    "\n"
    "      USAGE\n"
    "uacc [options] file...\n"
    "\n"
    "      OPTIONS\n"
    "  --help\n"
    "Display this information.\n"
    "\n"
  );
}

