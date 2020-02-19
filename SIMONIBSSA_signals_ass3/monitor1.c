#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <dirent.h>
#include <signal.h>


struct stat st;
struct dirent* dent;
DIR* dir;

int* childPid;
void handleAlarm(int sig) {
  printf("took longer than 10 seconds!\n");
  kill(*childPid, SIGKILL);
  wait(0);
}

void overwriteSignal(int sig) {
  printf("NOPE\n");
}
int main() {

  // overwrite signals
  signal(SIGINT, overwriteSignal);
  signal(SIGTSTP, overwriteSignal);
  signal(SIGTERM, overwriteSignal);

  int g;

  childPid = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* active = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *active = 0;
  int* q = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *q = 0;

  if(fork() == 0) {
    // child process
    
    char file[100];
    char userInput[100];
    char buffer[50];
    char tempBuffer[50];
    fflush(0);

    *childPid = getpid();
    while(1) {
      *active = 0;
      printf("\033[0;34m"); // set output color to blue
      printf("Simon Ibssa program 3!");
      printf("\033[0m"); //Resets the text to default color
      printf("$ ");
      scanf("%s", userInput);



      if(strcmp(userInput, "..") == 0) {
        strcpy(buffer, "..");
        printf("moved up a directory\n");
        continue;
      }

      if(strncmp("/", userInput, 1) == 0) {
        
        char* token;
        strcat(tempBuffer, ".");
        strcat(tempBuffer, "/");

        token = strtok(userInput, "/");
        while(token != NULL) {
          strcat(tempBuffer, token);
          token = strtok(NULL, userInput);
        }
        printf("%s\n", tempBuffer);
        dir = opendir(tempBuffer);

        if(dir != NULL) {
          printf("opening: %s\n", tempBuffer);
        } else {
          printf("subdirectory not found!\n");
        }

        continue;
        // OPEN SUB DIRECTORY HERE
      }
      // listing content of current directory
      if(strcmp(userInput, "list") == 0) {
        if(strcmp(buffer, "..") == 0) {
          strcpy(buffer, "..");
        } else {
          // opening current dir
          strcpy(buffer, ".");
        }
        dir = opendir(buffer);

        if(dir != NULL) {
          for(dent = readdir(dir); dent != NULL; dent = readdir(dir)) {
            printf("%s", dent -> d_name);

            if(dent -> d_type == DT_DIR) {
              printf(" - is a directory");
            }
            printf("\n");
          }
        }
      } else if(strcmp(userInput, "q") == 0) {
        *q = 1;
        return 0;
      } else {

        /*if(strcmp(userInput[0], "/") == 0) {
          printf("YOU WANNA GO IN A DIR?");
          int result = stat(userInput, &st);
          if(st.st_mode == S_IFDIR) {
            printf("opening directory!!\n");
          }
        }*/
        // assume it's a file name
        int result = stat(userInput, &st);

        if(result == -1) {
          printf("Could not open file! Try again\n");
        } else {
          // printing file information
          
          printf("I-node number: %ld\n", (long)st.st_ino);
          printf("Mode: %lo (octal)\n",(unsigned long)st.st_mode);
          printf("Link count: %ld\n", (long) st.st_nlink);
          printf("Ownership: UID=%ld GID=%ld\n", (long) st.st_uid, (long) st.st_gid);
          printf("Preferred I/O block size: %ld bytes\n", (long) st.st_blksize);
          printf("File size: %lld bytes\n", (long long) st.st_size);
          printf("Blocks allocated: %lld\n", (long long) st.st_blocks);
          printf("Last status change:       %s", ctime(&st.st_ctimespec));
          printf("Last file access:         %s", ctime(&st.st_atimespec));
          printf("Last file modification:   %s", ctime(&st.st_mtimespec));
          printf("\n");
        }

      }
    }
    return 1;
  } else {
    // parent process
    fflush(0);
    
    while(1) {

      // check if user pressed q
      if(*q == 1) {
        kill(*childPid, SIGKILL);
        wait(0);
        return 0;
      }
      sleep(1);
      *active = *active + 1;

      if(*active >= 10) {
        printf("10 seconds passed, inactive. KILL PROGRAM\n");
        kill(*childPid, SIGKILL);
        wait(0);
        return 0;
      }

    }

  }

  return 0;
}

