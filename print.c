#include "print.h"

#define NUM_CHARS 7
static int SPECIAL_CHARS[NUM_CHARS] = {' ','!','$','^','&','(',')'};
static char QUOTE_STR[] = "'";

// Takes a string of dashes and sets permissions
// https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
void getPermissionString(char* str, struct stat fileStat)
{
    int mode = fileStat.st_mode;
    if (S_ISDIR(mode))  str[0] = 'd';
    if (S_ISLNK(mode))  str[0] = 'l';
    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

// Sets a time string to str in format MMM DD YYYY HH:MM
void getTimeString(char* str, time_t modifyTime)
{
    strftime(str, 36, "%b %-2d %Y %H:%M", localtime(&modifyTime));
}

// Simple function that returns true if string str contains a special char
bool checkSpecialChar(char* str)
{
    for (int i = 0; i < NUM_CHARS; i++)
    {
        if(strchr(str, SPECIAL_CHARS[i]) != NULL) return true;
    } 
    return false;
}

// Returns the full file path with quotations if it contains a special char
char* getFilenameString(struct dirent* direntp, char* filepath)
{
    // Make a temp string, and copy from direnp(if avail) otherwise basename from path
    static char fileString[MAX_PATH_LEN];
    (direntp == NULL) ? strcpy(fileString, filepath/*basename(filepath)*/) : strcpy(fileString, direntp->d_name);

    // Check if string has special char (so special)
    bool HAS_SPECIAL = checkSpecialChar(fileString);

    // Has a special char!
    char temp[MAX_PATH_LEN];
    memset(temp, 0, sizeof(temp));
    if (HAS_SPECIAL)
    {
        // Build string and copy back to fileString
        strcat(temp, QUOTE_STR);
        strcat(temp, fileString);
        strcat(temp, QUOTE_STR);
        memset(fileString, 0, sizeof(fileString));
        strcpy(fileString, temp);
    }
    return fileString;
}

// Returns the linked item file path and adds quotations if needed
char* getLinkString(char* filePath)
{
    // Readlink into temp buffer and check if it has special
    char buf[MAX_PATH_LEN];
    memset(buf, 0, sizeof(buf));
    readlink(filePath, buf, sizeof(buf));
    bool HAS_SPECIAL;
    HAS_SPECIAL = checkSpecialChar(buf);

    // Return the link in proper way
    static char fileLink[MAX_PATH_LEN];
    memset(fileLink, 0 ,sizeof(fileLink));
    if (HAS_SPECIAL)
    {
        strcat(fileLink, QUOTE_STR);
        strcat(fileLink, buf);
        strcat(fileLink, QUOTE_STR);
    }
    else
    {
        strcat(fileLink, buf);
    }
    return fileLink;
}

char* getUserString(struct stat fileStat)
{
    // GET UID NAME
    struct passwd* Upws;
    Upws = getpwuid(fileStat.st_uid);
    return Upws->pw_name;
}

char* getGroupString(struct stat fileStat)
{
    // GET GID NAME
    struct group* Gpws;
    Gpws = getgrgid(fileStat.st_gid);
    return Gpws->gr_name;
} 

// Prints the -l (long) information for a file up to and not including the filename
void printLongEntry(columnLength maxLengths, struct stat fileStat, char* filepath)
{
    // GET PERMISSIONS
    char perm[12] = "----------";
    getPermissionString(perm, fileStat);

    // GET TIME
    char date[36];
    getTimeString(date, fileStat.st_mtime);
 
    // PRINT LONG INFO
    printf("%s ", perm);                                             // Permissions
    printf("%*ld ", maxLengths.LINKS_LEN, fileStat.st_nlink);        // Hard links to file
    printf("%-*s ", maxLengths.USER_LEN, getUserString(fileStat));   // User name
    printf("%-*s ", maxLengths.GROUP_LEN, getGroupString(fileStat)); // Group name
    printf("%*zd ", maxLengths.BYTES_LEN, fileStat.st_size);         // Filesize
    printf("%s ", date);                                             // Date
}

// Prints a single file entry
void printEntry(FLAGS* flags, columnLength maxLengths, struct dirent* direntp, struct stat fileStat, char* filePath)
{
    // Print inode if requested
    if (flags->INDEX == true)
    {
        printf("%*ju ", maxLengths.INODE_LEN, fileStat.st_ino);
    }

    // Print long if requested
    if (flags->LONG == true)
    {
        printLongEntry(maxLengths, fileStat, filePath);

    }
  
    // Print file/path name  
    printf("%s ", getFilenameString(direntp, filePath));
  
    //Check if file is a symlink if so print link location
    if (flags->LONG == true && S_ISLNK(fileStat.st_mode))
    {
        printf("-> %s", getLinkString(filePath));
    }
    printf("\n");
}

void printDirectory(struct dirent **namelist, int NUM_ENTRIES, FLAGS* flags, char* basePath)
{
    if (flags->RECURSIVE || flags->MULTIPLE_DIR)
    {
        printf("\n%s:\n", getFilenameString(NULL, basePath));
    }
    
    char filepath[MAX_PATH_LEN];
    struct stat fileStat;

    // Sections are duplicated because we need all file info widths first
    // before being able to print them all.
    // Get the column lengths we will need to print
    columnLength maxLengths = {1, 1, 1, 1, 1, 1};
    for (int i=0; i < NUM_ENTRIES; i++) 
    {
        // Set the full filepath of dir entry
        memset(filepath, 0, sizeof(filepath));
        strcat(filepath, basePath);
        strcat(filepath, namelist[i]->d_name);

        // Set stats and calculate column lengths
        lstat(filepath, &fileStat);
        getColumnLengths(&maxLengths, namelist[i], fileStat);
    }

    // Iterate through all dir/files in namelist and print
    for (int i=0; i < NUM_ENTRIES; i++)
    {
        // Set the full filepath of dir entry
        memset(filepath, 0, sizeof(filepath));
        strcat(filepath, basePath);
        strcat(filepath, namelist[i]->d_name);

        // Set stats and print
        lstat(filepath, &fileStat);
        printEntry(flags, maxLengths, namelist[i], fileStat, filepath);
    }
}

void printFile(FLAGS* flags, char* filePath)
{
    struct stat fileStat;
    lstat(filePath, &fileStat);
    columnLength maxLength = {1, 1, 1, 1, 1, 1};
    getSingleColumnLength(&maxLength, fileStat, filePath);
    printEntry(flags, maxLength, NULL, fileStat, filePath);
}
