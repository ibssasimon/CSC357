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

int main() {

  // second program doesn't need to O_CREAT and ftruncate();
  int fd = shm_open("write",O_RDWR,0777);

  char* writeMem = (char*)mmap(NULL, sizeof(char) * 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  while(1) {

    sleep(2);

    if(writeMem[0] != 0) {
      printf("%s was in memory\n", writeMem);
      memset(writeMem, 0, 100);
    }
  }

  
  close(fd);
  shm_unlink("write");
  return 0;
}