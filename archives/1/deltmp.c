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
	Last update: 03.07.2010

	License: GNU GPL, version 2 or higher

	Compile with gcc deltmp.c -o deltmp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// Global
#define TOOL "deltmp"
#define VERSION "0.0.1"

// Errors
#define WRONG_OS "[!] Wrong OS - accepted: xp|vista|seven|linux|mac.\n"
#define END_OF_PATH_ERR "[!] The path has to finsih by '/' or '\\'.\n"

// Prototypes
static void help();
static int purge_xp(char* path);

// Variables
static int files; // Number of deleted files
static float weight; // Total weight of deleted files


//
// Let's go!
//
int main(int argc, char *argv[]) {
	// Variable
	char separator = '/';

	// Initialisation
	files = 0;
	weight = 0;

	// Not enough arguments ?!
	if ( argc < 3 ) {
		help();
		exit(EXIT_FAILURE);
	} else {
		printf(">>> %s v.%s\n", TOOL, VERSION);
	}


	if ( argv[1][(strlen(argv[1]) - 1)] != '/' && argv[1][(strlen(argv[1]) - 1)] != '\\' ) {
		printf("'%s'", argv[1]);
		fprintf(stderr, END_OF_PATH_ERR);
		exit(EXIT_FAILURE);
	} else {
		separator = argv[1][(strlen(argv[1]) - 1)];
		printf("[-] Path: %s\n", argv[1]);
	}

	// OS specifications
	if ( ! strcmp(argv[2], "xp") ) {
		purge_xp(argv[1]);
	} else if ( ! strcmp(argv[2], "vista") ) {
	} else if ( ! strcmp(argv[2], "seven")  ) {
	} else if ( ! strcmp(argv[2], "linux") ) {
	} else if ( ! strcmp(argv[2], "mac") ) {
	} else {
	fprintf(stderr, WRONG_OS);
	exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}


//
// Help function.
//
static void help() {
  printf(">>> %s v.%s\n", TOOL, VERSION);
  printf("\nUsage: %s <mount_point> <system> [-v]\n", TOOL);
  printf("\nExample 1: %s /mnt/xp/ xp", TOOL);
  printf("\nExample 2: %s C:\\ xp\n\n", TOOL);
  printf("\nReport bugs and suggestions to bobotig@gmail.com.\n");
}


//
// XP purgation function.
//
static int purge_xp(char* path) {
	/*
	== Microsoft Windows XP ==
	Folders and files to purge:
		. Documents and Settings/<*>/Local Settings/Temp/*
		. Documents and Settings/<*>/Local Settings/Temporary Internet Files/*
		. Documents and Settings/<*>/Recent/*
		. Documents and Settings/<*>/Cookies/*
			-> not index.dat
		. RECYCLER/<*>/*
		. WINDOWS/Prefetch/*
			-> not Layout.ini
		. WINDOWS/Temp/*
		. WINDOWS/$*
	*/

	// Variables
	struct stat infos;
	char *file;

	// Let's go!
	printf("[-] OS: Microsoft Windows XP\n");
	printf("[+] Scanning ...\n");

	// Documents and Settings/<*>/Local Settings/temp/*
	sprintf(file, "%spagefile.sys", path);
	stat(file, &infos);
	++files;
	weight += infos.st_size;
	printf("Deleted files: %d (%f)\n", files, weight);
	return EXIT_SUCCESS;
}
