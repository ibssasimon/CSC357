#include <stdio.h>
#define STEP 2
/* Program prints a tree depending on user input, sample has 56 leaves */
/* Simon Ibssa */

void printBase();
char* printMyTreeTop(int i, char ast);
int main() {
    printf("This program creates a christmas tree\n");
    int leaves;
    int i;
    printf("Enter how many leaves for tree: ");
    scanf("%d", &leaves);

    printf("%d total leaves!\n", leaves);

    int base = 3;
    leaves -= 3;
    printf("top leaves: %d\n\n", leaves);
    char ast = '*';
    int row = 1;

    
    /* Code to print top of tree */
    i = 1;
    while(i <= leaves && row<=10) {
      printf("%*s",20 - (i-row), printMyTreeTop(i, ast));
      leaves-= i;
      i+=2;
      row++;
    }
    printf("\n");
    /* Code to print base of tree */
    printBase();
    return 0;
}

void printBase() {
  printf("%22s", "*\n");
  printf("%22s", "*\n");
  printf("%22s", "*\n");
  return;

}

char* printMyTreeTop(int i, char ast) {
  char asterisks[i+1];
  int j;
  for(j = 0; j < i; j++) {
    asterisks[j] = '*';
  }
  asterisks[j+1] = '\0';
  printf("%s", asterisks);
  printf("\n");
  return asterisks;
}