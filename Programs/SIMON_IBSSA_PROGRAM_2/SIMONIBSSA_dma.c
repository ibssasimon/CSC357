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
chunkhead* getLastChunk();
void analyse();
void analyze();



int main() {
  printf("Program 2\n");
  // defining first chunkhead
  // if heapsize is zero. SPECIAL case: allocate chunkhead + size
  // brk(size + chunkhead)

  //printf("%p\n", top);

 //unsigned char* c = mymalloc(8000);
  //myfree(c);
  //analyze();
  //unsigned char* d = mymalloc(4000);
  //analyze();



  BYTE* a[100];
  printf("first: \n");
  analyze();//50% points
  for(int i=0;i<100;i++){
    a[i]= mymalloc(1000);
  }
  for(int i=0;i<90;i++) {
    myfree(a[i]);
  }
  printf("second: \n");
  analyze(); //50% of points if this is correct
  printf("third: \n");
  myfree(a[95]);
  analyze();
  a[95] = mymalloc(1000);
  analyze();//25% points, this new chunk should fill the smaller free one
  //(best fit)
  for(int i=90;i<100;i++) {
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

    // iterate through chunks
    for(;current != NULL; current = (chunkhead*) current -> next) {

      if(current == 0) {
        return 0;
      }

      if(current -> info == 0 && current -> size >= size) {
        if(best_candidate != NULL) {
          if(best_candidate->size > current -> size) {
            best_candidate = current;
            }
        } else {
          best_candidate = current;
        }

        if(best_candidate == NULL) {
          chunkhead* newChunk = sbrk(size + sizeof(chunkhead));
          newChunk -> info = 1;
          newChunk -> size = size;
          newChunk -> prev = getLastChunk();
          newChunk -> next = NULL;
          return newChunk;
        }

        if(best_candidate -> size == size) {
          best_candidate-> info = 1;
          return (BYTE*)current + sizeof(chunkhead);
        }

        BYTE* newChunkAddress = split(best_candidate, size);
        return newChunkAddress;
      }

    }

    /*for(;current != NULL; current = (chunkhead*) current->next) {
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
    }*/
    chunkhead* new = sbrk(size);
    heapsize += size;

    new -> size = size;
    new -> info = 1;
    new -> prev = getLastChunk();
    new -> next = NULL;
    getLastChunk() -> next = new;

    return new;
  }

  
  

 
  return 0;
}


void myfree(BYTE* myaddress) {
  // implementation of free function

  chunkhead* ch;
  ch = (chunkhead*)(myaddress);

  if(ch -> info == 0 && ch -> next == NULL && ch -> prev == NULL) {
    ch = NULL;
    return;
  }


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

void analyse() {

  printf("-----PRINTING HEAP NOW ------------\n");
  int i = 0;
  chunkhead* ch = top;

  while(ch != NULL) {
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

    ch = ch -> next;
  }

  
    /*for(;ch != NULL; ch = (chunkhead*) ch->next) {
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
    }*/

  printf("--------HEAP SIZE: %d --------\n", heapsize);
  printf("Program break: %p\n", sbrk(0));
}

void analyze() {
printf("\n--------------------------------------------------------------\n");
  if(!top) {
  printf("no heap, program break on address: %x\n", sbrk(0));
  return;
  }
  chunkhead* ch = (chunkhead*)top;
  for (int no=0; ch; ch = (chunkhead*)ch->next,no++) {
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