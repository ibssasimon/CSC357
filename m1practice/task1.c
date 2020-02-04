
/*CPE 357 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

int i; 
//char txt[100];
//int sprintf(txt, "%d", i);
//printf(txt);

//printf("%d", 1);
int main() {
  fflush(0);
  if (fork() == 0) {
    fflush(0);
    if (fork() == 0) {
      printf("%s\n", "A");
      return 0;
    } else {   
      wait(&i);
      printf("%s\n", "B");
      return 0;
    }
  } else {
    wait(&i);
    if (fork() == 0) {
      printf("%s\n", "C");
      return 0;
    } else {
      wait(&i);
      printf("%s\n", "D");
    }

  } 
return 0;
}



