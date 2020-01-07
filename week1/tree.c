#include <stdio.h>
#define STEP 2
/* Program prints a tree depending on user input, sample has 56 leaves */
/* Simon Ibssa */

void printBase();
int main() {
    printf("This program creates a christmas tree\n");
    int leaves;
    int i;
    printf("Enter how many leaves for tree: ");
    scanf("%d", &leaves);

    printf("%d leaves!\n", leaves);

    int base = 3;
    leaves -= 3;
    printf("leaves: %d\n", leaves);
    char ast = '*';

    /* Code to print top of tree */
    

    /* Code to print base of tree */
    printBase();
    return 0;
}

void printBase() {
    printf("*\n\n*\n\n*\n");
}