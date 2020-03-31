#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>


int fd[2];
void handleAlarm(int n) {
  dup2(fd[0], STDIN_FILENO);
}

int main() {
  pipe(fd);
  char userBuffer[100];
  int* childPID1 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* childPID2 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* childPID3 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* childPID4 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* parentPID = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  int saveSTDIN = dup(STDIN_FILENO);

  *parentPID = getpid();

  signal(SIGUSR1, handleAlarm);


  if(fork() == 0) {
    *childPID1 = getpid();
    int i = 0;
    char myInt[5];
    close(fd[0]);
    while(1) {      
      sleep(5);
      i++;
      sprintf(myInt, "%d", i);
      char resultString[100] = "";
      strcat(resultString, "5 second interval passed");
      strcat(resultString, i);
      strcat(resultString, " times");

      write(fd[1], resultString, strlen(resultString));
      kill(*parentPID, SIGUSR1);
    }

  }
  if(fork() == 2) {
    *childPID2 = getpid();
    int i = 0;
    char myInt[5];
    close(fd[0]);
    while(1) {
      sleep(10);
      i++;
      sprintf(myInt, "%d", i);
      char resultString[100] = "";
      strcat(resultString, "10 second interval passed ");
      strcat(resultString, i);
      strcat(resultString, "times");
      write(fd[1], resultString, strlen(resultString));
      kill(*parentPID, SIGUSR1);
    }
  }

  if(fork() == 0) {
    *childPID3 = getpid();
    int i = 0;
    char myInt[5];
    close(fd[0]);
    while(1) {
      sleep(20);
      i++;
      sprintf(myInt, "%d", i);
      char resultString[100] = "";
      strcat(resultString, "10 second interval passed ");
      strcat(resultString, i);
      strcat(resultString, "times");
      write(fd[1], resultString, strlen(resultString));
      kill(*parentPID, SIGUSR1);
    }
  }

  if(fork() == 0) {
    *childPID4 = getpid();
    int i = 0;
    char myInt[5];
    close(fd[0]);
    while(1) {
      sleep(30);
      i++;
      sprintf(myInt, "%d", i);
      char resultString[100] = "";
      strcat(resultString, "10 second interval passed ");
      strcat(resultString, i);
      strcat(resultString, "times");
      write(fd[1], resultString, strlen(resultString));
      kill(*parentPID, SIGUSR1);
    }
  }


  while(1) {
    printf("Type something on screen: ");
    fflush(0);
    dup2(saveSTDIN, STDIN_FILENO);
    read(STDIN_FILENO, userBuffer, 100);
    printf("%s\n", userBuffer);
    fflush(0);

    if(strncmp(userBuffer,"q",1)==0) {
      kill(*childPID1, SIGKILL);
      kill(*childPID2, SIGKILL);
      kill(*childPID3, SIGKILL);
      kill(*childPID4, SIGKILL);

      wait(0);
      return 0;
    }
    userBuffer[0] = 0;

  }
  return 0;
}