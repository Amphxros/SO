#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"

/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

int print_text_file(char *path)
{
    /* To be completed (part A) */
    FILE *file = fopen(path, "r");
    if (!file)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[MAXLEN_LINE_FILE];
    while (fgets(line, MAXLEN_LINE_FILE, file) != NULL)
    {
        //char* token = strsep(&line, ":"); use this to split 
        
        printf("%s", line);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

int print_binary_file(char *path)
{
    /* To be completed (part B) */
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int num_entries;
    fread(&num_entries, sizeof(int), 1, file);

    student_t student;
    for (int i = 0; i < num_entries; i++)
    {
        fread(&student, sizeof(student_t), 1, file);
        printf("[Entry #%d]\n", i);
        printf("\tstudent_id=%d\n", student.student_id);
        printf("\tNIF=%s\n", student.NIF);
        printf("\tfirst_name=%s\n", student.first_name);
        printf("\tlast_name=%s\n", student.last_name);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

int write_binary_file(char *input_file, char *output_file)
{
    /* To be completed (part C) */
    FILE *input = fopen(input_file, "r");
    FILE *output = fopen(output_file, "wb");
    if (!input || !output)
    {
        perror("Error opening files");
        return EXIT_FAILURE;
    }

    int num_entries;
    fscanf(input, "%d", &num_entries);
    fwrite(&num_entries, sizeof(int), 1, output);

    student_t student;
    for (int i = 0; i < num_entries; i++)
    {
        fscanf(input, "%d:%9[^:]:%19[^:]:%19[^\n]",
               &student.student_id, student.NIF, student.first_name, student.last_name);
        fwrite(&student, sizeof(student_t), 1, output);
    }

    fclose(input);
    fclose(output);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    int ret_code, opt;
    struct options options;

    /* Initialize default values for options */
    options.input_file = NULL;
    options.output_file = NULL;
    options.action = NONE_ACT;
    ret_code = 0;

    /* Parse command-line options */
    while ((opt = getopt(argc, argv, "h:o:i:c")) != -1)
    {
        switch (opt)
        {
        case 'h':
            fprintf(stderr, "Usage: %s [ -h ]\n", argv[0]);
            exit(EXIT_SUCCESS);
        case 'i':
            options.input_file = optarg;
            options.action = PRINT_TEXT_ACT;
            break;
        case 'o':
            options.output_file = optarg;
			options.input_file=optarg;
            options.action = WRITE_BINARY_ACT;
            break;
        case 'c':
            options.input_file=optarg;
            options.action = PRINT_BINARY_ACT;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    if (options.input_file == NULL)
    {
        fprintf(stderr, "Must specify one record file as an argument of -i\n");
        exit(EXIT_FAILURE);
    }

    switch (options.action)
    {
    case NONE_ACT:
        fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
        ret_code = EXIT_FAILURE;
        break;
    case PRINT_TEXT_ACT:
        ret_code = print_text_file(options.input_file);
        break;
    case WRITE_BINARY_ACT:
        ret_code = write_binary_file(options.input_file, options.output_file);
        break;
    case PRINT_BINARY_ACT:
        ret_code = print_binary_file(options.input_file);
        break;
    default:
        break;
    }
    exit(ret_code);
}