#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <limits.h>
#include <dirent.h>

DIR* dir;
struct dirent* dent;


int main(int argc, char* argv[]) {
  // Task 2

  char directory[PATH_MAX];

  char fileName[100] = "";
  char subDirectory[100] = "";
  int found = 0;

  if(argv[1] != NULL) {
    strcpy(fileName, argv[1]);
  }

  if(argv[2] != NULL) {
    strcpy(subDirectory, argv[2]);
  }

  printf("filename: %s\n", fileName);
  printf("subdir: %s\n", subDirectory);

  printf("Searching....\n");

  
  getcwd(directory, (PATH_MAX));


  if(strcmp(subDirectory, "") == 0) {
    char buffer[50];
    strcpy(buffer, ".");
    dir = opendir(buffer);

    if(dir != NULL) {
      for(dent = readdir(dir); dent != NULL; dent = readdir(dir)) {
        if(strcmp(dent -> d_name, fileName) == 0) {
          found = 1;
          printf("Found: %s\n", fileName);
          printf("Absolute path: %s\n", directory);

        }
      }
    }
  } else {
    // subdir was passed in
    char tempBuffer[50] = "";
    strcat(tempBuffer,"/");
    strcat(tempBuffer, subDirectory);

    // strcat to directory
    strcat(directory, tempBuffer);

    dir = opendir(directory);
    if(dir == NULL) {
      printf("Folder name not present\n");
      return 0;
    }

    for(dent = readdir(dir); dent != NULL; dent = readdir(dir)) {
      if(strcmp(dent -> d_name, fileName) == 0) {
        found = 1;
        printf("Found: %s\n", fileName);
        printf("Absolute path: %s\n", directory);

      }
    }
  }



  if(found == 0) {
    printf("Couldn't find file :(\n");
    return 0;
  }




}