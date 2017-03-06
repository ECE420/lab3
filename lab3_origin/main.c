//Remember to change the  value of the wrong case to -1 !!!
/*
Test the result stored in the "data_output" by a serial version of calculation

-----
Compiling:
    "Lab3IO.c" should be included and "-lm" tag is needed, like
    > gcc serialtester.c Lab3IO.c -o serialtester -lm
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Lab3IO.h"
#include <omp.h>
#include "timer.h"
#define TOL 0.0005

int main(int argc, char* argv[])
{
	int i, j, k, size;
	double** Au;
	double* X;
	double temp, error, Xnorm;
	int* index;
	double start,end;
	FILE* fp;	
	
	int thread_count = strtol(argv[1], NULL, 10);


	/*Load the datasize and verify it*/
	Lab3LoadInput(&Au, &size);
	if ((fp = fopen("data_output","r")) == NULL){
		printf("Fail to open the result data file!\n");
		return 2;
	}
	fscanf(fp, "%d\n\n", &i);
	if (i != size){
		printf("The problem size of the input file and result file does not match!\n");
		return -1;
	}
	/*Calculate the solution by serial code*/
	X = CreateVec(size);
    index = malloc(size * sizeof(int));
    GET_TIME(start);
    # pragma omp parallel for num_threads(thread_count) 
    for (i = 0; i < size; ++i)
        index[i] = i;

    if (size == 1)
        X[0] = Au[0][1] / Au[0][0];
    else{
        /*Gaussian elimination*/
        for (k = 0; k < size - 1; ++k){
            /*Pivoting*/
            temp = 0;
	    //# pragma omp parallel for num_threads(thread_count) \
		default(none) shared(k,j) private(i,temp)
//		int lp;
//		int largest = 0;
//		#pragma omp parallel private(lp)
//		{
//		    lp = 0;j = 0;
//		    #pragma omp for
//		    for (i = k; i < size; ++i)
//			if (temp < Au[index[i]][k] * Au[index[i]][k]){
//			    temp = Au[index[i]][k] * Au[index[i]][k];
//			    lp = i;
//			}
//		    printf("the %d thread, has lp = %d \n",omp_get_thread_num, lp);
//		    if(Au[index[lp]][k] > largest){
//			#pragma critical
//			{
//		 	     if (Au[index[lp]][k] > largest){
//				 largest = Au[index[lp]][k];	
//				 j = lp;
//		    		 printf("the %d thread, has lp = %d, max is %d \n",omp_get_thread_num, lp, largest);
//			     }
//			}
//		    }
//		}
		
/* origin find max*/
		j=0;
		for (i = k; i<size; ++i)	
			if(temp < Au[index[i]][k]*Au[index[i]][k]){
				temp = Au[index[i]][k] *Au[index[i]][k];
				j = i;
			}
		if (j != k) {
			i = index[j];
			index[j] = index[k];
			index[k] = i;	
		}

/* end of origin find*/
		if (j != k)/*swap*/{
			i = index[j];
		    index[j] = index[k];
		    index[k] = i;
		}
		    /*calculating*/
		   # pragma omp parallel for num_threads(thread_count) private(temp, j)  
		    for (i = k + 1; i < size; ++i){
			temp = Au[index[i]][k] / Au[index[k]][k];
			
			for (j = k; j < size + 1; ++j)
			    Au[index[i]][j] -= Au[index[k]][j] * temp;
		    }       
        }
        /*Jordan elimination*/
        for (k = size - 1; k > 0; --k){
            for (i = k - 1; i >= 0; --i ){
                temp = Au[index[i]][k] / Au[index[k]][k];
                Au[index[i]][k] -= temp * Au[index[k]][k];
                Au[index[i]][size] -= temp * Au[index[k]][size];
            } 
        }
        /*solution*/
	# pragma omp parallel for num_threads(thread_count)   
        for (k=0; k< size; ++k)
            X[k] = Au[index[k]][size] / Au[index[k]][k];
    }
    GET_TIME(end);
    printf("time is %f", end-start);
    Lab3SaveOutput(X, size, end-start);
    DestroyVec(X);
    DestroyMat(Au, size);
    free(index);
    return 0;	
}
