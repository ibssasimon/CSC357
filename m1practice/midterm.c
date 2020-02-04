/* Simon Ibssa, CPE 357-03*
   Christian Eckhardt
   Midterm 1
*/
#include <stdio.h>
#include <sys/mman.h>

int main() {
  // text array
  char *text[10];

  char userInput[1000];

  printf("Enter a word: ");
  scanf("%s", userInput);

  unsigned char* text2 = (unsigned char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int i = 0;

  while(i < 10) {
    text[i] = text2;
    i++;
  }

  return 0;
}
