//
// Fonction	: ls
// Objectif	: lister un répertoire (récursivité possible).
// Entrées	:
//		- (char) chemin du répertoire
//		- (char) regexp à chercher dans les noms de fichier (par défaut *.*)
//		- (array) tableau dans lequel stocker les résultats
// Sortie	:
//		- (int) 0 si aucun fichier
//		- (int) 1 si une erreur est survenue
// MAJ		: 11/03/2010
//
int ls(char *path, char *match, char **files, int first_key, int recursive) {
	struct _finddata_t folder;
	char current_path[1024];
	int done;
	int hd;
	
	done = 0;
	strcpy(current_path, path);

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
				strcat(current_path, folder.name);
				files[first_key] = (char*)malloc(strlen(current_path) * sizeof(char));

				if ( recursive && folder.attrib&0x10 ) {
					strcat(current_path, SEPARATOR);
					if ( ls(current_path, match, files, first_key, recursive) ) {
						LS_ERROR;
						return EXIT_FAILURE;
					}
				} else {
					if ( files[first_key] ) {
						strcpy(files[first_key], current_path);
						first_key++;
					} else {
						MALLOC_ERROR;
						return EXIT_FAILURE;
					}
				}
				strcpy(current_path, path);
			}
			done = _findnext(hd, &folder);
		}
	}
	return EXIT_SUCCESS;
} //fin ls
