/*
	Delete Tomporary files
	----------------------

	This file (deltmp.h) is part of the deltmp project.
	Written by BoboTiG [ http://bobotig.fr ] <bobotig@gmail.com>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <io.h>

// Global
#define TOOL "deltmp"
#define VERSION "0.0.1"

// Errors
#define MALLOC_ERROR fprintf(stderr, "[!] %s %d : malloc() failed.\n", __FILE__, __LINE__)
#define LS_ERROR fprintf(stderr, "[!] %s %d : ls() failed.\n", __FILE__, __LINE__)
#define WRONG_OS "[!] Wrong OS - accepted: xp|vista|seven|linux|mac.\n"

// Array sizes
#define SIZE_MIN_LEN 10
#define SIZE_MAX_LEN 10240

// Separator
#ifdef WIN32
	#define SEPARATOR "\\"
#else
	#define SEPARATOR "/"
#endif

// Functions
#include "functions.h"

// Prototypes
static void help();
int in_array(char *value, char **array);
static int purge_xp(char *path);
