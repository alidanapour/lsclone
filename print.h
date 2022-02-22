#ifndef _PRINT_H_
#define _PRINT_H_

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>
#include <libgen.h>
#include "common.h"
#include "dirstats.h"
#include "structs.h"

// Returns the username belonging to the file stat
char* getUserString(struct stat fileStat);

// Returns the group name belonging to the file stat
char* getGroupString(struct stat fileStat);

// Print an entire directory
void printDirectory(struct dirent **namelist, int NUM_ENTRIES, FLAGS* flags, char* basePath);

// Print a single file
void printFile(FLAGS* flags, char* filePath);

#endif