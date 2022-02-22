#ifndef _COMMON_H_
#define _COMMON_H_

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "structs.h"

#define MAX_PATH_LEN 4096

// Returns 1 if filepath arg is a file 0 otherwise
int checkIfFile(char* arg);

// Filter for scandir()
int scanFilter(const struct dirent* entry);

// Function which parses -i -l -R flags using getopt()
int parseOptions(FLAGS* flags, int argc, char** argv);

// Function which takes in a set of file and path args and sorts them such that
// the files are first in the args array pathArgs then the paths are sorted after them
// This is done using ASCII comparison as read in clarifications.
void parsePaths(char** pathArgs, int optind, int numOfPathArgs, char** argv);

#endif