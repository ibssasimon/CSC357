/* Program prints a tree depending on user input, sample has 56 leaves */
/* Simon Ibssa */

#include <stdio.h>
#include <string.h>


/* Max length of stars is 19, because we can't exceed 10 lines of asterisks */
#define MAX_STAR_LENGTH 19
#define STEP 2


void printBase();
char* printMyTreeTop(int i, char ast, char* asterisks);
int main() {
    printf("This program creates a christmas tree\n");
    int leaves;
    int i;
    char asterisks[MAX_STAR_LENGTH + 1] = "";
    printf("Enter how many leaves for tree: ");
    scanf("%d", &leaves);

    printf("%d total leaves!\n", leaves);

    int base = 3;
    leaves -= 3;
    printf("top leaves: %d\n\n", leaves);
    char ast = '*';
    int row = 1;

    
    /* Iterate through number of leaves to print asterisks. Represents the top of the tree */
    i = 1;
    while(i <= leaves && row<=10) {
      printf("%*s",20 + (i - row), printMyTreeTop(i, ast, asterisks));
      leaves-= i;
      i+=2;
      row++;
    }


    
    /* Function to print remaining three asterisks for base */
    printBase();
    return 0;
}

void printBase() {
  int i;
  for(i = 0; i < 3; i++) {
    printf("%20s", "*\n");
  }
  return;
}

char* printMyTreeTop(int i, char ast, char* asterisks) {
  int j;

  for(j = 0; j < i; j++) {
    asterisks[j] = '*';
  }
  strcat(asterisks, "\n");
  return asterisks;

}