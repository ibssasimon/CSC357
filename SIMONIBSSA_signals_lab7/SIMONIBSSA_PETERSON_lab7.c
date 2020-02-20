#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

/* Simon Ibssa, CSC 357. Lab 7 using PETERSON ALGO*/

int main() {
  char* sentences = (char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);


  // variables for peterson's algo
  int* pr = (int*)mmap(NULL, 2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
  int* flag = (int*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);

  pr[0] = 0;
  pr[1] = 0;
  *flag = 0;

  // fork into child and parent processes

  if(fork() == 0) {
    // child case
    pr[0] = 1;
    *flag = 1;
    // critical section
    while(pr[1] == 1 && *flag == 1);
    strcpy(sentences, "Aye! Aye! Aye! Grrrraaah!! Ge ge ge grraaah!! Panda, Panda Panda, Panda, Panda, Panda, Panda. I got dogs in Atlanta, twisting rope, clean, and the Fanta (soda).\n");
    pr[0] = 0;

  } else {
    // parent case
    char myString[1000];
    pr[1] = 1;
    *flag = 0;
    //critical section
    while(pr[0] == 1 && *flag == 0);
    strcpy(myString, sentences);
    printf("%s", myString);
    pr[1] = 0;

  }
  return 0;
}