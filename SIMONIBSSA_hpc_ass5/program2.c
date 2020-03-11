#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


int main(int argc, char* argv[]) {
  printf("program 2, last 357 assignment... yippee!\n");

  // args* definition
  char* args[4];

  // mallocing space for each command line arg
  args[0] = malloc(100);
  args[1] = malloc(100);
  args[2] = malloc(100);

  // copying command line arguments into args
  strcpy(args[0], argv[1]);
  strcpy(args[2], argv[2]);
  args[3] = 0;


  // for loop with fork, sprintf, and execv call
  for(int i = 0; i < atoi(argv[2]); i++) {
    if(fork() == 0) {
      sprintf(args[1], "%d", i);
      execv(argv[1], args);
      printf("not successful\n");
      return 0;
    }
  }
  wait(0);
  return 0;
}