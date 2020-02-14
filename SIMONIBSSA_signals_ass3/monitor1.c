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
          printf("file mode: %d\n", st.st_mode);
          printf("file size: %lld\n", st.st_size);
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