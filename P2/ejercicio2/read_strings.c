#include <stdio.h>
#include <stdlib.h>

char* loadstr(FILE* input) {
    // Determine the length of the string
    long pos = ftell(input);
    long length = 0;
	//calculate the length of the file
    while (fgetc(input) != '\0') {
        length++;
    }

    // Return to the starting position
    fseek(input, pos, SEEK_SET);

    // Allocate memory for the string 
    char* str = (char*)malloc(length + 1);
    if (!str) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Read the string
    fread(str, 1, length + 1, input);

    return str;
}

void read_strings( FILE* file){
	while(!feof(file)){
		char * line= loadstr(file);
		printf("%s\n", line);
		free(line);
		if(feof(file)){
			break;
		}
	}
}

int main(int argc, char* argv[]){
	if(argc !=2){
		  fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(1);
	}

	FILE* file= fopen(argv[1], "rb");

	if(!file){
		perror("File not existent");
		exit(1);
	}
	
	read_strings(file);
	return 0;

}