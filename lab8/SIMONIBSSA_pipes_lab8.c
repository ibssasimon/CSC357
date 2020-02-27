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


char test[100] = "";
char read[100] = "";
int main() {
  char text[100];
  mypipe pipeA;

  init_pipe(&pipeA, 32);
  // strcat removing \0 (1)
  mywrite(&pipeA, "hello world", 12);
  mywrite(&pipeA, "it's a nice day", 16);

  // do we read only 16 bytes including hello world, or 16 bytes after hello world?
  myread(&pipeA, text, 12);
  printf("reading 12 bytes: %s\n", text);
  myread(&pipeA, text, 16);
  printf("reading 16 bytes: %s\n", text);

  // only reading 29 bytes? How to test carryover(2)
  mywrite(&pipeA, "and now we test the carryover", 30);
  myread(&pipeA, text, 30);
  printf("reading 30 bytes: %s\n", text);
  // expected output? (3)
  return 0;
}






void init_pipe(mypipe* pipe, int size) {
  // malloc size for pipe. I think?
  pipe -> pipebuffer = (mypipe*)malloc(size);
  for(int i = 0; i < strlen(pipe -> pipebuffer); i++) {
    pipe -> pipebuffer[i] = 0; 
  }
  pipe -> buffersize = size;

  // set start and end
  pipe -> start_occupied = 0;
  pipe -> end_occupied = 0;

}


int mywrite(mypipe* pipe, BYTE* buffer, int size) {
  // set start tag
  pipe -> start_occupied = 0;

  // define string Length
  int stringLength = strlen(buffer);
   
  if(size > pipe -> buffersize) {
    strncat(pipe -> pipebuffer, buffer, pipe -> buffersize);
    return pipe -> buffersize;
  }
  
  

  // set end occupied
  pipe -> end_occupied += stringLength;

  for(int i = 0; i < size; i++) {
    if(i < stringLength) {
      pipe -> pipebuffer[i + (pipe -> end_occupied - stringLength)] = buffer[i];
      test[i + (pipe -> end_occupied - stringLength)] = pipe -> pipebuffer[i + (pipe -> end_occupied - stringLength)];
    } else {
      pipe -> end_occupied += 1;
    }
  }
  //strncat(pipe -> pipebuffer, buffer, size);

 
  return size;

}

int myread(mypipe* pipe, BYTE* buffer, int size) {
  // set start and end flags
  //pipe -> start_occupied = 0;
  // pipe -> end_occupied = 1;
  // respecting size of buffer
  *buffer = 0;
  *read = 0;
  if(size > pipe -> buffersize) {
    strncpy(buffer, pipe -> pipebuffer, pipe -> buffersize);
    // move the previous forward & memset pipebuffer
    return pipe -> buffersize;
  } else {

    for(int i = pipe -> start_occupied; i < pipe -> start_occupied + size; i++) {
      buffer[i - (pipe -> start_occupied)] = pipe -> pipebuffer[i];
      read[i - (pipe -> start_occupied)] = pipe -> pipebuffer[i];
    }

    pipe -> start_occupied = size;
  }
  return size;
}