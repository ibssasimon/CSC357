#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <string.h>


typedef struct chunkdata{
int offset;
char c;
} chunkdata;

chunkdata ch;

FILE* file;
int main() {
  int x;
  int y;

  file = fopen("crypt_sec3.bin", "rb");

  if(file == NULL) {
    printf("Cannot open file!\n");
    return 0;
  }



  while(1) {
    fread(&ch.offset, 1, sizeof(ch.offset), file);
    fread(&ch.c, 1, sizeof(ch.c), file);
    int c = ch.c;
    int offset = ch.offset;
    
    if(c == 0 && offset == 0) {
      break;
    }

    

    c = c - offset;
    printf("%c", c);
  }

  return 0;

}