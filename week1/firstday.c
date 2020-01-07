#include <stdio.h>

/* testing to see if I have the correct stuff */

int fct (int y);
int main() {
    int a[10];
    int u = 0;

    char c[10];

    c[5] = '0';
    c[6] = 0;
    printf("%s", c);


    int *p;
    p = &u;
    *p = 10;
    return 0;

}

int fct(int y) {
    return y * 2;
}