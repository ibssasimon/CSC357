#include <stdio.h>

int main() {

  int g;
  fflush(0);

  if(fork() == 0) {
    // child 1st

    if(fork() == 0) {
      // child 2nd
      printf("1\n");
      return 1;
    } else {
      // parent 2nd
      wait(&g);
      printf("2\n");
    }
    return 1;
  } else {
    // parent 1st
    wait(&g);
    if(fork() == 0) {
      // child 3rd
      printf("5\n");
      return 1;
    } else {
      // parent 3rd
      wait(&g);
      if(fork() == 0) {
        // child 3rd
        printf("3\n");
        return 1;
      } else {
        wait(&g);
        printf("4\n");
      }
    }
  }
}