#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h> 
#include <sys/types.h> 
#include <string.h>





// global vars
int fd[2];
FILE* file;



int main() {
  // task 2


  char inputBuffer[1000];
  inputBuffer[0] = 0;
  int* childPID = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  

  pipe(fd);
    // fork here


  if(fork() == 0) {
    printf("Entering fork...\n");
    // child case
    *childPID = getpid();
    
    


    fflush(0);
    // infinite loop
    file = fopen("result.txt", "w+");

    close(fd[1]);
    while(1) {
      // wait 5 secodns 
      sleep(5);
      // read from pipe
      int bytesRead = read(fd[0], inputBuffer, strlen(inputBuffer));
      // if something into pipe write into file
      printf("bytesRead: %d\n", bytesRead);
      printf("inputBuffe: %s\n", inputBuffer);


      if(bytesRead != 0) {
        printf("wrtinig to file....\n");

        if(file != NULL) {
          printf("success\n");
          int write = fwrite(inputBuffer, bytesRead, 1, file);
          printf("wrote %d bytes\n", write);

        }
      }
    }
    fclose(file);
    return 0;
  }

  close(fd[0]);
  while(1) {
    // read here

    printf("write something: ");
    scanf("%s", inputBuffer);
    printf("inputBuffer: %s\n", inputBuffer);

    write(fd[1], inputBuffer, strlen(inputBuffer));

    if(strncmp(inputBuffer, "q", 1) == 0) {
      kill(*childPID, SIGKILL);
      waitpid(*childPID, 0, WNOHANG);
      fclose(file);
    }



  }
  


  return 0;
}