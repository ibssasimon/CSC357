#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

/* Simon Ibssa, CSC 357. Lab 7 using PETERSON ALGO*/

int main() {
  char* sentences = (char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);


  // variables for peterson's algo
  int* pr = (int*)mmap(NULL, 2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
  int* flag = (int*)mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
  char* text = (char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  pr[0] = 0;
  pr[1] = 0;
  *flag = 0;

  char sentence1[] = {"Aye! Aye! Aye! Grrrraaah!! Ge ge ge grraaah!! Panda, Panda Panda, Panda, Panda, Panda, Panda."};
  char sentence2[] = {"I got dogs in Atlanta, twisting rope, clean, and the Fanta (soda). Black X6 , Phantom White X6, Panda"};
  char outString[1000];
  int i;
  // fork into child and parent processes
  if(fork() == 0) {
    // child case
   

    for(i = 0;;i=!i) {
      *flag = 1;
      pr[0] = 1;
      while(pr[1] == 1 && *flag == 1);

      if(i % 2 == 0) {
        // critical section
        strcpy(text, sentence1);
      } else {
        strcpy(text, sentence2);
      }
      // ending critical section
      pr[0] = 0;
    }
   
    

  } else {
    // parent case
    
    while(1) {
      pr[1] = 1;
      *flag = 0;
      while(pr[0] == 1 && *flag == 0);
      //critical section
      strcpy(outString, text);
      printf("%s\n", outString);
      // ending critical section
      pr[1] = 0;
    }
  
  }
  return 0;
}