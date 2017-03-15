#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Lab3IO.h"

#include "timer.h"

int main(int argc, char *argv[]) 
{
    int i, j, k, size;
    double** Au;
    double* X;
    double temp;
    int* index;
    //FILE* fp;
    double start, end;
    int thread_count=0;

    int largest;
    int largest_index;
    /*Load the datasize and verify it*/
    Lab3LoadInput(&Au, &size);

    thread_count = atoi(argv[1]);

    /*Calculate the solution by parallel code*/
    X = CreateVec(size);
    index = malloc(size * sizeof(int));

    GET_TIME(start);
    // # pragma omp parallel for num_threads(thread_count)
    for (i = 0; i < size; ++i)
        index[i] = i;

    if (size == 1)
        X[0] = Au[0][1] / Au[0][0];
    else
    {
        /*Gaussian elimination*/
        for (k = 0; k < size - 1; ++k){
            /*Pivoting*/
            j = 0;
   	    double largest;
	    int largest_index;
            #pragma omp parallel num_threads(thread_count) \
		shared(k,largest,largest_index,index,i) private(temp,j)
            {
		largest =0;
		largest_index = 0;
		j = 0;	
		temp =0;
                #pragma omp for
                for (i = k; i < size; ++i)
                {
                    if (temp < Au[index[i]][k] * Au[index[i]][k])
                    {
                        temp = Au[index[i]][k] * Au[index[i]][k];
                        j = i;
                    }  
                }
                if (Au[index[j]][k]*Au[index[j]][k] > largest)/*swap*/
		#pragma critical 	
                {
			if(Au[index[j]][k]*Au[index[j]][k] > largest )
			{
				largest = Au[index[j]][k]*Au[index[j]][k];
				largest_index = j;
			}
                }
		#pragma omp barrier

		#pragma omp single
		{
		if (largest_index != k){
			i = index[largest_index];
			index[largest_index] = index[k];
			index[k] = i;
		}
		}

                /*calculating*/
                #pragma omp for private(temp, j)
                for (i = k + 1; i < size; ++i)
                {
                    temp = Au[index[i]][k] / Au[index[k]][k];
                    for (j = k; j < size + 1; ++j)
                        Au[index[i]][j] -= Au[index[k]][j] * temp;
                }  
            }     
        }

        /*Jordan elimination*/
        for (k = size - 1; k > 0; --k)
        {
            #pragma omp parallel for num_threads(thread_count) private(temp)
            for (i = k - 1; i >= 0; --i )
            {
                temp = Au[index[i]][k] / Au[index[k]][k];
                Au[index[i]][k] -= temp * Au[index[k]][k];
                Au[index[i]][size] -= temp * Au[index[k]][size];
            } 
        }
        /*solution*/
        #pragma omp parallel for num_threads(thread_count)
        for (k=0; k< size; ++k)
        {
            X[k] = Au[index[k]][size] / Au[index[k]][k];
        }
    }    


    GET_TIME(end);

    Lab3SaveOutput(X,size,end-start);
    DestroyVec(X);
    DestroyMat(Au, size);
    free(index);
    return 0;
}

