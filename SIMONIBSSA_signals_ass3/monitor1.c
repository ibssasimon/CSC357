#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>


struct stat st;
int main() {

  if(fork() == 0) {
    // child process
    
  } else {
    // parent process
  }
  return 0;
}