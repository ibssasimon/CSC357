#include <stdio.h>
/* Program prints a tree depending on user input, sample has 56 leaves */


int main() {
    int leaves;
    printf("Enter how many leaves for tree: ");
    scanf("%d", &leaves);

    printf("%d leaves!\n", leaves);
    return 0;
}