#include <stdio.h>

int main() {
  // program prints 1 2 3 4 in order according to forking process

  int g;

  fflush(0);
  if(fork() == 0) {
    // child process 1st fork
    printf("1\n");
    return 1;
  } else {
    // parent process 1st fork
    wait(&g);
    printf("2\n");
    if(fork() == 0) {
      // child 2nd fork
      printf("3\n");
      return 1;
    } else {
      wait(&g);
      printf("4\n");
      // parent 2nd fork
    }
  }
}