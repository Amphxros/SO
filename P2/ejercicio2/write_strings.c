#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

void write_strings(FILE* file, int argc, char* argv[]){
	for(int i=2; i<argc;i++){
		fwrite(argv[i], 1, strlen(argv[i]) + 1, file);
	}
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
        fprintf(stderr, "Usage: %s <output_file> <string1> <string2> ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
	 FILE* file = fopen(argv[1], "wb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    write_strings(file, argc, argv);
	fclose(file);
	
	return 0;
}