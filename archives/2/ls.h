#include <io.h>

// Errors
#define MALLOC_ERROR fprintf(stderr, "[!] %s %d : malloc() failed.\n", __FILE__, __LINE__)


//
// Fonction	: ls
// Objectif	: lister un répertoire.
// Entrées	:
//		- (char) chemin du répertoire
//		- (char) regexp à chercher dans les noms de fichier (par défaut *.*)
//		- (array) tableau dans lequel stocker les résultats
// Sortie	:
//		- (int) 0 si aucun fichier
//		- (int) 1 si une erreur est survenue
// MAJ		: 11/03/2010
//
int ls(char *path, char *match, char **files, int size_max) {
	struct _finddata_t folder;
	int done;
	int i;
	int hd;
	
	done = 0;
	i = 0;

	if ( ! strcmp(path, "") || _chdir(path) ) {
		return EXIT_FAILURE;
	}
	if ( ! strcmp(match, "") ) {
		match = "*.*";
	}
	hd = _findfirst(match, &folder);
	if ( hd != -1 ) {
		while ( ! done )  {
			if ( strcmp(folder.name, ".") != 0 && strcmp(folder.name, "..") != 0 ) {
				files[i] = (char*)malloc(strlen(folder.name) * sizeof(char));
				
				if ( files[i] ) {
					strcpy(files[i], folder.name);
					i++;
				} else {
					MALLOC_ERROR;
					return EXIT_FAILURE;
				}
			}
			done = _findnext(hd, &folder);
		}
	}
	return EXIT_SUCCESS;
} //fin ls
