#include <stdio.h>

#define BYTE unsigned char

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

}

void init_pipe(mypipe* pipe, int size) {

}

int mywrite(mypipe* pipe, BYTE* buffer, int size) {

}

int myread(mypipe* pipe, BYTE* buffer, int size) {
  
}