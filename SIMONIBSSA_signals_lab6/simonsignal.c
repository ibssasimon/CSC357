#include <stdio.h>
#include <unistd.h>
#include <time.h>
int main() {
  printf("Nearly indestructable child program! Muahahahaha!\n");
  printf("Main program ID: %d\n", getpid());
  int g;
  fflush(0);

  
  if(fork() == 0) {
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
    wait(&g);
    printf("parent ID: %d\n", getpid());
  }
  return 0;
}