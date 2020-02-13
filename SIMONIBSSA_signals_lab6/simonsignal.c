// Simon Ibssa, CSC 357

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <dirent.h>
#include <signal.h>


// function declarations
void overwriteSignal(int n);

int main() {

  // overwriting signals
  signal(SIGINT, overwriteSignal);
  signal(SIGTSTP, overwriteSignal);
  signal(SIGTERM, overwriteSignal);
  signal(SIGKILL, overwriteSignal);


  struct dirent* dent;
  DIR* dir;

  printf("Nearly indestructible child program! Muahahahaha!\n");
  printf("Main program ID: %d\n", getpid());
  int g;
  fflush(0);
  char buffer[50];




  while(1) {

    if(fork() == 0) {
      signal(SIGINT, overwriteSignal);
      signal(SIGTSTP, overwriteSignal);
      signal(SIGTERM, overwriteSignal);
      signal(SIGKILL, overwriteSignal);
      fflush(0);
      // child process

      

      printf("child ID: %d\n", getpid());
      while(1) {
        strcpy(buffer, ".");
        dir = opendir(buffer);

        if(dir != NULL) {
          for(dent = readdir(dir); dent != NULL; dent = readdir(dir)) {
            printf("%s", dent -> d_name);

            if(dent -> d_type == DT_DIR) {
              printf(" - is a directory");
            }
            printf("\n");
          }
        }
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("\n");
        printf("the time is %02d:%02d:%02d\n", tm.tm_hour % 12, tm.tm_min, tm.tm_sec);
        sleep(10);
      }
      return 1;
    } else {
      wait(0); 
      // return back to loop
    }
  }
  
  return 0;
}

// helper functions
void overwriteSignal(int n) {
  printf("NOPE\n");
}