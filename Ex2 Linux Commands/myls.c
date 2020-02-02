#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> //for modified time in ls -l
#include <dirent.h>   //for directory struct
#include <time.h>     //for struct tm
#include <pwd.h>      //for getpwuid

#define RESET_COLOR "\e[m" //color macros for printing with color
#define MAKE_GREEN "\e[32m"
#define MAKE_BLUE "\e[36m"

void simpleList(char *dirname);                   //ls
void longList(char *dirname);                     //ls -l
void recursiveList(char *dirname, char *relpath); //ls -R
void permissionsPrint(mode_t file);               //printing file permissions

int main(int argc, char *argv[])
{
    if (argc > 3)
        printf("\nEnter less than 3 arguments\n.");

    else if (argc == 1)
        simpleList(".");

    else if (argc == 2)
        simpleList(argv[1]);

    else
    {
        if (strcmp(argv[1], "-l") == 0)
        {
            longList(argv[2]);
        }
        else if (strcmp(argv[1], "-R") == 0)
            recursiveList(argv[2], argv[2]);
    }

    return 0;
}

void simpleList(char *dirname)
{
    struct dirent *de;
    DIR *dr = opendir(dirname);

    if (dr == NULL)
    {
        printf("\nCould not find the specified directory.\n");
        return;
    }

    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_type == DT_DIR)
            printf(MAKE_BLUE "\n%s\\" RESET_COLOR, de->d_name); //printing directories in blue
        else
            printf(MAKE_GREEN "\n%s" RESET_COLOR, de->d_name); //printing other files in green
    }
    printf("\n\nDirectories indicated with a '\\' at the end.\n");
    closedir(dr);
}

void longList(char *dirname)
{
    struct dirent *de;
    struct stat ds;
    struct tm modtime;
    struct passwd *pwd;

    DIR *dr = opendir(dirname);

    if (dr == NULL)
    {
        printf("\nCould not find the specified directory.\n");
        return;
    }

    while ((de = readdir(dr)) != NULL)
    {

        char dirbuf[1024];                                              //to store the absolute pathname from the current directory, as the stat() works properly only on such pathnames
        snprintf(dirbuf, sizeof(dirbuf), "%s/%s", dirname, de->d_name); //using snprintf() to store it in the proper format

        stat(dirbuf, &ds); //to get information about file

        time_t t = ds.st_mtime;    //getting modified time of file
        localtime_r(&t, &modtime); //converting time_t to struct tm which has %c, which is the appropriate local based time and date format
        char timebuf[80];
        strftime(timebuf, sizeof(timebuf), "%c", &modtime); //storing the %c value in a string for printing using strftime()

        pwd = getpwuid(ds.st_uid); //getting username of the user id of file

        printf("\n");
        permissionsPrint(ds.st_mode); //to print permissions
        printf("\t%ld\t%ld\t%s\t%ld\t%s\t%s", de->d_ino, ds.st_nlink, pwd->pw_name, ds.st_size, timebuf, de->d_name);
    }
    printf("\n");
}
void recursiveList(char *dirname, char *relpath)
{

    struct dirent *de;
    char *temppath;
    temppath = (char *)malloc(sizeof(char) * 10000);
    DIR *dr = opendir(dirname);

    if (dr == NULL)
    {
        printf("\nCould not find the specified directory.\n");
        return;
    }

    while ((de = readdir(dr)) != NULL)
    {
        if (de->d_name[0] != '.') //excluding hidden directories and parent, current directories.
        {
            if (de->d_type == DT_DIR)
            {
                printf(MAKE_BLUE "\n%s:" RESET_COLOR, de->d_name); //printing directories in blue
                temppath = strcpy(temppath, "");                   //creating the path to the sub-folder, using existing relative path
                temppath = strcat(temppath, relpath);
                temppath = strcat(temppath, "/");
                temppath = strcat(temppath, de->d_name);
                printf("\n%s", temppath);
                recursiveList(temppath, temppath);
            }
            else
                printf(MAKE_GREEN "\n%s" RESET_COLOR, de->d_name); //printing other files in green
        }
    }
    printf("\n");
    closedir(dr);
}

void permissionsPrint(mode_t file)
{
    printf((S_ISDIR(file)) ? "d" : "-");
    printf((file & S_IRUSR) ? "r" : "-");
    printf((file & S_IWUSR) ? "w" : "-");
    printf((file & S_IXUSR) ? "x" : "-");
    printf((file & S_IRGRP) ? "r" : "-");
    printf((file & S_IWGRP) ? "w" : "-");
    printf((file & S_IXGRP) ? "x" : "-");
    printf((file & S_IROTH) ? "r" : "-");
    printf((file & S_IWOTH) ? "w" : "-");
    printf((file & S_IXOTH) ? "x" : "-");
}

/*
OUTPUT:

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ gcc myls.c -o myls
vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ ./myls

new.txt
mygrep.c
mygrep
mycp.c
..\
.vscode\
.\
sample.txt
mycp
ss
myls.c
myls
mycp2.c
content.txt

Directories indicated with a '\' at the end.

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ ./myls -l "../../.."

drwxr-xr-x      1317955 7       vishakan        4096    Mon Nov 25 22:20:06 2019        Python
drwxr-xr-x      1179650 49      vishakan        4096    Mon Dec 30 17:13:20 2019        ..
drwxr-xr-x      1705275 5       vishakan        4096    Sat Dec 28 20:32:20 2019        Semester IV
drwxr-xr-x      1179740 5       vishakan        4096    Sat Dec 28 20:31:54 2019        .
drwxr-xr-x      1312099 9       vishakan        4096    Sat Dec 28 20:30:46 2019        Semester III

vishakan@legion:~/Desktop/Semester IV/OS Lab/Ex1$ ./myls -R ../..

DBMS Lab:
../../DBMS Lab

OS Lab:
../../OS Lab
Ex1:
../../OS Lab/Ex1
new.txt
mygrep.c
mygrep
mycp.c
sample.txt
mycp
ss
myls.c
myls
mycp2.c
content.txt


DAA Lab:
../../DAA Lab

*/