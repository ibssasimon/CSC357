#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m" 
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"


// step 1 of pipe
int fd[2];

// function overwrites reading to behave like STDIN
void overwrite(int n) {
  dup2(fd[0], STDIN_FILENO);
}


/* KEY TAKEAWAYS:
  - Make sure when you overwrite, you overwrite STDOUT aka dup2(fd[0])

  - Understand the syntax to get standard time timt_t now; time(&now); printf("ctime(&now)");

  - Understand why you have to send process three times / flag for something existing in pipe
 */
int main() {

  char userInput[100] = "";

  // step 2 of pipe
  pipe(fd);
  int saveSTDIN = dup(STDIN_FILENO);
  // catching SIGUSR1 to trigger overwrite function
  signal(SIGUSR1, overwrite);

  int* parentPID = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* child1PID = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* child2PID = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* flag = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  *flag = 0;
  *parentPID = getpid();



  // child 1
  if(fork() == 0) {
    *child1PID = getpid();
    while(1) {
      sleep(5);
      time_t now;
      time(&now);
      *flag = 1;
      // sending overwrite signal to parent, then write to pipe
      kill(*parentPID, SIGUSR1);
      // write red color to pipe
      write(fd[1], KRED, strlen(KRED));
      kill(*parentPID, SIGUSR1);
      // write current time in pipe
      write(fd[1], ctime(&now), strlen(ctime(&now)));
      kill(*parentPID, SIGUSR1);
      *flag = 0;
      write(fd[1], KNRM, strlen(KNRM));
    }

    return 0;
  }

  // child 2
  if(fork() == 0) {
    *child2PID = getpid();
    while(1) {
      sleep(11);

      time_t now;
      time(&now);
      // flag if we  write to something in pipe. Only restore STDIN if flag is 0
      *flag = 1;
      kill(*parentPID, SIGUSR1);
      write(fd[1], KCYN, strlen(KCYN));
      kill(*parentPID, SIGUSR1);
      write(fd[1], ctime(&now), strlen(ctime(&now)));
      kill(*parentPID, SIGUSR1);
      *flag = 0;
      write(fd[1], KNRM, strlen(KNRM));
    }

    return 0;

  }



  // PARENT - reads from keyboard
  while(1) {
    if(*flag == 0) {
      fflush(0);
      printf("Type something in: ");
      fflush(0);
      // restore stdin
      dup2(saveSTDIN, STDIN_FILENO);
    }

    fflush(0);
    int bytesRead = read(STDIN_FILENO, userInput, 100);
    fflush(0);
    // add null terminator
    userInput[bytesRead] = 0;

    if(strncmp(userInput, "q\n", 2) == 0) {
      kill(*child1PID, SIGKILL);
      kill(*child2PID, SIGKILL);
      wait(0);
      return 0;
    }

    printf("%s\n", userInput);
    fflush(0);

    userInput[0] = 0;

  }

  return 0;
}