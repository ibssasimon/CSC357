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

int main()
{
    int fd = shm_open("write",O_RDWR|O_CREAT,0777);
    ftruncate(fd,sizeof(char)*100);
    char *p = mmap(0,sizeof(char)*100,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    p[0]=0;
    char holder[100];
    int *childPID = mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    char *args[2];
    args[0] = malloc(sizeof(char)*15);
    args[1] = malloc(sizeof(char)*15);
    strcpy(args[0],"listen");
    args[1] = 0;

    
    if(fork()==0)
    {
        *childPID = getpid();
        printf("child pid: %d\n",*childPID);
        execv(args[0],args);
        return 0;
    }

    while(1)
    {
        printf("Please enter a word: ");
        scanf("%s",holder);
        if(strcmp(holder,"q")==0)
        {
            break;
        }
        if(p[0]==0)
        {
            strcpy(p,holder);
        }
        else
        {
            strcat(p,holder);
        }
        printf("You fully wrote: %s\n",p);
    }
    kill(*childPID,SIGKILL);
    wait(0);
    close (fd);
    shm_unlink("write");
    return 0;
}