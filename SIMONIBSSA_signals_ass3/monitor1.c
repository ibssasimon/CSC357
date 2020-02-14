#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>


struct stat st;
int main() {
  int g;
  printf("\033[0;34m"); // set output color to blue
  printf("stat prog");
  printf("\033[0m"); //Resets the text to default color
  printf(" $\n");


  if(fork() == 0) {
    // child process
    int result = stat("test.txt", &st);

    if(result == -1) {
      printf("Could not open file!\n");
    }

    printf("%d\n", st.st_mode);
    printf("%d\n", st.st_size);



    return 1;
  } else {
    // parent process
    wait(&g);

  }
  return 0;
}