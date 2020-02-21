#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdbool.h>

/* Simon Ibssa, CSC 357. Lab 7 using PETERSON ALGO*/

int main() {
  char* sentences = (char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);


  // variables for peterson's algo
  bool* wantToEnter = (bool*)mmap(NULL, 2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
  int* turn = (int*)mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
  char* text = (char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  wantToEnter[0] = false;
  wantToEnter[1] = false;
  *turn = 0;

  char sentence1[] = {"Aye! Aye! Aye! Grrrraaah!! Ge ge ge grraaah!! Panda, Panda Panda, Panda, Panda, Panda, Panda."};
  char sentence2[] = {"I got dogs in Atlanta, twisting rope, clean, and the Fanta (soda). Brah brah brah"};
  char outString[1000];
  int i;
  // fork into child and parent processes
  if(fork() == 0) {
    // child case
   
    /* TODO(sibssa): implement p0 for dekker */
    
    for(i = 0;;i=!i) {
      *turn = 1;
      wantToEnter[0] = 1;
      while(wantToEnter[1] == 1 && *turn == 1);

      if(i % 2 == 0) {
        // critical section
        strcpy(text, sentence1);
      } else {
        strcpy(text, sentence2);
      }
      // ending critical section
      wantToEnter[0] = 0;
    }
   
    

  } else {
    // parent case
    
    while(1) {
      wantToEnter[1] = 1;
      *turn = 0;
      while(wantToEnter[0] == 1 && *turn == 0);
      //critical section
      strcpy(outString, text);
      printf("%s\n", outString);
      // ending critical section
      wantToEnter[1] = 0;
    }
  
  }
  return 0;
}