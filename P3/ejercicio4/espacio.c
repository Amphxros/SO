#include <stdio.h>
#include <stdlib.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	struct stat stat_buffer;

	if(lstat(fname, &stat_buffer)==-1){
		perror("Error al obtener información del fichero");
        return -1;
	}

	if(S_ISDIR(stat_buffer.st_mode)){
		return get_size_dir(fname,blocks);
	}
	else{
		*blocks+=stat_buffer.st_blocks;
		return 0;
	}

}

/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
	DIR* dir= opendir(dname);
	if(dir==NULL){
		 perror("Error al abrir el directorio");
        return -1;
	}

	struct dirent *entry;
	while((entry= readdir(dir)) !=NULL){
		// Ignorar las entradas . y ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char entry_path[PATH_MAX];
        snprintf(entry_path, PATH_MAX, "%s/%s", dname, entry->d_name);

        // Llamar a get_size para obtener el tamaño del fichero o directorio
        if (get_size(entry_path, blocks) == -1) {
            closedir(dir);
            return -1;
        }
	}
	closedir(dir);

	return 0;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	if (argc < 2) {
        fprintf(stderr, "Uso: %s <fichero1> [<fichero2> ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Procesar cada fichero de la línea de comandos
    for (int i = 1; i < argc; ++i) {
        size_t total_blocks = 0;
        if (get_size(argv[i], &total_blocks) == -1) {
            fprintf(stderr, "Error al obtener el tamaño del fichero %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }

        // Mostrar el tamaño total en kilobytes
        printf("%ldK %s\n", (long)(total_blocks * 512 / 1024), argv[i]);
    }

    return 0;
}