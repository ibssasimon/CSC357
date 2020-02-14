#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>


struct stat st;
int main() {
  int g;
  //printf("\033[0;34m"); // set output color to blue
  //printf("stat prog");
  //printf("\033[0m"); //Resets the text to default color
  //printf(" $\n");


  if(fork() == 0) {
    // child process
    char file[100];
    char userInput[100];

    while(1) {
      printf("Enter an option: [filename] [list] or [q]: ");
      scanf("%s", userInput);

      // listing content of current directory
      if(strcmp(userInput, "list") == 0) {
        printf("Listing content of current directory!\n");
      } else if(strcmp(userInput, "q") == 0) {
        return 1;
      } else {
        // assume it's a file name
        printf("\n");


        int result = stat(userInput, &st);

        if(result == -1) {
          printf("Could not open file! Try again\n");
        } else {
          // printing file information
          printf("I-node number: %ld\n", (long)st.st_ino);
          printf("Mode: %lo (octal)\n",(unsigned long)st.st_mode);
          printf("Link count: %ld\n", (long) st.st_nlink);
          printf("Ownership: UID=%ld GID=%ld\n", (long) st.st_uid, (long) st.st_gid);
          printf("Preferred I/O block size: %ld bytes\n", (long) st.st_blksize);
          printf("File size: %lld bytes\n", (long long) st.st_size);
          printf("Blocks allocated: %lld\n", (long long) st.st_blocks);
          printf("Last status change:       %s", ctime(&st.st_ctime));
          printf("Last file access:         %s", ctime(&st.st_atime));
          printf("Last file modification:   %s", ctime(&st.st_mtime));
          printf("\n");
        }

      }



    }


    return 1;
  } else {
    // parent process
    wait(&g);

  }
  return 0;
}