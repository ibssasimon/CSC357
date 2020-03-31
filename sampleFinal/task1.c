#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>

#define KNRM "\x1B[0m" 
#define KRED "\x1B[31m" 
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG"\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"


// global vars
int fd[2];

void overwrite(int n) {
  dup2(fd[0], STDIN_FILENO);
}

int main() {
  int g;
  int* childPids = (int*)mmap(NULL, sizeof(int) * 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* parentPid = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  char userInput[100];
  pipe(fd);
  int saveSTDIN = dup(STDIN_FILENO);


  signal(SIGUSR1, overwrite);
  
  *parentPid = getpid();
  if(fork() == 0) {
    // child 1
    childPids[0] = getpid();
    while(1) {
      sleep(2);
      // redirect STDIN
      kill(*parentPid, SIGUSR1);
      // write to pipe
      write(fd[1], KRED, strlen(KRED));

    }
    return 0;

  }

  if(fork() == 0) {
    // child 2
    childPids[1] = getpid();

    while(1) {
      sleep(3);
      // redirect STDIN
      kill(*parentPid, SIGUSR1);
      // write to pipe
      write(fd[1], KGRN, strlen(KGRN));
    }
    return 0;
  }

  if(fork() == 0) {
    // child 3
    childPids[2] = getpid();

    while(1) {
      sleep(5);
      // redirect STDIN
      kill(*parentPid, SIGUSR1);
      // write to pipe
      write(fd[1], KMAG, strlen(KMAG));
    }
    return 0;
  }

  if(fork() == 0) {
    // child 4
    childPids[3] = getpid();

    while(1) {
      sleep(7);
      // redirect STDIN
      kill(*parentPid, SIGUSR1);
      // write to pipe
      write(fd[1], KYEL, strlen(KYEL));
    }
    return 0;
  }


  while(1) {
    printf("Please write something: ");
    fflush(0);
    int byteRead = read(0, userInput, 100);

    // add null terminating char
    userInput[byteRead] = 0;

    printf("%s\n", userInput);
    // restoring STDIN every iteration
    dup2(saveSTDIN, STDIN_FILENO);


    // kill children processes, then parent
    if(strncmp(userInput, "q\n",2) == 0) {
      for(int i = 0; i < 4; i++) {
        kill(childPids[i], SIGKILL);
      }
      wait(0);
      return 0;
    }
    userInput[0] = 0;
  }


  return 0; 

}