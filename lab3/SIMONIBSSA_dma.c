#include <stdio.h>
#include <string.h>

// pagesize definition
#define PAGESIZE 1024
#define _1MB 1048576 


// global array declaration - size of 1MB
unsigned char myheap[_1MB];

typedef unsigned char BYTE;

typedef struct chunkhead {
  unsigned int size;
  unsigned int info;
  BYTE* next;
  BYTE* prev;

}chunkhead;

// function declarations
BYTE* ibssaloc(unsigned int size);
void myfree(BYTE* myaddres);
void merge(BYTE* chunk1, BYTE* chunk2);
BYTE* split(chunkhead* chunk, int size);


int main() {
  printf("my dynamic memory allocation assignment\n");
  printf("address of my heap: %x\n", &myheap);
  // defining first chunkhead
  chunkhead *ch = (chunkhead*)myheap;
  ch -> info = 0;
  ch -> size = _1MB - sizeof(chunkhead);
  ch -> next = 0;
  ch -> prev = 0;

  ibssaloc(1024);
  ibssaloc(2048);


  return 0;
}



BYTE* ibssaloc(unsigned int size) {
  // get first chunkhead, typecasted
  chunkhead* ch = (chunkhead*)myheap;

  if(size % PAGESIZE != 0) {
    printf("Size must be a multiple of PAGESIZE\n");
    return 0;
  }

  if(ch -> info == 1 || ch -> size < size) {
    ch = (chunkhead*)ch -> next;
  } else {
    BYTE* newChunkAddress = split(ch, size);
    return newChunkAddress;

  }

  if(ch == 0) {
    return 0;
  }

  // check ch-> size with size
  // check ch -> info with 0 or 1
      // if false, go to next chunkhead

      // if true, set ch to new values
      // split chunkhead into mem value
      // link chunks
      // return ((BYTE*)chunkhead) + 16;

  

  // iterate through all chunkheads checking size and info

  return 0;
}


void myfree(BYTE* myaddress) {
  // implementation of free function

}

void merge(BYTE* chunk1, BYTE* chunk2) {
  // implentation of merge function
}

BYTE* split(chunkhead* chunk, int size) {
  // implementation of split function. Should return address of new chunk.
  chunkhead* newChunk = ((BYTE*)chunk) + size;
  chunk -> size = size;
  chunk -> info = 1;
  chunk -> prev = 0;
  chunk -> next = (BYTE*)newChunk;

  newChunk -> info = 0;
  newChunk -> size = _1MB - sizeof(chunkhead) - size;
  newChunk -> next = 0;
  newChunk -> prev = chunk;

  return (BYTE*)newChunk;
}