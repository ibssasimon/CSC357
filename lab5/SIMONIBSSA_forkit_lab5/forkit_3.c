#include <stdio.h>

int main() {

  int g;
  fflush(0);

  if(fork() == 0) {
    // child 1st
    wait(&g);
    if(fork() == 0) {
      // child 2nd
      wait(&g);
      printf("5\n");
      return 1;
    } else {
      // parent 2nd
      printf("4\n");
      return 1;
    }
  } else {
    // parent 1st
    if(fork() == 0) {
      // child 3rd
      wait(&g);
      printf("3\n");
      return 1;
    } else {
      if(fork() == 0) {
        // child 3rd
        wait(&g);
        printf("2\n");
        return 1;
      } else {
        printf("1\n");
        return 1;
      }
      return 1;
    }
    return 1;
  }
}