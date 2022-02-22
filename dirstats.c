#include "dirstats.h"

void getPath(char* path, int pathLen, char* arg)
{
    strcat(path, arg);
    char lastChar = arg[strlen(arg) - 1];
    if (lastChar != '/')
    {
        char slash[] = "/";
        strcat(path, slash);
    }
    return;
}

void getColumnLengths(columnLength* maxLengths, struct dirent* direntp, struct stat fileStat)
{
    // Get the single entry lengths
    int inode_len = (fileStat.st_ino   == 0) ? 1 : (floor(log10(fileStat.st_ino)) + 1);
    int links_len = (fileStat.st_nlink == 0) ? 1 : (floor(log10(fileStat.st_nlink)) + 1);
    int bytes_len = (fileStat.st_size  == 0) ? 1 : (floor(log10(fileStat.st_size)) + 1); 
    int user_len  = strlen(getUserString(fileStat));
    int group_len = strlen(getGroupString(fileStat));
    int name_len  = strlen(direntp->d_name);

    // Update the maxlengths if needed
    if (inode_len > maxLengths->INODE_LEN) maxLengths->INODE_LEN = inode_len;
    if (links_len > maxLengths->LINKS_LEN) maxLengths->LINKS_LEN = links_len;
    if (bytes_len > maxLengths->BYTES_LEN) maxLengths->BYTES_LEN = bytes_len;
    if (user_len  > maxLengths->USER_LEN)  maxLengths->USER_LEN  = user_len;
    if (group_len > maxLengths->GROUP_LEN) maxLengths->GROUP_LEN = group_len;
    if (name_len  > maxLengths->NAME_LEN)  maxLengths->NAME_LEN  = name_len;
    
    return;
}

void getSingleColumnLength(columnLength* maxLengths, struct stat fileStat, char* filename)
{
    maxLengths->INODE_LEN = (fileStat.st_ino   == 0) ? 1 : (floor(log10(fileStat.st_ino)) + 1);
    maxLengths->LINKS_LEN = (fileStat.st_nlink == 0) ? 1 : (floor(log10(fileStat.st_nlink)) + 1);
    maxLengths->BYTES_LEN = (fileStat.st_size  == 0) ? 1 : (floor(log10(fileStat.st_size)) + 1); 
    maxLengths->USER_LEN  = strlen(getUserString(fileStat));
    maxLengths->GROUP_LEN = strlen(getGroupString(fileStat));
    maxLengths->NAME_LEN  = strlen(filename);

    return;
}