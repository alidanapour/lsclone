#ifndef _DIRSTATS_H_
#define _DIRSTATS_H_

#include "common.h"
#include "structs.h"
#include "print.h"

// A simple function which adds a '/' to the end of a string.
// NOTE! the path string must be + 1 larger and the pathlen must not
// include this extra space.
void getPath(char* path, int pathLen, char* arg);

// Returns the column lengths for a directory
void getColumnLengths(columnLength* maxLengths, struct dirent* direntp, struct stat fileStat);

// Returns the column lengths for a single file
void getSingleColumnLength(columnLength* maxLengths, struct stat fileStat, char* filename);

#endif