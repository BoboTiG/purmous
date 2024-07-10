#include <stdio.h>
#include <stdlib.h>
#include <io.h>

//
// Fonction	: ls
// Objectif	: lister un répertoire.
// Entrées	:
//		- (char) chemin du répertoire
//		- (char) regexp à chercher dans les noms de fichier (par défaut *.*)
// Sortie	:
//		- (int) 0 si aucun fichier
//		- (int) 1 si une erreur est survenue
// MAJ		: 11/03/2010
//
int ls(char* path, char* match) {
	struct _finddata_t folder;
	int done = 0;
	int hd;

	if ( strcmp(path, "") == 0 || _chdir(path) ) {
		return EXIT_FAILURE;
	}
	if ( strcmp(match, "") == 0 ) {
		match = "*.*";
	}
	
	hd = _findfirst(match, &folder);
	if ( hd != -1 ) {
		while ( ! done )  {
			if ( strcmp(folder.name, ".") != 0 && strcmp(folder.name, "..") != 0 ) {
				printf("%s\n", folder.name);
			}
			done = _findnext(hd, &folder);
		}
	}
	return EXIT_SUCCESS;
} //fin ls


int main(int argc, char *argv[]) {
	//printf("%d\n", argc);
	if ( argc != 3 ) {
		return EXIT_FAILURE;
	}
	ls(argv[1], argv[2]);
}
