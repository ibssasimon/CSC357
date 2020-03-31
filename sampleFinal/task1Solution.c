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
#include <time.h>

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m" 
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"


int fd[2];

void overwrite()
{
    dup2(fd[0],STDIN_FILENO);
}

int main()
{
    char input[1000];
    input[0] =0;
    int* childPID1 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* childPID2 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* flag = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pipe(fd);
    int save_stdin = dup(STDIN_FILENO);
    int parentPID = getpid();
    signal(SIGUSR1,overwrite);
    *flag = 0;
    
    if(fork()== 0)
    {
        *childPID1 = getpid();
        //printf("\nchild1 PID: %d\n",*childPID1);
        while(1)
        {
            sleep(5);
            time_t now;
            time(&now);
            *flag = 1;
            kill(parentPID,SIGUSR1);
            int written = write(fd[1],KRED,strlen(KRED));
            kill(parentPID,SIGUSR1);
            write(fd[1],ctime(&now),strlen(ctime(&now)));
            kill(parentPID,SIGUSR1);
            *flag = 0;
            write(fd[1],KNRM,strlen(KNRM));
        }
        return 0; 
    }

    if(fork()== 0)
    {
        *childPID2 = getpid();
        //printf("\nchild2 PID: %d\n",*childPID2);
        while(1)
        {
            sleep(11);
            time_t now;
            time(&now);
            *flag = 1;
            kill(parentPID,SIGUSR1);
            write(fd[1],KCYN,strlen(KCYN));
            kill(parentPID,SIGUSR1);
            write(fd[1],ctime(&now),strlen(ctime(&now)));
            kill(parentPID,SIGUSR1);
            *flag = 0;
            write(fd[1],KNRM,strlen(KNRM));
        }
        return 0;
    }
    

    while(1)
    {
        // printf("Flag: %d\n",*flag);
        // fflush(0);
        if(*flag == 0)
        {
            printf("\nType something: ");
            fflush(0);
            dup2(save_stdin,STDIN_FILENO);
        }
        int temp = read(STDIN_FILENO,input,1000); 
        
        input[temp] = 0;
        
        if(strncmp(input,"q\n",2)==0)
        {
            kill(*childPID1,SIGKILL);
            kill(*childPID2,SIGKILL);
            wait(0);
            return 0;
        }
        printf("%s",input);
        fflush(0);
        input[0]=0;
    }



    return 0;
}