#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main() {

  /* Key points:
    - Only O_CREAT and ftruncate() when creating the first program
    - Allocate 2 spaces for arguments, malloc space for first, then copy the name of the second program name into the first space.
    - Clear the second space

    - fork and execv

    - close() filedescriptor, then shm_unlink(name of shared memory);


    - make sure to kill children, wait(0), then return 0;
  
  
   */

  int fd = shm_open("write", O_RDWR | O_CREAT, 0777);
  ftruncate(fd, sizeof(char) * 100);

  // ftruncate size and mmap size must be the same

  char* writeMem = (char*)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  // clear write mem
  memset(writeMem, 0, 100);

  char userInput[100];

  int* childPid = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  char* args[2];
  args[0] = malloc(sizeof(char) * 15);
  args[1] = malloc(sizeof(char) * 15);
  strcpy(args[0], "listen");
  args[1] = 0;




  // fork and call execv
  if(fork() == 0) {
    *childPid = getpid();
    execv(args[0], args);
    return 0;
  }

  while(1) {
    printf("Enter a word: ");
    scanf("%s", userInput);

    if(strcmp(userInput, "q") == 0) {
      break;
    }

    if(writeMem[0] == 0) {
      strcpy(writeMem, userInput);
    } else {
      strcat(writeMem, userInput);
    }

    printf("you wrote: %s", userInput);

  }

  // finish program
  kill(*childPid, SIGKILL);
  wait(0);
  close(fd);
  shm_unlink("write");
  munmap(writeMem, 100);
  return 0;
  
  }