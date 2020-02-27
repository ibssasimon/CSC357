#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
  printf("find stuff program\n");


  if(fork() == 0) {
    // child case
    /* 
      CHILD should search, report and end
    */
  } else {
    // parent case

    /*
      Parent should do the scanf. COPY ASGN 3 SCANF CODE HERE
     */
  }
  return 0;
}