#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <limits.h>
#include <dirent.h>
#include <time.h>


int fd[2];

int override = 0;

void overwrite(int n) {
  dup2(fd[0], STDIN_FILENO);
  override = 1;
}
int main() {
  // Task 3
  srand(time(0));
  pipe(fd);

  signal(SIGUSR1, overwrite);

  int bytesRead;
  int saveSTDIN = dup(STDIN_FILENO);

  int* parentPid = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* childPid = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  char* numbers = (char*)mmap(NULL, sizeof(char) * 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  char* correct = (char*)mmap(NULL, sizeof(char) * 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  char* resultString = (char*)mmap(NULL, sizeof(char) * 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* i = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


  *parentPid = getpid();

  char userNum[10] = "";

  *i = 20;
  if(fork() == 0) {
    // child 1
    *childPid = getpid();

    while(1) {
      sleep(5);
      *i -= 5;
      if(*i <= 0) {
        char temp[50] = "";
        strcat(temp, "\ntook too long\n");
        strcpy(resultString, temp);
        write(fd[1], resultString, strlen(resultString));
        kill(*parentPid, SIGUSR1);
        kill(*parentPid, SIGKILL);
        return 0;
      }
      char temp[50] = "";

      sprintf(temp, "\n%d", *i);
      strcat(temp, " seconds left\n");
      strcpy(resultString, temp);
      write(fd[1], resultString, strlen(resultString));
      kill(*parentPid, SIGUSR1);

      memset(temp, 0, 50);

    }
    


  }


  // parent case

  for(int i = 0; i < 4; i++) {
    char onedigit = rand() %4 + 48;
    numbers[i] = onedigit;
    correct[i] = 0;
  }


  printf("answers: %c", numbers[0]);
  printf("%c", numbers[1]);
  printf("%c", numbers[2]);
  printf("%c\n", numbers[3]);


  while(1) {

    // read from keyboard

    // restore stdin
    dup2(saveSTDIN, STDIN_FILENO);
    int sum = 0;
    for(int i = 0; i < 4; i++) {
      sum += correct[i];
    }

    if(sum ==  4) {
      // exit program
      printf("Congratulations, you won!\n");
      kill(*childPid, SIGKILL);
      wait(0);
      return 0;
    }


    printf("Enter 4 integers: ");
    fflush(0);

    if(override == 1) {
      bytesRead = read(0, userNum, 20);
      userNum[bytesRead] = 0;
    } else {
      bytesRead = read(0, userNum, 4);
      userNum[4] = 0;
    }

    printf("you typed: %s\n", userNum);

    fflush(0);

    // remove spaces from userinput
    for(int i = 0; i < strlen(userNum); i++) {
      if(userNum[i] == ' ') {
        userNum[i] = '\0';
      }
    }

    for(int i = 0; i < 4; i++) {
      if(numbers[i]  == userNum[i]) {
        printf("Index %d number is correct\n", i);
        correct[i] = 1;
      }
    }

  }


}