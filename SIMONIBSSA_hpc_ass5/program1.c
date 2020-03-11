/* Simon Ibssa, CSC 357 
  Assignment 5, High Performance Programming 
  README: (whoever is grading this)
  
  To Christian and Alec, the best Professor + TA Combo Alive. Seriously knowledgeable and down to earth guys. Keep hacking!

  This quarter was awesome. And as I write this last 357 project in a coffee shop, I see how helpful both you have been.

  And I have to be grateful for Zoom. Where would us students be without late night zoom calls? :D

  Best,
   
  Simon
*/
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
#define MATRIX_DIMENSION_XY 10


// generates a random float between user inputted range
float getRandomFloat( float min, float max ) {
  return rand() % 9 + 1;
}

//SEARCH FOR TODO

//************************************************************************************************************************
// sets one element of the matrix
void set_matrix_elem(float *M,int x,int y,float f) {
  M[x + y*MATRIX_DIMENSION_XY] = f;
}
//************************************************************************************************************************

// lets see it both are the same
int quadratic_matrix_compare(float *A,float *B) {
  for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
      for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
        if(A[a +b * MATRIX_DIMENSION_XY]!=B[a +b * MATRIX_DIMENSION_XY]) 
          return 0;
    
  return 1;
}
//************************************************************************************************************************

//print a matrix
void quadratic_matrix_print(float *C) {
  printf("\n");
  for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
      {
      printf("\n");
      for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
          printf("%.2f,",C[a + b* MATRIX_DIMENSION_XY]);
      }
  printf("\n");
}

//************************************************************************************************************************

// multiply two matrices
void quadratic_matrix_multiplication(float *A,float *B,float *C) {
	//nullify the result matrix first
  for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
      for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
          C[a + b*MATRIX_DIMENSION_XY] = 0.0;
  //multiply
  for(int a = 0;a<MATRIX_DIMENSION_XY;a++) // over all cols a
      for(int b = 0;b<MATRIX_DIMENSION_XY;b++) // over all rows b
          for(int c = 0;c<MATRIX_DIMENSION_XY;c++) // over all rows/cols left
              {
                  C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a + c*MATRIX_DIMENSION_XY]; 
              }
}
//************************************************************************************************************************

// multiply two matrices
void quadratic_matrix_multiplication_parallel(float *A,float *B,float *C, int par_id, int par_count) {

  int work = 10 / par_count;
  int start = par_id * work;
  int end = start + work;
	//nullify the result matrix first
  for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
      for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
          C[a + b*MATRIX_DIMENSION_XY] = 0.0;
  //multiply
  for(int a = 0;a<MATRIX_DIMENSION_XY;a++) // over all cols a
      for(int b = start;b<end;b++) // over all rows b
          for(int c = 0;c<MATRIX_DIMENSION_XY;c++) // over all rows/cols left
              {
                  C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a + c*MATRIX_DIMENSION_XY]; 
              }
}


//************************************************************************************************************************
void synch(int par_id,int par_count,int *ready) {
  //TODO: synch algorithm. make sure, ALL processes get stuck here until all ARE here

  int synchId = ready[par_count] + 1;
  ready[par_id] = synchId;

  int myBreak = 0;
  while(1) {
    myBreak = 1;
    for(int i = 0; i < par_count; i++) {
      if(ready[i] < synchId) {
        myBreak = 0;
        break;
      }
    }
    if(myBreak == 1) {
      ready[par_count] = synchId;
      break;
    }

  }
}
//************************************************************************************************************************



int main(int argc, char *argv[]) {
  // to guarantee random floats
  srand(time(0));

  int par_id = 0; // the parallel ID of this process
  int par_count = 1; // the amount of processes
  float *A,*B,*C; //matrices A,B and C
  int *ready; //needed for synch
  if(argc!=3){printf("no shared\n");}
  else
      {
      par_id= atoi(argv[1]);
      par_count= atoi(argv[2]);
    // strcpy(shared_mem_matrix,argv[3]);
      }
  if(par_count==1){printf("only one process\n");}

  int fd[4];
  if(par_id==0)
      {
      //TODO: init the shared memory for A,B,C, ready. shm_open with C_CREAT here! then ftruncate! then mmap
      int size = 100 * sizeof(float);

      int aMatrix = shm_open("matrixA", O_RDWR | O_CREAT, 0777);
      int bMatrix = shm_open("matrixB", O_RDWR | O_CREAT, 0777);
      int cMatrix = shm_open("matrixC", O_RDWR | O_CREAT, 0777);
      int synchObject = shm_open("synchobject", O_RDWR | O_CREAT, 0777);


      ftruncate(aMatrix, size);
      ftruncate(bMatrix, size);
      ftruncate(cMatrix, size);
      ftruncate(synchObject, 11 * sizeof(int));


      A = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, aMatrix, 0);
      B = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, bMatrix, 0);
      C = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, cMatrix, 0);
      ready = (int*)mmap(NULL, sizeof(int) * 11, PROT_READ | PROT_WRITE, MAP_SHARED, synchObject, 0);

      }
  else
      {
    //TODO: init the shared memory for A,B,C, ready. shm_open withOUT C_CREAT here! NO ftruncate! but yes to mmap
      int size = 100 * sizeof(float);

      int aMatrix = shm_open("matrixA", O_RDWR, 0777);
      int bMatrix = shm_open("matrixB", O_RDWR, 0777);
      int cMatrix = shm_open("matrixC", O_RDWR, 0777);
      int synchObject = shm_open("synchobject", O_RDWR, 0777);


      A = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, aMatrix, 0);
      B = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, bMatrix, 0);
      C = (float*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, cMatrix, 0);
      ready = (int*)mmap(NULL, sizeof(int) * 11, PROT_READ | PROT_WRITE, MAP_SHARED, synchObject, 0);


      sleep(2); //needed for initalizing synch
      }



  // Debuggin
  printf("par id: %d | par count: %d\n", par_id, par_count);
  synch(par_id,par_count,ready);

  if(par_id==0)
      {
    //TODO: initialize the matrices A and B
    // initializing matrices a and b to random float
      for(int i = 0; i < MATRIX_DIMENSION_XY; i++) {
        for(int j = 0; j < MATRIX_DIMENSION_XY; j++) {
          set_matrix_elem(A, i, j, getRandomFloat(1.0, 9.0));
          set_matrix_elem(B, i, j, getRandomFloat(1.0, 9.0));
        }
      }
      }



  /* Printing matrix */
  printf("Matrix A: \n");
  printf("[");
  for(int i = 0; i < MATRIX_DIMENSION_XY; i++) {
    for(int j = 0; j < MATRIX_DIMENSION_XY; j++) {
      printf("%.2f ", A[i + j*MATRIX_DIMENSION_XY]);
    }
    printf("\n");
  }
  printf("]\n");
  printf("\n");
  printf("Matrix B: \n");
  printf("[");
  for(int i = 0; i < MATRIX_DIMENSION_XY; i++) {
    for(int j = 0; j < MATRIX_DIMENSION_XY; j++) {
      printf("%.2f ", B[i + j*MATRIX_DIMENSION_XY]);
    }
    printf("\n");
  }
  printf("]\n");


  synch(par_id,par_count,ready);

  quadratic_matrix_multiplication_parallel(A, B, C, par_id, par_count);
    
  synch(par_id,par_count,ready);

  printf("matrix c: [");
  if(par_id==0)
      quadratic_matrix_print(C);
  synch(par_id, par_count, ready);

  printf("]\n");
  close (fd[0]);
  close (fd[1]);
  close (fd[2]);
  close (fd[3]);
  shm_unlink("matrixA");
  shm_unlink("matrixB");
  shm_unlink("matrixC");
  shm_unlink("synchobject");

  //lets test the result:
  float M[MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY];
  quadratic_matrix_multiplication(A, B, M);
  if (quadratic_matrix_compare(C, M)) {
    printf("full points!\n");
  } else {
    printf("buuug!\n");
  }

  return 0;    
}