#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdbool.h>

/* Simon Ibssa, CSC 357. Lab 7 using DEKKER ALGO*/

int main() {
  char* sentences = (char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);


  // variables for dekker's algo
  bool* wantToEnter = (bool*)mmap(NULL, 2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
  int* turn = (int*)mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
  char* text = (char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  wantToEnter[0] = false;
  wantToEnter[1] = false;
  *turn = 0;

  char sentence1[] = {"Aye! Aye! Aye! Grrrraaah!! Ge ge ge grraaah!! Panda, Panda Panda, Panda, Panda, Panda, Panda."};
  char sentence2[] = {"I got dogs in Atlanta, twisting rope, clean, and the Fanta (soda). Black X6 , Phantom White X6, Panda"};
  char outString[1000];
  int i;
  // fork into child and parent processes
  if(fork() == 0) {
    // child case
   
    /* TODO(sibssa): fix triple print error */
    
    for(i = 0;;i=!i) {
      wantToEnter[0] = true;
      while(wantToEnter[1]) {
        if(*turn != 0) {
          wantToEnter[0] = false;
          while(*turn != 0);
          // busy wait
          wantToEnter[0] = true;
        }
      }

      if(i % 2 == 0) {
        // critical section
        strcpy(text, sentence1);
      } else {
        // critical section
        strcpy(text, sentence2);
      }
      // ending critical section
      *turn = 1;
      wantToEnter[0] = false;
    }
   
    

  } else {
    // parent case
    
    while(1) {
      wantToEnter[1] = true;
      while(wantToEnter[0]) {
        if(*turn != 1) {
          wantToEnter[1] = false;
          while(*turn != 1);
          // busy wait
          wantToEnter[1] = true;

        }
      }
      //critical section
      strcpy(outString, text);
      printf("%s\n", outString);
      // ending critical section
      *turn = 0;
      wantToEnter[1] = false;
    }
  
  }
  return 0;
}