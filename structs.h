#ifndef _STRUCTS_H_
#define _STRUCTS_H_
#include <stdbool.h>

typedef struct{
    bool INDEX;
    bool LONG;
    bool RECURSIVE;
    bool MULTIPLE_DIR;
} FLAGS;

typedef struct {
    int INODE_LEN;
    int LINKS_LEN;
    int USER_LEN;
    int GROUP_LEN;
    int BYTES_LEN;
    int NAME_LEN;
} columnLength;

#endif