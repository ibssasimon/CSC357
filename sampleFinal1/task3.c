#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>



DIR* dir;
struct dirent* dent;
char fileName[100] =  "";

// function declarations
static void list_dir (const char * dir_name, char* fileName);

int main(int argc, char* argv[]) {

  printf("Welcome to task 3. Let's play\n");
  char userInput[100] = "";

  int flag = 0;
  strcpy(fileName, argv[1]);

  if(argv[2] != NULL) {
    flag = 1;
  }

  if(strncmp(fileName, "q", 1) == 0) {
    printf("quitting prorgam....\n");
    return 0;
  }

  printf("fileName: %s\n", fileName);
  printf("flag: %d\n", flag);

  if(flag == 0) {
    // open current directory
    char buffer[50];
    strcpy(buffer, ".");

    dir = opendir(buffer);

    if(dir != NULL) {
      for(dent = readdir(dir); dent != NULL; dent = readdir(dir)) {

        if(strcmp(dent -> d_name, fileName) == 0) {
          printf("found: %s\n", fileName);
        }
      }
    }

  } else {
    // opening all subdirs
    char buffer[50];
    strcpy(buffer, ".");
    list_dir(buffer, fileName);

  }
  return 0;

}

static void list_dir (const char * dir_name, char* fileName){
    DIR * d;
    /* Open the directory specified by "dir_name". */
    d = opendir (dir_name);

    /* Check it was opened. */
    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    while (1) {
        struct dirent * entry;
        const char * d_name;

        /* "Readdir" gets subsequent entries from "d". */
        entry = readdir (d);
        if (! entry) {
            /* There are no more entries in this directory, so break
               out of the while loop. */
            break;
        }
        d_name = entry->d_name;
        /* Print the name of the file and directory. */
        int count = 0;

        if(strcmp(d_name, fileName) == 0) {
          count ++;
          printf("Found %d instance(s) %s in %s\n", count, fileName, dir_name);
        }

        if (entry->d_type & DT_DIR) {

            /* Check that the directory is not "d" or d's parent. */
          
            if (strcmp (d_name, "..") != 0 &&
                strcmp (d_name, ".") != 0) {
                int path_length;
                char path[PATH_MAX];
                path_length = snprintf (path, PATH_MAX, "%s/%s", dir_name, d_name);
                /* Recursively call "list_dir" with the new path. */
                list_dir (path, fileName);
            }
	        }
    }
}