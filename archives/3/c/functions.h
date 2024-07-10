/*
	Delete Tomporary files
	----------------------

	This file (functions.h) is part of the deltmp project.
	Written by BoboTiG [ http://bobotig.fr ] <bobotig@gmail.com>
*/


//
// Fonction	: help
// Objectif	: afficher le message d'aide.
// Entrées	:
// Sortie	:
// MAJ		: 14/03/2010
//
static void help() {
  printf(">>> %s v.%s\n", TOOL, VERSION);
  printf("\nUsage: %s mount_point system\n", TOOL);
  printf("\nExample 1: %s /mnt/xp/ xp", TOOL);
  printf("\nExample 2: %s C: xp\n\n", TOOL);
  printf("\nReport bugs and suggestions to bobotig@gmail.com.\n");
} //fin help


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
static int ls(char *path, char *match, char **files, int first_key, int recursive, char **values_to_ignore) {
	struct _finddata_t fifo;
	char current_path[1024];
	unsigned int done;
	unsigned int hd;
	
	done = 0;
	strcpy(current_path, path);

	if ( ! strcmp(path, "") || _chdir(path) ) {
		LS_ERROR;
		return EXIT_FAILURE;
	}
	if ( ! strcmp(match, "") ) {
		match = "*.*";
	}
	hd = _findfirst(match, &fifo);
	if ( hd != -1 ) {
		while ( ! done )  {
			if ( strcmp(fifo.name, ".") != 0 && strcmp(fifo.name, "..") != 0 ) {
				if ( ! in_array(fifo.name, values_to_ignore) ) {
					strcat(current_path, fifo.name);
					/*if ( recursive && fifo.attrib&0x10 ) {
						strcat(current_path, SEPARATOR);
						files[first_key] = (char*)malloc(strlen(current_path) * sizeof(char));
						if ( files[first_key] ) {
							strcpy(files[first_key], current_path);
							printf("Dossier : '%s'\n", files[first_key]);
							first_key++;
							if ( ls(current_path, match, files, first_key, 1) ) {
								LS_ERROR;
								return EXIT_FAILURE;
							}
						} else {
							MALLOC_ERROR;
							return EXIT_FAILURE;
						}
					} else {*/
						files[first_key] = malloc(sizeof(files) * strlen(current_path) + 1);
						if ( files[first_key] != NULL ) {
							strcpy(files[first_key], current_path);
							//printf("Fichier : '%s'\n", files[first_key]);
							first_key++;
						} else {
							MALLOC_ERROR;
							return EXIT_FAILURE;
						}
					//}
				} else {
					printf("Fichier interdit : '%s'\n", fifo.name);
				}
				strcpy(current_path, path);
			}
			done = _findnext(hd, &fifo);
		}
	}
	return EXIT_SUCCESS;
} //fin ls


//
// Fonction	: in_array
// Objectif	: cherche une valeur dans un tableau.
// Entrées	:
//		- (array) tableau
//		- (char) valeur à chercher
// Sortie	:
//		- (int) 1 si trouvé, sinon 0
// MAJ		: 14/03/2010
//
int in_array(char *value, char **array) {
	size_t n;
	unsigned int i;
	
	n = sizeof array / sizeof array[0];
	i = 0;
	
	while ( i <= n ) {
		if ( ! strcmp(array[i], value) ) {
			return 1;
		}
		i++;
	}
	return 0;
} //fin in_array



//
// Fonction	: purge_xp
// Objectif	: Microsoft Windows XP purgation.
// Entrées	:
//		- (char) chemin du répertoire
// Sortie	:
//		- (int) EXIT_SUCCESS ou EXIT_FAILURE
// MAJ		: 14/03/2010
//
static int purge_xp(char *path) {
	/* Folders and files to purge:
		. Documents and Settings/<*>
			-> not All Users
			-> not Default User
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
	
	//struct stat infos;
	size_t n;
	char **files;
	char **values_to_ignore;
	unsigned int i;
	
	files = malloc(sizeof(files) * SIZE_MAX_LEN);
	values_to_ignore = malloc(sizeof(values_to_ignore) * SIZE_MIN_LEN);
	values_to_ignore[0] = malloc(sizeof(values_to_ignore[0]) * strlen("All Users") + 1);
	values_to_ignore[1] = malloc(sizeof(values_to_ignore[1]) * strlen("Default User") + 1);
	values_to_ignore[2] = malloc(sizeof(values_to_ignore[2]) * strlen("LocalService") + 1);
	values_to_ignore[3] = malloc(sizeof(values_to_ignore[3]) * strlen("NetworkService") + 1);
	strcpy(values_to_ignore[0], "All Users");
	strcpy(values_to_ignore[1], "Default User");
	strcpy(values_to_ignore[2], "LocalService");
	strcpy(values_to_ignore[3], "NetworkService");
	i = 0;
	
	// Let's go!
	printf("[-] OS  : Microsoft Windows XP\n");
	printf("[+] Scanning ...\n");

	// Documents and Settings/<*>
	//		-> not All Users
	//		-> not Default Users
	//		-> not LocalService
	//		-> not NetworkService
	//
	if ( files != NULL && values_to_ignore != NULL ) {
		if ( ls(strcat(path, "Documents and Settings\\"), "*.*", files, i, 1, values_to_ignore) ) {
			LS_ERROR;
			return EXIT_FAILURE;
		}
		n = sizeof files / sizeof files[0];
		while (files[i] != '\0' && strcmp(files[i], "") != 0 ) {
		//while ( i <= n ) {
			printf("%s\n", files[i]);
			i++;
		}
		free(files);
		files = NULL;
		free(values_to_ignore);
		values_to_ignore = NULL;
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
