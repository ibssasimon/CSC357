/* Simon Ibssa, CPE 357*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


// pagesize definition
#define PAGESIZE 4096


// global array declaration heapsize which is initialized to zero
int heapsize = 0;

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
BYTE* split(chunkhead* chunk, int size);
chunkhead* getLastChunk();
void analyze();



int main() {
  BYTE*a[100];
  int i;
  analyze();//50% points
  for(i=0;i<100;i++) {
    a[i]= mymalloc(1000);
  }
  for(i=0;i<90;i++) {
    myfree(a[i]);
  }
  analyze(); //50% of points if this is correct
  myfree(a[95]);
  a[95] = mymalloc(1000);
  analyze();//25% points, this new chunk should fill the smaller free one
  //(best fit)

  // WE HAVE 11 CHUNKS BUT ONLY DEALLOCATE 10! That last chunk contains all free space from previous merges
  for(i=90;i<100;i++) {
    myfree(a[i]);
  }
  analyze();// 25% should be an empty heap now with the start address
  //from the program start


  return 0;
}



BYTE* mymalloc(unsigned int size) {
  // use sbrk to move program break
  // set to a variable so you can return that value
  // if top is null create first chunk

  // best candidate pointer
  chunkhead* best_candidate = NULL;

  
  // increment size by chunkhead
  size += sizeof(chunkhead);

  // make sure size is multiple of pagesize
  size = (size / PAGESIZE + 1) * PAGESIZE;

  

  heapsize += size + sizeof(chunkhead);

  // allocating first chunk
  if(top == NULL) {
    top = sbrk(size);
    chunkhead* current = top;
    current -> size = size;
    current -> info = 1;
    current -> prev = NULL;
    return (BYTE*)current + sizeof(chunkhead);
  } else {
    chunkhead* current = top;

    // iterate through chunks
    for(;current != NULL; current = (chunkhead*) current -> next) {

      if(current == 0) {
        return 0;
      }

      if(current -> info == 0 && current -> size >= size) {
        if(best_candidate != NULL) {
          if(best_candidate->size > current -> size && best_candidate -> size > size) {
            best_candidate = current;
          }
        } else {
          best_candidate = current;
        }
      }
    }


    if(best_candidate == NULL) {
      chunkhead* newChunk = sbrk(size);
      newChunk -> info = 1;
      newChunk -> size = size;
      newChunk -> prev = getLastChunk();
      // save getLastChunk, set next to newChunk

      if(getLastChunk()) {
        getLastChunk() -> next = newChunk;
      }
      newChunk -> next = NULL;
      return (BYTE*)newChunk + sizeof(chunkhead);
    }

    if(best_candidate -> size == size) {
      best_candidate-> info = 1;
      return (BYTE*)best_candidate + sizeof(chunkhead);
    }

    // couldn't find best fit case
    BYTE* newChunkAddress = split(best_candidate, size);
    return (BYTE*)newChunkAddress + sizeof(chunkhead);
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
  heapsize -= (ch -> size + sizeof(chunkhead));


  // case where we are freeing the second to last chunk, (DELETE TOP CHUNK)
  if(heapsize == 0 && ch -> info == 0 && ch -> next == NULL && ((chunkhead*)ch -> prev) -> info == 0) {
    // set program break to where ch was
    brk(top);
    top = NULL;
    return;
  }

  
    // ch next is occupied, ch prev is occupied (1)
    if(isNextFree == 0 && isPrevOccupied == 1) {
      return;
    }

    // ch next is occupied, ch prev is free (2)
    if(isNextOccupied == 1 && isPrevFree == 1) {
      
      if(ch -> prev != NULL) {
        ((chunkhead*)ch -> prev) -> size += ((chunkhead*)ch) -> size;

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
  /*BYTE* newChunkAddress = ((BYTE*)chunk) + sizeof(chunkhead) + size;
  chunkhead* newChunk = newChunkAddress;


  newChunk -> size = (chunk-> size) - sizeof(chunkhead) - size;
  chunk -> size = size;
  chunk -> info = 1;
  chunk -> next = (BYTE*)newChunk;

  newChunk -> info = 0;
  newChunk -> next = 0;
  newChunk -> prev = (BYTE*)chunk;
  chunk -> prev = ((chunkhead*)newChunk -> prev) -> prev;

  return ((BYTE*)chunk) + sizeof(chunkhead);*/

  BYTE* newChunkAddress = (BYTE*)chunk + size + sizeof(chunkhead);
  chunkhead* newChunk = newChunkAddress;

  newChunk -> size = size;
  // don't need to increment heapsize since splitting
  //heapsize += newChunk -> size;

  chunk -> size = (chunk ->size) - size;
  chunk -> info = 0;
  chunk -> next = (BYTE*)newChunk;

  newChunk -> info = 1;
  newChunk -> next = 0;
  newChunk -> prev = (BYTE*)chunk;
  chunk -> prev = ((chunkhead*)newChunk -> prev) -> prev;
  return newChunk;


}

void analyze() {
printf("\n--------------------------------------------------------------\n");
  int no;
  if(!top) {
  printf("no heap, program break on address: %x\n", sbrk(0));
  return;
  }
  chunkhead* ch = (chunkhead*)top;
  for (no=0; ch; ch = (chunkhead*)ch->next,no++) {
  printf("%d | current addr: %x |", no, ch);
  printf("size: %d | ", ch->size);
  printf("info: %d | ", ch->info);
  printf("next: %x | ", ch->next);
  printf("prev: %x", ch->prev);
  printf(" \n");
  }
  printf("program break on address: %x\n",sbrk(0));
}

chunkhead* getLastChunk() {
  if(!top) {
    return NULL;
  }

  chunkhead* ch = (chunkhead*)top;
  for (; ch->next; ch = (chunkhead*)ch->next);
  return ch;
}