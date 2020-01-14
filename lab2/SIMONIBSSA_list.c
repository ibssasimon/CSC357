#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

// Defining listelement struct
typedef struct listelement listelement;
 struct listelement {
  listelement *next, *prev;
  char text[1000];
};

// function declarations
int getInput();
void insertString(char* string);
void printList();
void removeString(int i);
listelement* get(int i);

//Initializing global double linked list
listelement *head = NULL;

int main() {

  int userData;
  char input[1000];
  int rm;
  while((userData = getInput()) != 4) {
    if(userData == 1) {
      printf("insert text\n");
      scanf("%s", input);
      insertString(input);
      memset(input, '\0', sizeof input);
      printf("done push string\n\n");
    } else if(userData == 2) {
      printList();
    } else if(userData == 3) {
      printf("which element do you want to remove?\n");
      scanf("%d", &rm);
      removeString(rm);
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

void insertString(char* string) {
  listelement* newNode = NULL;
  newNode = (listelement*)malloc(sizeof(listelement));
  strcpy(newNode ->text, string);

  if(head == NULL) {
    head = newNode;
    head -> next = NULL;
    head -> prev = NULL;
  } else {
    listelement* p = head;
    while(p != 0 && p-> next != 0) {
      p = p -> next;
    }
    p-> next = newNode;
    newNode -> prev = p;
    newNode -> next = 0;
  }
}

void printList() {
  if(head == NULL) {
    printf("Empty list.\n");
  }
  for(listelement* p = head; p!= 0; p = p->next) {
    printf("%s\n", p->text);
  }
}

void removeString(int i) {
  listelement *p = get(i);
  if(p != NULL) {
    
    if(p == head) {
      head = head -> next;
      free(p);
      if(head != NULL) {
        head -> prev = NULL;
      }
    } else {
      p -> prev -> next = p -> next;
      if(p -> next != NULL) {
        p -> next -> prev = p -> prev;
        free(p);
      }
    }
  } else {
    printf("Cannot remove item.\n");
  }
}


listelement* get(int i) {
  int j;
  if(i == 0) {
    printf("This list does not support that.\n");
    return NULL;
  } else if(i == 1) {
    return head;
  } else if (i > 1 && i > 0) {
    listelement* p = head;
    for(j = 0; j < i-1; j++) {
      p = p -> next;
    }
    return p;
  } else {
    printf("This list does not support that. \n");
    return NULL;
  }
}

