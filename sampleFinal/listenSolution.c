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

void main()
{
    int fd = shm_open("write",O_RDWR,0777);
    char *p = mmap(0,sizeof(char)*100,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    while(1)
    {
        
        sleep(2);
        printf("Checkkkk\n");
        if(p[0] != 0)
        {
            printf("This was inside: %s\n",p);
            p[0]=0;
        }
    }
    
}