#include "print.h"
#include "dirstats.h"
#include "structs.h"
#include "common.h"

// Forward declaration (see bottom of doc)
// Takes in a list of entries in a directory and recursively calls myls if an entry is sub-directory.
void recursiveSearch(struct dirent **namelist, int NUM_ENTRIES, char* basePath, int optind, char** argv);

int main(int argc, char* argv[])
{
    // Set up and retrieve flags
    optind = 1;    
    FLAGS* flags = (FLAGS*)malloc(sizeof(FLAGS));
    if (parseOptions(flags, argc, argv) != 0) return -1;

    // Setup an array of files or paths in args
    int numOfPathArgs = (argc - optind) == 0 ? 1 : (argc - optind);
    char* pathArgs[numOfPathArgs];
    parsePaths(pathArgs, optind, numOfPathArgs, argv);

    // Now iterate through files/paths
    for(int i = 0; i < numOfPathArgs; i++)
    {
        // Arg is a file
        if (checkIfFile(pathArgs[i]))
        {
            printFile(flags, pathArgs[i]);
        }
        // Arg is a directory
        else
        {
            // Get path of arg (may not end in /)
            int pathLen = strlen(pathArgs[i]);
            char* path = (char*)calloc(pathLen + 2, sizeof(char)); // + 2 In case we have to add a '/'
            getPath(path, pathLen, pathArgs[i]);
            
            // Get a list of dir entries using scandir
            struct dirent **namelist;
            int NUM_ENTRIES;
            NUM_ENTRIES = scandir(path, &namelist, scanFilter, alphasort);
            if (NUM_ENTRIES == -1)
            {
                printf("myls: cannot access: '%s' No such file or directory\n", path);
                free(path);
                free(flags);
                return -1;
            }

            // Print the directory for this path
            printDirectory(namelist, NUM_ENTRIES, flags, path);

            // Recursive portion, iterate again and if the entry is a folder
            if (flags->RECURSIVE) 
            {
                recursiveSearch(namelist, NUM_ENTRIES, path, optind, argv);
            }
            
            // Cleanup for next iteration
            for (int i = 0; i < NUM_ENTRIES; i++)
            {
                free(namelist[i]);
            }
            free(namelist);
            free(path);
        }
    }
    free(flags);
}

void recursiveSearch(struct dirent **namelist, int NUM_ENTRIES, char* basePath, int optind, char** argv)
{   
    // Iterate through sub-directories only
    for (int i=0; i < NUM_ENTRIES; i++){
        if (namelist[i]->d_type == DT_DIR){

            // Set up path arg
            char* filepathArg = (char*)calloc(strlen(basePath) + strlen(namelist[i]->d_name) + 1, sizeof(char));
            strcat(filepathArg, basePath);
            strcat(filepathArg, namelist[i]->d_name);

            // Set up new args (same flags + 1 path)
            char* finalArgs[optind + 1];
            for (int i = 0; i < optind; i++)
            {
                finalArgs[i] = argv[i];
            }
            finalArgs[optind] = filepathArg;

            // Recursion
            main(optind + 1, finalArgs);
      
            // Cleanup filepath and finalargs after recursion
            free(filepathArg);
            memset(finalArgs, 0, sizeof(finalArgs));
        }
    }
}