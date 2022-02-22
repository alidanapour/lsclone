#include "common.h"

static char DEFAULT[] = "./";

int checkIfFile(char* arg)
{
    struct stat fileStat;
    lstat(arg, &fileStat);
    return (S_ISREG(fileStat.st_mode) || S_ISLNK(fileStat.st_mode));
}

// Compare function for sort
static int compareFunc(const void *a, const void *b)
{
    return strcoll(*(const char **)a, *(const char **)b);
}

int scanFilter(const struct dirent* entry)
{
    if (entry->d_name[0] == '.') return 0;
    return 1;
}

int parseOptions(FLAGS* flags, int argc, char** argv)
{
    // Ensure all flags initially are false
    flags->INDEX        = false;
    flags->LONG         = false;
    flags->RECURSIVE    = false;
    flags->MULTIPLE_DIR = false;

    // Parse through flags
    int opt;
    while((opt = getopt(argc, argv, "+ilR")) != -1)
    {
        switch (opt) 
        {
            case 'i':
                flags->INDEX = true;
                break;
            case 'l':
                flags->LONG = true;
                break;
            case 'R':
                flags->RECURSIVE = true;
                break;
            case '?':
                return -1;
        }
    }

    // Determine if multiple file/path args were passed in
    if ((argc - optind) > 1) flags->MULTIPLE_DIR = true;
    return 0;
}

void parsePaths(char** pathArgs, int optind, int numOfPathArgs, char** argv)
{
    // Add all the path args to pathargs (if none, default to current directory)
    for (int i = 0; i < numOfPathArgs; i++)
    {
        pathArgs[i] = argv[i + optind];
    }    
    if (pathArgs[0] == NULL) pathArgs[0] = DEFAULT;

    // Move files to print ahead of array
    int nextFileSpot = 0;
    char* tempFilePtr;
    for (int i = 0; i < numOfPathArgs; i++)
    {
        if (checkIfFile(pathArgs[i])){
            tempFilePtr = pathArgs[i];
            pathArgs[i] = pathArgs[nextFileSpot];
            pathArgs[nextFileSpot] = tempFilePtr;
            nextFileSpot++;
        }
    }

    // Sort the Files among themselves
    qsort(pathArgs, nextFileSpot, sizeof(char*), compareFunc);
    // Sort the paths among themselves
    qsort(pathArgs + nextFileSpot, numOfPathArgs - nextFileSpot, sizeof(char*), compareFunc);

}