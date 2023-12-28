#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"

/**
 * @brief  Make a copy of the existing string allocating memory accordingly
 *
 * @param original
 * @return new string that is a clone of the original
 **/
static inline char* clone_string(const char* original)
{
    return strdup(original);
}

/**
 * @brief Parse /etc/passwd file and return its contents as an array of entries.
 * The number of entries in the array is returned via the nr_entries parameter.
 *
 * @param options
 * @param nr_entries
 * @return array of entries (passwd_entry_t*) and entry count (nr_entries parameter)
 */
passwd_entry_t* parse_passwd(struct options* options, int* nr_entries)
{
    FILE* passwd;
    char line[MAX_PASSWD_LINE + 1];
    passwd_entry_t* entries;
    passwd_entry_t* cur_entry;
    int entry_count = 0;
    int cur_line = 1;

    if ((passwd = fopen(options->inputfile, "r")) == NULL) {
        fprintf(stderr, "%s could not be opened: ", options->inputfile);
        perror(NULL);
        return NULL;
    }

    /* Figure out the number of lines */
    while (fgets(line, MAX_PASSWD_LINE + 1, passwd) != NULL) {
        entry_count++;
    }

    /* Rewind position indicator */
    fseek(passwd, 0, SEEK_SET);

    entries = malloc(sizeof(passwd_entry_t) * entry_count);
    /* zero fill the array of structures */
    memset(entries, 0, sizeof(passwd_entry_t) * entry_count);

    /* Parse file */
    int entry_idx = 0;
    while (fgets(line, MAX_PASSWD_LINE + 1, passwd) != NULL) {
        /* Point to the beginning of the line */
        char* lineptr = line;
        token_id_t token_id = LOGIN_NAME_IDX;
        cur_entry = &entries[entry_idx];

        while ((token_id <= USER_SHELL_IDX) && (lineptr != NULL)) {
            char* token = strsep(&lineptr, ":");
            switch (token_id) {
            case LOGIN_NAME_IDX:
                strcpy(cur_entry->login_name, token);
                break;
            case ENCRYPTED_PASS_IDX:
                cur_entry->optional_encrypted_passwd = clone_string(token);
                break;
            case UID_IDX:
            case GID_IDX:
                if (sscanf(token, "%d", (token_id == UID_IDX) ? &cur_entry->uid : &cur_entry->gid) != 1) {
                    fprintf(stderr, "Warning: Wrong format for %s field in line %d\n",
                            (token_id == UID_IDX) ? "UID" : "GID", cur_line);
                    (token_id == UID_IDX) ? (cur_entry->uid = 0) : (cur_entry->gid = 0);
                }
                break;
            case USER_NAME_IDX:
                cur_entry->user_name = clone_string(token);
                break;
            case USER_HOME_IDX:
                cur_entry->user_home = clone_string(token);
                break;
            case USER_SHELL_IDX:
                /* remove new line from token */
                token[strlen(token) - 1] = '\0';
                cur_entry->user_shell = clone_string(token);
                break;
            default:
                break;
            }
            token_id++;
        }

        if (token_id != NR_FIELDS_PASSWD) {
            fprintf(stderr, "Could not process all tokens from line %d\n", entry_idx + 1);
            return NULL;
        }

        entry_idx++;
        cur_line++;
    }

    (*nr_entries) = entry_count;
    fclose(passwd);
    return entries;
}

/**
 * @brief Free up the array of entries, including the
 * memory of dynamically allocated strings
 *
 * @param entries
 * @param nr_entries
 */
static void free_entries(passwd_entry_t* entries, int nr_entries)
{
    for (int i = 0; i < nr_entries; i++) {
        passwd_entry_t* entry = &entries[i];
        free(entry->optional_encrypted_passwd);
        free(entry->user_name);
        free(entry->user_home);
        free(entry->user_shell);
    }

    free(entries);
}

/**
 * @brief This function takes care of invoking the parser,
 * and then displays the contents of /etc/passwd based on the
 * selected output mode
 *
 * @param options
 * @return int
 */
static int show_passwd(struct options* options)
{
    int nr_entries;
    int retCode;
    passwd_entry_t* entries = parse_passwd(options, &nr_entries);

    if (!entries)
        return EXIT_FAILURE;

    for (int i = 0; i < nr_entries; i++) {
        passwd_entry_t* e = &entries[i];
        switch (options->output_mode) {
        case VERBOSE_MODE:
            fprintf(options->outfile, "[Entry #%d]\n", i);
            fprintf(options->outfile, "\tlogin=%s\n\tenc_pass=%s\n\t"
                                      "uid=%d\n\tgid=%d\n\tuser_name=%s\n\t"
                                      "home=%s\n\tshell=%s\n",
                    e->login_name, e->optional_encrypted_passwd,
                    e->uid, e->gid, e->user_name,
                    e->user_home, e->user_shell);
            break;
        case PIPE_MODE:
            fprintf(options->outfile, "|%s|%s|%d|%d|%s|%s|%s|\n",
                    e->login_name, e->optional_encrypted_passwd,
                    e->uid, e->gid, e->user_name,
                    e->user_home, e->user_shell);
            break;
        case CSV_MODE:
            fprintf(options->outfile, "%s,%s,%d,%d,%s,%s,%s\n",
                    e->login_name, e->optional_encrypted_passwd,
                    e->uid, e->gid, e->user_name,
                    e->user_home, e->user_shell);
            break;
        }
    }

    free_entries(entries, nr_entries);
    retCode = EXIT_SUCCESS;

    return retCode;
}

int main(int argc, char* argv[])
{
    int retCode, opt;
    struct options options;

    /* Initialize default values for options */
    options.outfile = stdout;
    options.output_mode = VERBOSE_MODE;
    options.inputfile = "/etc/passwd"; // Default input file

    /* Parse command-line options */
    while ((opt = getopt(argc, argv, "hvp:o:i:c")) != -1) {
        switch (opt) {
        case 'h':
            fprintf(stderr, "Usage: %s [ -h | -v | -p | -o <output_file> | -i <input_file> | -c ]\n", argv[0]);
            exit(EXIT_SUCCESS);
        case 'v':
            options.output_mode = VERBOSE_MODE;
            break;
        case 'p':
            options.output_mode = PIPE_MODE;
            break;
        case 'o':
            if ((options.outfile = fopen(optarg, "wx")) == NULL) {
                fprintf(stderr, "The output file %s could not be opened: ", optarg);
                perror(NULL);
                exit(EXIT_FAILURE);
            }
            break;
        case 'i':
            options.inputfile = optarg;
			
            break;
        case 'c':
            options.output_mode = CSV_MODE;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    retCode = show_passwd(&options);
    fclose(options.outfile);

    return retCode;
}
