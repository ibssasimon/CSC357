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


int main(int argc, char* argv[]) {
  // Task 4



  int* sharedMem;
  int fd;


  srand(time(0));

  if(atoi(argv[1]) == 0) {
    fd = shm_open("shared", O_RDWR | O_CREAT, 0777);
    ftruncate(fd, sizeof(int));

    sharedMem = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    *sharedMem = getpid();
    int i = 0;
    while(1) {
      int randNum = rand() % 100;
      sleep(1);
      printf("%d\n", randNum);
    }

  } else {
    fd = shm_open("shared", O_RDWR, 0777);
    sharedMem = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    kill(*sharedMem, SIGKILL);    
    close(fd);
    shm_unlink("shared");
    return 0;
  }


}