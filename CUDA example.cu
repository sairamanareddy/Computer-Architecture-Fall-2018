#define N 512
#define NUM_BLOCKS 16
#define NUM_THREADS 48
//Do not change above three lines.

//Submission should be named as  <RollNumber>_Prog.cu
//Upload just this cu file and nothing else. If you upload it as a zip, it will not be evaluated. 

/*Remember the following guidelines to avoid losing marks
This exercise is quite simple. The only tricky part is that total number of threads (NUM_BLOCKS*NUM_THREADS) may be different (higher or lower) from N.

Index of an array should not exceed the array size. 

No output array-element should be computed more than once

No marks will be given if the program does not compile or run (TAs will not debug your program at all)

Do not change the name of any variable that we have introduced. 
*/


#include <stdio.h>
#include<stdlib.h>
//TODO: WRITE GPU KERNEL. It should not be called repeatedly from the host, but just once. Each time it is called, it may process more than array-element or not process any array-element at all. 
__global__ void add(int* d_A, int* d_B, int* d_C){
	int tid = NUM_THREADS * blockIdx.x + threadIdx.x;
	while(tid < N){
		for(int j=0;j<N;j++){
		if(tid < N) d_C[tid+j*N]=d_A[tid+j*N]+d_B[tid+j*N];
		}
		tid+=NUM_THREADS*NUM_BLOCKS;
	}
}
int main (int argc, char **argv) {
  int A[N][N], B[N][N], C[N][N];
  int *d_A, *d_B, *d_C; // These are the copies of A, B and C on the GPU
  int *h_C;       // This is a host copy of the output of B from the GPU
  int i, j;
h_C = (int *)malloc(N*N*sizeof(int));	//Allocate memory for the host copy of the output.
  
  for(i=0;i<N;i++) {
    for(j=0;j<N;j++) {
      A[i][j] = i+j;
      B[i][j]= 2*j-1;      
    }
  }

  // sequential implementation of main computation
  for(i=0;i<N;i++) {
    for(j=0;j<N;j++) {
      C[i][j] = A[i][j]+B[i][j];
    }
  }

  // TODO: ALLOCATE MEMORY FOR GPU COPIES OF d_A, d_B and d_C
  cudaMalloc((void**)&d_A, N*N*sizeof(int));
  cudaMalloc((void**)&d_B, N*N*sizeof(int));
  cudaMalloc((void**)&d_C, N*N*sizeof(int));
  // TODO: COPY A TO d_A
    cudaMemcpy(d_A, A, N*N*sizeof(int), cudaMemcpyHostToDevice);
  // TODO: COPY B TO d_B
    cudaMemcpy(d_B, B, N*N*sizeof(int), cudaMemcpyHostToDevice);
  // TODO: CREATE BLOCKS with THREADS AND INVOKE GPU KERNEL
   //Use NUM_BLOCKS blocks, each with NUM_THREADS  threads
      add<<<NUM_BLOCKS, NUM_THREADS>>>(d_A, d_B, d_C);
  // TODO: COPY d_C BACK FROM GPU to CPU in variable h_C
      cudaMemcpy(h_C, d_C, N*N*sizeof(int), cudaMemcpyDeviceToHost);

  // TODO: Verify result is correct by comparing
  for(i=0;i<N;i++) {
    for(j=0;j<N;j++) {
    //TODO: compare each element of h_C and C by subtracting them
        //print only those elements for which the above subtraction is non-zero
	if(h_C[i*N+j]!=C[i][j]){
		printf("ERROR!h_C[%d]=%d!=C[%d][%d]=%d\n", i*N+j, h_C[i*N+j], i, j, C[i][j]);
	}
    }
   }
    //IF even one element of h_C and C differ, report an error.
    //Otherwise, there is no error.
    //If your program is correct, no error should occur.
	free(h_C);cudaFree(d_A);cudaFree(d_B);cudaFree(d_C);
}


