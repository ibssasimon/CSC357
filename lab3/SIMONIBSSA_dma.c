#include <stdio.h>
#include <string.h>

// global array declaration - size of 1MB
unsigned char myheap[1048576];

// pagesize definition
#define PAGESIZE 1024

typedef struct chunkhead {
  unsigned int size;
  unsigned int info;
  unsigned char* next;
  unsigned char* prev;

}chunkhead;

// function declarations
unsigned char* ibssaloc(unsigned int size);
void myfree(unsigned char* myaddres);


int main() {
  printf("my dynamic memory allocation assignment\n");
  return 0;
}

unsigned char* ibssaloc(unsigned int size) {
  return NULL;
}


void myfree(unsigned char* myaddress) {

}