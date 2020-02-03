/* Simon Ibssa, CPE 357*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


// pagesize definition
#define PAGESIZE 4096
#define _1MB 1048576 


// global array declaration heapsize which is initialized to zero
int heapsize = 0;

unsigned char myheap[_1MB];
void* top = NULL;

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
void analyze();



int main() {
  printf("Program 2\n");
  // defining first chunkhead
  // if heapsize is zero. SPECIAL case: allocate chunkhead + size
  // brk(size + chunkhead)

  printf("%p\n", top);

  unsigned char* a = mymalloc(4000);
  unsigned char* b = mymalloc(1024);
  analyze();
  return 0;
  unsigned char* c = mymalloc(1000);
  unsigned char* d = mymalloc(3000);
  analyze();
  myfree(c);
  analyze();
  c = mymalloc(1000);
  analyze();
  b[0] = b[1023] = 0;
  myfree(b);
  myfree(d);
  analyze();
  myfree(c);
  analyze();

  return 0;
}



BYTE* mymalloc(unsigned int size) {
  // use sbrk to move program break
  // set to a variable so you can return that value
  // if top is null crearte first chunk

  size = (size / PAGESIZE + 1) * PAGESIZE;

  if(size % PAGESIZE != 0) {
    printf("Size must be a multiple of PAGESIZE\n");
    return 0;
  }

  

  // allocating first chunk
  if(top == NULL) {
    top = sbrk(size + sizeof(chunkhead));
    heapsize += size + sizeof(chunkhead);
    chunkhead* current = top;
    current -> size = size;
    current -> info = 1;
    current -> prev = NULL;
    return current;
  } else {
    chunkhead* current = top;
    for(;current != NULL; current = (chunkhead*) current->next) {
      if(current == 0) {
        return 0;
      }

      if(current -> info == 0 && current -> size > size) {
        BYTE* newChunkAddress = split(current, size);
        return newChunkAddress;
      } else if(current -> info == 0 && current -> size == size){
          current-> info = 1;
        return (BYTE*)current + sizeof(chunkhead);
      }

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

  int isPrevOccupied = 0;
  int isNextOccupied = 0;
  int isPrevFree = 0;
  int isNextFree = 0;

  if (chunkheadNext) {
    if (chunkheadNext->info == 1) {
      isNextOccupied = 1;
    } else {
      isNextFree = 1;
    }
  } else {
    isNextOccupied = 1;
  }

  if (chunkheadPrev) {
    if (chunkheadPrev->info == 1) {
      isPrevOccupied = 1;
    } else {
      isPrevFree = 1;
    }
  } else {
    isPrevOccupied = 1;
  }



  ch -> info = 0;
  
    // ch next is occupied, ch prev is occupied (1)
    if(isNextFree == 0 && isPrevOccupied == 1) {
      return;
    }

    // ch next is occupied, ch prev is free (2)
    if(isNextOccupied == 1 && isPrevFree == 1) {
      
      if(ch -> prev != NULL) {
        ((chunkhead*)ch -> prev) -> size += ((chunkhead*)ch) -> size + sizeof(chunkhead);

        chunkhead* potentialNext = ch -> next;

        chunkheadPrev -> next = potentialNext;


        if(potentialNext != NULL ) {
          potentialNext -> prev = chunkheadPrev;
        }
      }
      return;
    }

    // ch next is free, ch prev is occupied (3)
    if(isNextFree == 1 && isPrevFree == 0) {

      ((chunkhead*)ch) -> size += ((chunkhead*)ch -> next) -> size + sizeof(chunkhead);

      chunkhead* potentialNext = ((chunkhead*)ch -> next) -> next;
      
      ch -> next = potentialNext;

      if(potentialNext != NULL) {
        potentialNext -> prev = ch;
      }
      return;
    }

    // ch next is free, ch prev is free (4)
    if(isNextFree == 1 && isPrevOccupied == 0) {
        // previous size incremented by next and current size
      ((chunkhead*)ch -> prev) -> size += ((chunkhead*)ch) -> size  + ((chunkhead*)ch -> next) -> size + sizeof(chunkhead);
      // potential next is ch -> next next
      chunkhead* potentialNext = ((chunkhead*)ch -> next) -> next;
      // chunheadPrev -> next = potential -> next

      chunkheadPrev -> next = potentialNext;
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

void analyze() {

  printf("-----PRINTING HEAP NOW ------------\n");
  int i = 0;
  chunkhead* ch = top;

    for(;ch != NULL; ch = (chunkhead*) ch->next) {
      i++;
      printf("Chunk #%d\n", i);
      printf("Size = %d bytes\n", ch -> size);

      if(ch -> info == 1) {
        printf("Occupied\n");
      } else {
        printf("Free\n");
      }
      printf("Current = %p\n", ch);
      printf("Next = %p\n", (ch -> next));
      printf("Prev = %p\n", (ch -> prev));
      printf("\n");
    }
}