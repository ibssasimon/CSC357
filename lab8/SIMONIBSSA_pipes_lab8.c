#include <stdio.h>
#include <stdalign.h>

typedef unsigned char BYTE;

// pipe struct

typedef struct mypipe {
  BYTE* pipebuffer;
  int buffersize;
  int start_occupied;
  int end_occupied;
} mypipe;

// function declarations

//initializes (malloc) the pipe with a size of "size" and sets start and end.
void init_pipe(mypipe* pipe, int size);

//writes "size" bytes from buffer into the pipe, returns size
int mywrite(mypipe *pipe,BYTE* buffer, int size);

//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if pipe is empty
int myread(mypipe* pipe, BYTE* buffer, int size);



int main() {
  char text[100];
  mypipe pipeA;
  init_pipe(&pipeA, 32);
  printf("initialized pipe\n");
  return 0;
}






void init_pipe(mypipe* pipe, int size) {
  // malloc size for pipe. I think?
  mypipe* p = (mypipe*)malloc(size);
  pipe -> buffersize = p;

  // set start and end
  pipe -> start_occupied = 0;
  pipe -> end_occupied = 0;
}

int mywrite(mypipe* pipe, BYTE* buffer, int size) {

}

int myread(mypipe* pipe, BYTE* buffer, int size) {
  
}