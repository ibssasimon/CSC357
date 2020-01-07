#include <stdio.h>
#define STEP 2
/* Program prints a tree depending on user input, sample has 56 leaves */
/* Simon Ibssa */

int main() {
    printf("This program creates a christmas tree\n");
    int leaves;
    printf("Enter how many leaves for tree: ");
    scanf("%d", &leaves);

    printf("%d leaves!\n", leaves);

    int base = 3;
    leaves -= 3;
    printf("leaves: %d\n", leaves);
    char treeleaf = '*';

    /* Code to print top of tree */
    printf("%20c\n", treeleaf);
    printf("%20s\n", "***");


    /* Code to print base of tree */
    return 0;
}

void printBase(int n) {

}