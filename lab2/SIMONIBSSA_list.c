#include <stdio.h>

// function declarations
int getInput();

// Defining listelement struct
typedef struct listelement listelement;
 struct listelement {
  listelement *next, *prev;
  char text[1000];
};
int main() {
  //Initializing double linked list


  int userData;
  while((userData = getInput()) != 4) {
    if(userData == 1) {
      printf("I'm going to push a string\n");
    } else if(userData == 2) {
      printf("I'm going to print a list\n");
    } else if(userData == 3) {
      printf("I'm going to delete an item\n");
    } else if(userData < 0 || userData > 4) {
      return 0;
    }
  }
  return 0;
}

int getInput() {
  // Function gets user input and determines next step of program
  int input;
  printf("Select:\n");
  printf("1 push string\n");
  printf("2 print list\n");
  printf("3 delete item\n");
  printf("4 end program\n");
  scanf("%d", &input);
  return input;
}