#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
int main(int argc, char* argv[]){
    int retCode, opt;
     /* Parse command-line options */
    while ((opt = getopt(argc, argv, "a:b:c:d:e:f")) != -1) {
        switch (opt) {
        case 'a':
        
        break;
        case 'b':
           
            break;
        case 'c':
            
            break;
        case 'd':
            
            break;
        case 'e':
            
            break;
        case 'f':
            
            break;
        default:
            
        }
    }

    return 0;
}