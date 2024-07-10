/*
	Delete Tomporary files
	----------------------

	This tool was written to purge temporary files on a mounted system.
	It is full functional with:
	- Microsoft Windows XP, Vista, Seven
	- GNU/Linux systems
	- MAC systems

	Written by BoboTiG [ http://bobotig.fr ] <bobotig@gmail.com>
	Creation: 08 november 2009
	Last update: 14 march 2010

	License: GNU GPL version 3 or higher.
*/

// Includes and defines
#include "deltmp.h"

//
// Let's go!
//
int main(int argc, char *argv[]) {
	unsigned int nb_files;
	float files_weight;
	
	nb_files = 0;
	files_weight = 0;

	// Not enough arguments ?!
	//printf("%d\n", argc);
	if ( argc != 3 ) {
		help();
		return EXIT_FAILURE;
	}
	
	printf("\n>>> %s v.%s\n\n", TOOL, VERSION);
	printf("[-] Path: %s\n", argv[1]);
	strcat(argv[1], SEPARATOR);
	
	// OS specifications
	if ( ! strcmp(argv[2], "xp") ) {
		purge_xp(argv[1]);
	} else if ( ! strcmp(argv[2], "vista") ) {
	} else if ( ! strcmp(argv[2], "seven")  ) {
	} else if ( ! strcmp(argv[2], "linux") ) {
	} else if ( ! strcmp(argv[2], "mac") ) {
	} else {
		fprintf(stderr, WRONG_OS);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
