#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

int main() {
  char* text[10];
  char userIN[100];
  int i;
  // allocate mem
  for(i = 0; i < 10; i++) {
    text[i] = mmap(NULL, sizeof(char) * 100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    text[i][0] = 0;
  }

  while(1) {
    printf("Enter a word: \n");
    scanf("%s", userIN);

    if(strcmp(userIN, "quit") == 0) {
      // quit program

      for(i = 0; i < 10; i++) {
        munmap(text[i], 100);
      }
      break;
    } else if(strcmp(userIN, "print") == 0) {
      printf("These are the elements on the heap: \n");

      for(i = 0; i < 10; i++) {
        if(text[i] != NULL) {
          printf("%s\n", text[i]);
        }
      }
    } else {

      // check for duplicates

      for(i = 0; i < 10; i++) {
        if(strcmp(text[i], userIN) == 0) {
          text[i][0] = 0;
          userIN[0] = '\0';
        }
      }

      // insertion
      for(i = 0; i < 10; i++) {
        if(text[i][0] == 0) {
          strcpy(text[i], userIN);
          userIN[0] = '\0'; 
          break;
        }
      }
    }
  }
  return 0;
}