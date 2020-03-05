#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <signal.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

// global vars
struct stat st;
struct dirent* dent;
DIR* dir;
int* childPid;

// function declaration
bool get_argument(char* line, int argn, char* result);
void add_null_term(char *txt);
void reportChild(int n);

// file descriptors
int fd[2];
int override = 0;

int main() {

  // defining signal handlers
  signal(SIGUSR1, reportChild);

  // vars
  printf("Starting program\n");
  int g;
  childPid = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* active = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *active = 0;
  int* q = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *q = 0;
  int* OGParent = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  char userBuffer[100];
  char* flag = (char*)mmap(NULL,  5* sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *flag = '\0';
  flag[2] = '\0';
  int* childPIDS = (int*)mmap(NULL, 10* sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  // pipe for file descriptors
  pipe(fd);
  int save_stdin = dup(STDIN_FILENO);

  

  fflush(0);
  // update parent PID
  *OGParent = getpid();
  fflush(0);
  printf("parent pid: %d\n", *OGParent);
  int childIndex = 0;
  while(1) {
    printf("\033[0;34m"); // set output color to blue
    printf("find stuff");
    printf("\033[0m"); //Resets the text to default color
    printf("$ ");
    fflush(0);
    dup2(save_stdin,STDIN_FILENO);
    override = 0;
    // reading default user input
    read(0, userBuffer, 1000);

    if(override == 0) {
      add_null_term(userBuffer);//to get a NULL at the end of the string in case of a user input
    }

    fflush(0);

    if(strncmp("q", userBuffer, 1) == 0) {
      return 0;
    }

    // print f here
    
    if(strncmp("find", userBuffer, 4) == 0) {
      // fork and find file

      // call get_argument with 2. If flag is -f call get argument again with 3 and strcat -s to flag
      int flagSuccess = get_argument(userBuffer, 2, flag);
      if(flag[1] == 'f') {
        // strcat -s
        char tempflag[5] = "";
        int flagSuccess2 = get_argument(userBuffer, 3, tempflag);
        strcat(flag, tempflag);
      }
      printf("flag: %s\n", flag);
      char fileName[100] = "";
      int fileSucess = get_argument(userBuffer, 1, fileName);
      printf("my file name: %s\n", fileName);
      
      fflush(0);
      
      // start fork to find file in dir
      if(fork() == 0) {
        // child case
        *childPid = getpid();
        childPIDS[childIndex] = *childPid;
        childIndex++;
        char directory[PATH_MAX];
        
        
        // For the flag situations -- currently I'm breaking out of loops before sending interrupt signal. That part is broken.
        /* TODO(sibssa): send interrupt signal to parent process */
        if(*flag == '\0') {
          // no flag is set, search current dir
          strcpy(directory, ".");
          dir = opendir(directory);

          char result[10000] = "";
          int kidnum=0;
          for(int i=0;i<10;i++) if(childPIDS[i]==0) {childPIDS[i]=getpid();kidnum=i;break;}
          // build up result then pipe it
          if(dir != NULL) {
            for(dent = readdir(dir); dent != NULL; dent = readdir(dir)) {
              // compare file names
              if(strncmp(dent -> d_name, fileName, strlen(fileName) - 1) == 0) {
                // piping dir and filename
                // close read
                printf("found stuff: %s", fileName);
                if(getcwd(directory, sizeof(directory)) != NULL) {
                  printf(" in: %s\n", directory);
                }
                  break;
               // close(fd[0]);
               // sprintf(result,"kid %d is reporting!",kidnum);
               // strcat(result,"\nfound stuff:\n");
               // strcat(result, fileName);
               // strcat(result, " in ");
               // strcat(result, directory);
               // strcat(result, "\n");
               // strcat(result,"\0");//null terminator is important, because pipe!
               // write(fd[1],result,strlen(result));
                //close(fd[1]); //close write  
              }

            }
          }
          kill(*OGParent, SIGUSR1);
        }

        // Algorithm to search subdirectories of current dir
        if(flag[1] == 's') {
          char tempBuffer[100] = "";
          printf("searching all subdirectories\n");

          strcpy(directory, ".");
          dir = opendir(directory);

          if(dir != NULL) {
            for(dent = readdir(dir); dent != NULL; dent = readdir(dir)) {

              if(dent ->d_type == DT_DIR) {
                // modify dir to contain subdirectory
                DIR* dir2;
                struct dirent* dent2;
                strcat(tempBuffer, "/");
                strcat(tempBuffer, dent -> d_name);

                strcat(directory, tempBuffer);

                // open subdirectory to search for file
                dir2 = opendir(directory);
                if(dir2 != NULL) {
                  for(dent2 = readdir(dir); dent2 != NULL; dent2 = readdir(dir2)) {
                    if(strncmp(dent2 -> d_name, fileName, strlen(fileName) - 1) == 0) {
                      chdir(directory);
                      if(getcwd(directory, sizeof(directory)) != NULL) {
                        printf("found stuff: %s", fileName);
                        printf("in: %s\n", directory);
                      }
                      break;
                      kill(*OGParent, SIGUSR1);
                    }
                  }
                }
              
                // check to see if we actually moved into a subdir or just current dir
                if(tempBuffer[2] != 0) {
                  // updating dir after searching subdirectory
                  for(int i = strlen(directory); i > 0; i--) {
                    // remove first /
                    if(directory[i] == '/') {
                      directory[i] = '\0';
                      break;
                    }
                  }
                }
              }
              memset(tempBuffer, 0, strlen(tempBuffer));
            }
          }
        }      

        
      } else {
        // parent case
      }

      
    }

    /* TODO(sibssa): segfaulting when killing PID that user enters */
    if(strncmp("kill", userBuffer, 3) == 0) {
      printf("kill this mothafucka\n");
      char pid[10] = "";
      int pidSuccess = get_argument(userBuffer, 1, pid);
      pid[strlen(pid) - 1] = '\0';
      printf("killing: %s", pid);
      int iPid = atoi(pid);
      //kill(atoi(pid), SIGKILL);
    }
  }


    //killing the kid for good:
    for(int i=0;i<10;i++) if(childPIDS[i]!=0)       waitpid(childPIDS[i],0,0);
    printf("input check: %s",userBuffer);

  return 0;
}


/* Function provided by Professor to extract argument from command line */
bool get_argument(char* line, int argn, char* result) {
	//firstly: remove all spaces at the front
	char temp[1000];
	int start_space = 1;
	for (int i = 0, u = 0; i <= strlen(line); i++)
		if (line[i] == ' ' && start_space == 1) continue;
		else {
			temp[u++] = line[i]; 
			start_space = 0;
		}
	//now remove an double or tripple spaces
	char temp2[1000];
	int space_on = 0;
	for (int i = 0, u = 0; i <= strlen(temp); i++) {
		if (space_on == 1 && temp[i] == ' ') continue;
		temp2[u++] = temp[i];
		if (temp[i] == ' ') space_on = 1;
		else space_on = 0;
	}
	//finally extract the arguments
	int start, end;
	start = end = 0;
	int count = 0;
	int quote = 0;
	for (int i = 0; i <= strlen(temp2); i++)
		if (temp2[i] == '\"') quote = !quote;
		else if (quote == 0 &&(temp2[i] == ' ' || temp2[i] == 0)) {
			end = i;
			if (argn == count) {
				int length = end - start;
				strncpy(result, temp2 + start, length);
				result[length] = 0;
				return 1;
			}
			start = end + 1;
			count++;
		}
	return 0;
}

void add_null_term(char *txt){
for(int i=0;i<100;i++)
    if(txt[i]=='\n') {txt[i+1]=0;break;}
}

void reportChild(int n) {
  dup2(fd[0],STDIN_FILENO); //Overwrite userinput
  override=1;
}