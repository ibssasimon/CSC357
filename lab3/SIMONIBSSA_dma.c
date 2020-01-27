/* Simon Ibssa, CPE 357*/

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
BYTE* mymalloc(unsigned int size);
void myfree(BYTE* myaddres);
void merge(chunkhead* chunk1, chunkhead* chunk2);
void mergeMult(chunkhead* chunk1, chunkhead* chunk2, chunkhead* chunk3);
BYTE* split(chunkhead* chunk, int size);
void analyse();



int main() {
  printf("my dynamic memory allocation assignment\n");
  printf("address of my heap: %x\n", &myheap);
  // defining first chunkhead
  chunkhead *ch = (chunkhead*)myheap;
  ch -> info = 0;
  ch -> size = _1MB - sizeof(chunkhead);
  ch -> next = 0;
  ch -> prev = 0;



  unsigned char* a = mymalloc(1000);
  unsigned char* b = mymalloc(1000);
  unsigned char* c = mymalloc(1000);

  myfree(b);
  analyse();
  printf("REmoveing a\n");
  myfree(a);
  analyse();

  return 0;
}



BYTE* mymalloc(unsigned int size) {
  // get first chunkhead, typecasted
  chunkhead* ch = (chunkhead*)myheap;

  size = (size / PAGESIZE + 1) * PAGESIZE;

  if(size % PAGESIZE != 0) {
    printf("Size must be a multiple of PAGESIZE\n");
    return 0;
  }

  for(;ch != NULL; ch = (chunkhead*) ch->next) {
    if(ch == 0) {
      return 0;
    }

    if(ch -> info == 0 && ch -> size >= size) {
      BYTE* newChunkAddress = split(ch, size);
      return newChunkAddress;
    }

  }
  return 0;
}


void myfree(BYTE* myaddress) {
  // implementation of free function
  chunkhead* ch;
  ch = (chunkhead*)(myaddress - sizeof(chunkhead));

  chunkhead* chunkheadNext = (chunkhead*)ch -> next;
  chunkhead* chunkheadPrev = (chunkhead*)ch -> prev;

  ch -> info = 0;
  
  // preventing segfault by null check

    // ch next is occupied, ch prev is occupied (1)
    if(((chunkhead*)ch -> next) -> info == 1 && ch -> prev &&((chunkhead*)ch -> prev) -> info == 1) {
      return;
    }

    // ch next is occupied, ch prev is free (2)
    if(((chunkhead*)ch -> next) -> info == 1 && ch -> prev &&((chunkhead*)ch -> prev) -> info == 0) {
      
      ((chunkhead*)ch -> prev) -> size += ((chunkhead*)ch) -> size + sizeof(chunkhead);

      chunkhead* potentialNext = ch -> next;

      chunkheadPrev -> next = potentialNext;


      if(potentialNext != NULL ) {
        potentialNext -> prev = chunkheadPrev;
      }
      return;


    }

    // ch next is occupied, ch prev is free (3) inverse of last one
    if(((chunkhead*)ch -> next) -> info == 0 && ch -> prev && ((chunkhead*)ch -> prev) -> info == 1) {

      ((chunkhead*)ch -> next) -> size += ((chunkhead*)ch) -> size + sizeof(chunkhead);

      chunkhead* potentialPrev = ch -> prev;

      chunkheadNext -> prev = potentialPrev;

      if(potentialPrev != NULL) {
        potentialPrev -> next = chunkheadNext;
      }

      return;
    }

    // ch next is free, ch prev is free (4)
    if(((chunkhead*)ch -> next) -> info == 0 && ch -> prev && ((chunkhead*)ch-> prev) -> info == 0) {
      // previous size incremented by next and current size
      ((chunkhead*)ch -> prev) -> size += ((chunkhead*)ch) -> size  + ((chunkhead*)ch -> next) -> size + sizeof(chunkhead);
      // potential next is ch -> next next
      chunkhead* potentialNext = ((chunkhead*)ch -> next) -> next;
      // chunheadPrev -> next = potential -> next
      chunkheadPrev -> next = potentialNext -> next;
      // if potential next not null, potentialNext prev = chunkhead prev
      if(potentialNext!= NULL) {
        potentialNext -> prev = ch -> prev;
      }
      return;
    } 
}

BYTE* split(chunkhead* chunk, int size) {
  // implementation of split function. Should return address of new chunk.
  BYTE* newChunkAddress = ((BYTE*)chunk) + sizeof(chunkhead) + size;
  chunkhead* newChunk = (chunkhead*)newChunkAddress;


  newChunk -> size = (chunk-> size) - sizeof(chunkhead) - size;
  chunk -> size = size;
  chunk -> info = 1;
  chunk -> next = (BYTE*)newChunk;

  newChunk -> info = 0;
  newChunk -> next = 0;
  newChunk -> prev = (BYTE*)chunk;
  chunk -> prev = ((chunkhead*)newChunk -> prev) -> prev;

  return ((BYTE*)chunk) + sizeof(chunkhead);
}

void analyse() {
  int i = 0;
  chunkhead* ch = (chunkhead*)myheap;

    for(;ch != NULL; ch = (chunkhead*) ch->next) {
      i++;
      printf("Chunk #%d\n", i);
      printf("Size = %d bytes\n", ch -> size);

      if(ch -> info == 1) {
        printf("Occupied\n");
      } else {
        printf("Free\n");
      }
      printf("Next = %p\n", (ch -> next));
      printf("Prev = %p\n", (ch -> prev));
      printf("\n");
    }
}