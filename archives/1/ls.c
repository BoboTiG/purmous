#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void ls(char* path) {
    DIR* folder = NULL;
	struct dirent* file = NULL;

	if ( path == NULL ) {
		fprintf(stderr, "Erreur : nom de répertoire null\n");
		exit(1);
	}
	if ( opendir(path) == NULL ) {
		return ;
	}

	/*
        Ici, on sait qu'on a affaire à un répertoire, donc on le parcourt récursivement
    */
	if ( (folder = opendir(path)) == NULL ) {
		perror("opendir ");
		exit(1);
	}

	while ( (file = readdir(folder)) != NULL ) {
		/*
            Affichage inconditionnel du nom de fichier ou de repertoire
		*/
		if ( strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0 ) {
			printf("%s\n", file->d_name);
		}
	}
}

int main(int argc, char *argv[]) {
    ls(argv[1]);
}
