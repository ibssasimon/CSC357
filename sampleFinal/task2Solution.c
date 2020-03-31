#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

// global vars
int fd[2];
FILE* file;

void close_file()
{
    fclose(file);
}


int main() 
{
    // task 2
    char inputBuffer[1000];
    inputBuffer[0] = 0;
    int* childPID = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pipe(fd);
    signal(SIGUSR1,close_file);
        // fork here

    if(fork() == 0) 
    {
        //printf("Entering fork...\n");
        // child case
        *childPID = getpid();
        
        fflush(0);
        // infinite loop
        file = fopen("result.txt", "w+");

        //close(fd[1]);
        while(1) 
        {
            // wait 5 secodns 
            sleep(5);
            //printf("Child PID: %d\n",*childPID);
            char change[1000];
            
            // read from pipe
            write(fd[1], "\0", 1);
            int bytesRead = read(fd[0], change, 1000);
            // if something into pipe write into file
            //printf("bytesRead: %d\n", bytesRead);
            //printf("inputBuffer from child: %s\n", change);

            if(bytesRead != 0) 
            {
                //printf("wrtinig to file....\n");

                if(file != NULL) 
                {
                    //printf("success\n");
                    int wrote = fwrite(change, 1, bytesRead-1, file);
                    //printf("wrote %d bytes\n", wrote);
                    fflush(file);
                }
            }
            
        }
        fclose(file);
        return 0;
    }


    else
    {
        //close(fd[0]);
        while(1) 
        {
            // read here

            printf("write something: ");
            scanf("%s", inputBuffer);
            //printf("inputBuffer: %s\n", inputBuffer);

            int written = write(fd[1], inputBuffer, strlen(inputBuffer));
            
            //printf("written to child: %d\n",written);

            if(strcmp(inputBuffer, "q") == 0) 
            {
                kill(*childPID, SIGUSR1);
                kill(*childPID, SIGUSR1);
                kill(*childPID, SIGKILL);
                waitpid(*childPID, 0, 0);
                wait(0);
                return 0;
            }
        }
    }
    
    return 0;
}