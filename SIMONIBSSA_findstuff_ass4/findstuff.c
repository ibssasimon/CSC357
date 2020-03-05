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
#include <errno.h>

// global vars
struct stat st;
struct dirent* dent;
DIR* dir;
int* childPid;
int* OGParent;

// function declaration
bool get_argument(char* line, int argn, char* result);
void add_null_term(char *txt);
void reportChild(int n);
static void list_dir (const char * dir_name, char* fileName);

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
  OGParent = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  char userBuffer[100];
  char* flag = (char*)mmap(NULL,  5* sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *flag = '\0';
  flag[2] = '\0';
  int* childPIDS = (int*)mmap(NULL, 10* sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  int* tasks = (int*)mmap(NULL, 10* sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


  // pipe for file descriptors
  pipe(fd);
  int save_stdin = dup(STDIN_FILENO);

  /* TODO(sibssa): get list, kill working, and subdir of subdir working */
  

  fflush(0);
  // update parent PID
  *OGParent = getpid();
  fflush(0);
  printf("parent pid: %d\n", *OGParent);
  int childIndex = 0;
  while(1) {
    printf("\n\033[0;34m"); // set output color to blue
    printf("find stuff");
    printf("\033[0m"); //Resets the text to default color
    printf("$ ");
    fflush(0);
    dup2(save_stdin,STDIN_FILENO);
    override = 0;
    // reading default user input
    read(0, userBuffer, 1000);

    /* TODO(sibssa): subdir searching */
    if(override == 0) {
      add_null_term(userBuffer);//to get a NULL at the end of the string in case of a user input
    }
    printf("%s\n",userBuffer);
    fflush(0);

    if(strncmp("q", userBuffer, 1) == 0) {
      kill(*OGParent, SIGKILL);
      return 0;
    }


    if(strncmp("list ..", userBuffer, 7) == 0) {
      printf("kid processes running: ");
      printf("[");
      for(int i = 0; i < 10; i++) {
        if(tasks[i] != 0) {
          printf("kid %d is searching for file", childPIDS[i]);
        } else {
          printf("%d, ", childPIDS[i]);
        }
        if(i == 9) {
          printf("%d", childPIDS[i]);
          break;
        }
      }
      printf("]\n");

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
      printf("my flag: %s\n", flag);
      char fileName[100] = "";
      int fileSucess = get_argument(userBuffer, 1, fileName);
      
      fflush(0);
      
      // start fork to find file in dir
      if(fork() == 0) {
        // child case
        *childPid = getpid();
        childPIDS[childIndex] = *childPid;
        tasks[childIndex] = 1;
        childIndex++;
        char directory[PATH_MAX];
        
        
        // For the flag situations -- currently I'm breaking out of loops before sending interrupt signal. That part is broken.
        /* TODO(sibssa): send interrupt signal to parent process */
        if(*flag == '\0') {
          // no flag is set, search current dir
          strcpy(directory, ".");
          dir = opendir(directory);
          int kidnum=0;
          char result[10000] = "";
        
          for(int i=0;i<10;i++) if(childPIDS[i]==0) {childPIDS[i]=getpid();kidnum=i;break;}
          
          sprintf(result,"\nkid %d is reporting!",kidnum);
          // build up result then pipe it
          if(dir != NULL) {
            for(dent = readdir(dir); dent != NULL; dent = readdir(dir)) {
              // compare file names
              if(strncmp(dent -> d_name, fileName, strlen(fileName) - 1) == 0) {
                // piping dir and filename
                // close read
                strcat(result, "\n");
                strncat(result, fileName,strlen(fileName)-1);
                strcat(result, " in ");
                strcat(result, directory);
                strcat(result, "\n");
                if(getcwd(directory, sizeof(directory)) != NULL) {
                 // printf(" in: %s\n", directory);
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
          result[strlen(result)]=0;
          write(fd[1],result,strlen(result)+1);
          close(fd[1]); //close write  
          kill(*OGParent, SIGUSR1);
          strcpy(result, 0);
        }

        // Algorithm to search subdirectories of current dir
        if(flag[1] == 's') {
          char tempBuffer[100] = "";
          strcpy(directory, ".");
          dir = opendir(directory);
          int kidnum=0;
          char result[10000] = "";
        
          for(int i=0;i<10;i++) if(childPIDS[i]==0) {childPIDS[i]=getpid();kidnum=i;break;}
          
          sprintf(result,"\nkid %d is reporting!",kidnum);
          /*if(getcwd(directory, sizeof(directory)) != NULL) {
            list_dir(directory, fileName);
          }*/

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
                      printf("found: %s", fileName);
                      printf("in: %s\n", directory);
                    if(getcwd(directory, sizeof(directory)) != NULL) {
                      // printf(" in: %s\n", directory);
                    }
                    break;
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


          result[strlen(result)]=0;
          write(fd[1],result,strlen(result)+1);
          close(fd[1]); //close write  
          kill(*OGParent, SIGUSR1);
          strcpy(result, 0);
        }      

        
      } else {
        // parent case
      }

      
    }

    /* TODO(sibssa): segfaulting when killing PID that user enters */
    if(strncmp("kill", userBuffer, 3) == 0) {
      char pid[10] = "";
      int pidSuccess = get_argument(userBuffer, 1, pid);
      pid[strlen(pid) - 1] = '\0';
      printf("killing: %s", pid);
      int iPid = atoi(pid);
      kill(atoi(pid), SIGKILL);
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



static void list_dir (const char * dir_name, char* fileName) {
    DIR * d;

    /* Open the directory specified by "dir_name". */

    d = opendir (dir_name);

    /* Check it was opened. */
    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    while (1) {
        struct dirent * entry;
        const char * d_name;

        /* "Readdir" gets subsequent entries from "d". */
        entry = readdir (d);
        if (! entry) {
            /* There are no more entries in this directory, so break
               out of the while loop. */
            break;
        }
        d_name = entry->d_name;
        /* Print the name of the file and directory. */
	//printf ("%s/%s\n", dir_name, d_name);
  char potentialResult[1000] = "";
  strcat(potentialResult, dir_name);
  strcat(potentialResult, "FILE");
  strcat(potentialResult, d_name);
  printf("%s", potentialResult);
  
  bool match = false;
  for(int i = strlen(potentialResult); i > 0; i--) {
    if(potentialResult[i] == '/') {
      int k = 0;
      for(int j = i+1; j < strlen(fileName); i++) {
        if(potentialResult[j] == fileName[k]) {
          match = true;
          k++;
        }
      }
    }
  }
  if(match) {
    /* pipe stuff */
    strcpy(potentialResult, 0);
    strcat(potentialResult, "\n");
    strncat(potentialResult, fileName,strlen(fileName)-1);
    strcat(potentialResult, " in ");
    strcat(potentialResult, dir_name);
    strcat(potentialResult, "\n");
  }


  strcpy(potentialResult, 0);

  

#if 0
	/* If you don't want to print the directories, use the
	   following line: */

        if (! (entry->d_type & DT_DIR)) {
	    printf ("%s/%s\n", dir_name, d_name);
	}

#endif /* 0 */


        if (entry->d_type & DT_DIR) {

            /* Check that the directory is not "d" or d's parent. */
            
            if (strcmp (d_name, "..") != 0 &&
                strcmp (d_name, ".") != 0) {
                int path_length;
                char path[PATH_MAX];
 
                path_length = snprintf (path, PATH_MAX,
                                        "%s/%s", dir_name, d_name);
                printf ("%s\n", path);
                if (path_length >= PATH_MAX) {
                    fprintf (stderr, "Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }
                /* Recursively call "list_dir" with the new path. */
                list_dir (path, fileName);
            }
	}
    }
    /* After going through all the entries, close the directory. */
    if (closedir (d)) {
        fprintf (stderr, "Could not close '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
}
