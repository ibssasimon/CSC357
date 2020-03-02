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

// global vars
struct stat st;
struct dirent* dent;
DIR* dir;
int* childPid;

// function declaration
bool get_argument(char* line, int argn, char* result);

int main() {
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
  char* flag = (char*)mmap(NULL,  3* sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *flag = '\0';
  flag[2] = '\0';

  while(1) {
    printf("Entering program\n");
    printf("\033[0;34m"); // set output color to blue
    printf("find stuff");
    printf("\033[0m"); //Resets the text to default color
    printf("$ ");
    fflush(0);
    // reading default user input
    read(0, userBuffer, 100);


    // update parent PID
    *OGParent = getpid();
    if(strncmp("find", userBuffer, 4) == 0) {
      // fork and find file

      // finding flag
      for(int i = 0; i < strlen(userBuffer); i++) {
        if(userBuffer[i] == '-') {
          // out of bounds wrapping
          if(i + 1 < strlen(userBuffer)) {
            if(userBuffer[i+1] == 's') {
              flag[0] = '-';
              flag[1] = 's';
              break;
            }
            if(userBuffer[i+1] == 'f') {
              flag[0] = '-';
              flag[1] = 'f';
              break;
            }

          }
        }
      }

      // call get_argument with 2. If flag is -f call get argument again with 3 and strcat -s to flag
      int flagSuccess = get_argument(userBuffer, 2, flag);
      printf("flag: %s\n", flag);
      char fileName[100];
      int fileSucess = get_argument(userBuffer, 1, fileName);
      printf("my file name: %s\n", fileName);
      return 0;
      // start fork to find file in dir
      if(fork() == 0) {
        // child case
        *childPid = getpid();
        char directory[PATH_MAX];

        if(*flag == '\0') {
          // no flag is set, search current dir
          strcpy(directory, ".");
          dir = opendir(directory);

          if(dir != NULL) {
            for(dent = readdir(dir); dent != NULL; dent = readdir(dir)) {
              printf("searching ....%s", dent -> d_name);

              if(dent -> d_type == DT_DIR) {
                printf(" - is a directory");
              }
              printf("\n");
            }
          }

        }
      } else {
        // parent case
      }
      return 0;

      
    }
  }


  /*

  ------- FOR FINDING TEXT -----
  char fileName[1000];
        int success = get_argument("hello world -s", 1, fileName);
  --------------------------------------------
  if(fork() == 0) {
    // child case

  } else {
    char file[100];
    char userInput[100];
    char buffer[50];
    char tempBuffer[50];
    char directory[PATH_MAX];

    fflush(0);

    *childPid = getpid();
    while(1) {
      *active = 0;
      printf("\033[0;34m"); // set output color to blue
      printf("find stuff");
      // print current directory

      if(getcwd(directory, sizeof(directory)) != NULL) {
        // usually would print dir here
      }

      printf("\033[0m"); //Resets the text to default color
      printf("$ ");
      scanf("%s", userInput);



      if(strcmp(userInput, "..") == 0) {
        strcpy(buffer, "..");
        // update directory
        for(int i = strlen(directory); i > 0; i--) {
          // remove first /
          if(directory[i] == '/') {
            directory[i] = '\0';
            break;
          }
        }
        chdir(directory);
        printf("moved up a directory!\n");
        continue;
      }

      if(strncmp("/", userInput, 1) == 0) {
        *tempBuffer = '\0';
        char* token;
        strcat(tempBuffer, "/");

        token = strtok(userInput, "/");
        while(token != NULL) {
          strcat(tempBuffer, token);
          token = strtok(NULL, userInput);
        }
        strcat(directory, tempBuffer);

        // OPEN SUB DIRECTORY HERE
        dir = opendir(directory);
        /* TODO(sibssa): move into subdirectory 

        if(dir != NULL) {
          printf("opening: %s\n", tempBuffer);
        } else {
          printf("Folder does not exist!\n");
        }
        chdir(directory);
        continue;
      }
      // listing content of current directory
      if(strcmp(userInput, "list") == 0) {
        if(strcmp(buffer, "..") == 0) {
          strcpy(buffer, "..");
        } else {
          // opening current dir
          strcpy(buffer, ".");
        }
        dir = opendir(directory);

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
  }*/
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