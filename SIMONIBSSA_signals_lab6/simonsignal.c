#include <stdio.h>
#include <unistd.h>
#include <time.h>
int main() {
  printf("Main program ID: %d\n", getpid());
  printf("---BEGIN FORKING----\n");
  fflush(0);
  int g;
  if(fork() == 0) {
    // child process
    printf("I am the child, my ID is: %d\n", getpid());
    return 1;
  } else {
    wait(&g);
    printf("I am the parent, my ID is: %d\n", getpid());
  }
  return 0;
}