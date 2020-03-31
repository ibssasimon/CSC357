#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>

int main() {
  printf("Hello world\n");
  int i = 0;
  i++;
  printf("i: %d\n", i);
  return 0;
}
