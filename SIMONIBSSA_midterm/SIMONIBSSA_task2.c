#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>


int main() {
  printf("This program prints your full name!\n");

  char *text = mmap(NULL, 300, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int g;
  int i;

  fflush(0);
  if(fork() == 0) {
    // child 1

    if(fork() == 0) {
      // child 2

      strcpy(text, "Simon ");
      return 1;
      
      // print firstname
    } else {
      // print lastname
      wait(&g);
      strcat(text, "Seifu ");
    }

    return 1;
  } else {
    // parent 1
    wait(&g);
    strcat(text, "Ibssa\n");

  }
  
  printf("%s", text);
  


  return 0;
}