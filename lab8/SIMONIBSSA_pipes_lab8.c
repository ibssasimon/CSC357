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

  myread(&pipeA, text, 12);
  printf("%s\n", text);
  myread(&pipeA, text, 16);
  printf("%s\n", text);

  mywrite(&pipeA, "and now we test the carryover", 30);
  myread(&pipeA, text, 30);
  printf("%s\n", text);
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

  // carryover case 
  if(size > ( pipe -> buffersize - pipe -> start_occupied)) {
    int remaining = pipe -> buffersize - (pipe -> end_occupied);
    pipe -> end_occupied = ((pipe ->buffersize) - remaining + size) % pipe -> buffersize;
    pipe ->start_occupied = pipe -> end_occupied;

    for(int i = 0; i < size; i++) {
      pipe -> pipebuffer[((pipe -> start_occupied )+ i ) % (pipe -> buffersize)] = buffer[i];
      test[((pipe -> start_occupied )+ i )% (pipe -> buffersize)] = buffer[i];
    }
    return size;
  }

  // define string Length
  int stringLength = strlen(buffer);
   
  if(size > pipe -> buffersize) {
    for(int i = 0; i < stringLength; i++) {
      if(i < stringLength) {
        pipe -> pipebuffer[i + (pipe -> end_occupied - stringLength)] = buffer[i];
        test[i + (pipe -> end_occupied - stringLength)] = pipe -> pipebuffer[i + (pipe -> end_occupied - stringLength)];
      } else {
        pipe -> end_occupied += 1;
      }
    }
    return pipe -> buffersize;
  }
  
  

  // set end occupied
  pipe -> end_occupied += stringLength;
  pipe -> end_occupied = (pipe -> end_occupied % pipe -> buffersize);

  for(int i = 0; i < size; i++) {
    if(i < stringLength) {
      pipe -> pipebuffer[i + (pipe -> end_occupied - stringLength)] = buffer[i];
      test[i + (pipe -> end_occupied - stringLength)] = pipe -> pipebuffer[i + (pipe -> end_occupied - stringLength)];
    } else {
      pipe -> end_occupied += 1;
    }
  }

 
  return size;

}

int myread(mypipe* pipe, BYTE* buffer, int size) {
  // set start and end flags
  //pipe -> start_occupied = 0;
  // pipe -> end_occupied = 1;
  // respecting size of buffer
  *buffer = 0;

  // carryover case 
  if(size > ( pipe -> buffersize - pipe -> start_occupied)) {
    pipe ->start_occupied = pipe -> end_occupied;
    int remaining = pipe -> buffersize - (pipe -> end_occupied);
    pipe -> end_occupied = ((pipe ->buffersize) - remaining + size) % pipe -> buffersize;

    for(int i = 0; i < size; i++) {
      buffer[i] = pipe -> pipebuffer[((pipe -> start_occupied )+ i ) % (pipe -> buffersize)];
      read[i] = test[((pipe -> start_occupied )+ i )% (pipe -> buffersize)];
    }
    return size;
  }
  
  if(size > pipe -> buffersize) {
    for(int i = pipe -> start_occupied; i < pipe -> start_occupied + pipe -> buffersize; i++) {
      buffer[i - (pipe -> start_occupied)] = pipe -> pipebuffer[i];
      read[i - (pipe -> start_occupied)] = pipe -> pipebuffer[i];
      pipe -> pipebuffer[i] = 0;
      test[i] = 0;
    }

    pipe -> start_occupied = pipe -> buffersize;
    return pipe -> buffersize;
  } else {

    for(int i = pipe -> start_occupied; i < pipe -> start_occupied + size; i++) {
      buffer[i - (pipe -> start_occupied)] = pipe -> pipebuffer[i];
      read[i - (pipe -> start_occupied)] = pipe -> pipebuffer[i];
      pipe -> pipebuffer[i] = 0;
      test[i] = 0;
    }

    pipe -> start_occupied += size;
  }
  return size;
}