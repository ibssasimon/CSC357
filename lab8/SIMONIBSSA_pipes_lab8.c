#include <stdio.h>
#include <stdalign.h>
#include <string.h>

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
  mywrite(&pipeA, "hello world", 12);
  mywrite(&pipeA, "it's a nice day", 16);

  myread(&pipeA, text, 12);
  printf("%s\n", text);
  // do we read only 16 bytes including hello world, or 16 bytes after hello world?
  myread(&pipeA, text, 16);
  printf("%s\n", text);
  //myread(&pipeA, text, 16);
  //printf("%s\n", text);
  return 0;
}






void init_pipe(mypipe* pipe, int size) {
  // malloc size for pipe. I think?
  pipe -> pipebuffer = (mypipe*)malloc(size);
  pipe -> buffersize = size;

  // set start and end
  pipe -> start_occupied = 0;
  pipe -> end_occupied = 0;

}


int mywrite(mypipe* pipe, BYTE* buffer, int size) {
  // set start tag
  pipe -> start_occupied = 1;
   // set end occupied
  pipe -> end_occupied = 0;
  if(size > pipe -> buffersize) {
    strncat(pipe -> pipebuffer, buffer, pipe -> buffersize);
    return pipe -> buffersize;
  }
  
  // strcat string to buffer
  strncat(pipe -> pipebuffer, buffer, size);

 
  return size;

}

int myread(mypipe* pipe, BYTE* buffer, int size) {
  // set start and end flags
  pipe -> start_occupied = 0;
  pipe -> end_occupied = 1;
  // respecting size of buffer
  if(size > pipe -> buffersize) {
    strncpy(buffer, pipe -> pipebuffer, pipe -> buffersize);
    return pipe -> buffersize;
  } else {
    strncpy(buffer, pipe->pipebuffer, size);
  }
  return size;
}