/*
	Delete Tomporary files
	----------------------

	This tool was written to purge temporary files on a mounted system.
	It is full functional with:
	- Microsoft Windows XP, Vista, Seven
	- GNU/Linux systems
	- MAC systems

	Writen by BoboTiG [ bobotig@gmail.com ]
	Creation: 08.11.2009
	Last update: 11.03.2010

	License: GNU GPL, version 2 or higher.
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
#define SIZE_USERNAME 10
#define SIZE_MAX_LEN 256

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
static int purge_xp(char *path);
