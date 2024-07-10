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
#include "deltmp.h"

//
// Let's go!
//
int main(int argc, char *argv[]) {
	int nb_files;
	float files_weight;
	
	nb_files = 0;
	files_weight = 0;

	// Not enough arguments ?!
	//printf("%d\n", argc);
	if ( argc < 3 ) {
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
		printf("'%s'\n", argv[2]);
		return EXIT_FAILURE;
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


/*
== Microsoft Windows XP purgation ==
Folders and files to purge:
	. Documents and Settings/<*>
		-> not All Users
		-> not Default Users
		-> not LocalService
		-> not NetworkService
			(...)/Local Settings/Temp/*
			(...)/Local Settings/Temporary Internet Files/*
			(...)/Recent/*
			(...)/Cookies/*
				-> not index.dat
	. RECYCLER/<*>/*
	. WINDOWS/Prefetch/*
		-> not Layout.ini
	. WINDOWS/Temp/*
	. WINDOWS/$*
*/
static int purge_xp(char *path) {
	//struct stat infos;
	char **files;
	char **values_to_delete;
	int i;
	
	i = 0;
	files = (char**)malloc(SIZE_USERNAME*100 * sizeof(char*));
	
	// Let's go!
	printf("[-] OS  : Microsoft Windows XP\n");
	printf("[+] Scanning ...\n");

	// Documents and Settings/<*>
	//		-> not All Users
	//		-> not Default Users
	//		-> not LocalService
	//		-> not NetworkService
	//
	if ( files ) {
		if ( ls(strcat(path, "Documents and Settings\\"), "*.*", files, 0, 1) ) {
			LS_ERROR;
			return EXIT_FAILURE;
		}
		while ( files[i] != '\0' && strcmp(files[i], "") != 0 ) {
			printf("%s\n", files[i]);
			i++;
		}
	} else {
		MALLOC_ERROR;
		return EXIT_FAILURE;
	}

	/*stat(file, &infos);
	++files;
	weight += infos.st_size;
	printf("Deleted files: %d (%f)\n", files, weight);*/
	//free(pfiles);
	return EXIT_SUCCESS;
}
