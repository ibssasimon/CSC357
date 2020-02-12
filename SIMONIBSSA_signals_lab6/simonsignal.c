// Simon Ibssa, CSC 357

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <signal.h>


// function declarations
void overwriteSignal(int n);

int main() {

  // overwriting signals
  signal(SIGINT, overwriteSignal);
  signal(SIGTSTP, overwriteSignal);



  printf("Nearly indestructable child program! Muahahahaha!\n");
  printf("Main program ID: %d\n", getpid());
  int g;
  fflush(0);

  if(fork() == 0) {
    fflush(0);
    // child process
    printf("child ID: %d\n", getpid());
    while(1) {
      time_t t = time(NULL);
      struct tm tm = *localtime(&t);
      printf("the time is %02d:%02d:%02d\n", tm.tm_hour % 12, tm.tm_min, tm.tm_sec);
      sleep(5);
    }
    return 1;
  } else {
    if(wait(&g) != NULL) {
      if(fork() == 0) {
        fflush(0);
        while(1) {
          time_t t = time(NULL);
          struct tm tm = *localtime(&t);
          printf("the time is %02d:%02d:%02d\n", tm.tm_hour % 12, tm.tm_min, tm.tm_sec);
          sleep(5);
        }
        return 1;
      } else {
        wait(&g);
      }
    }

    printf("parent ID: %d\n", getpid());
  }
  return 0;
}

// helper functions
void overwriteSignal(int n) {
  printf("NOPE\n");
}