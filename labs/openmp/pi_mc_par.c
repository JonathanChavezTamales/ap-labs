#include <stdio.h>
#include <omp.h>
#include "random.h"

static long num_trials = 1000000;
int threads = 4;

int main (){
    long i;  long NcircArr[threads]; 
    long Ncirc = 0;
    double pi, x, y, test;
    double r = 1.0;   // radius of circle. Side of squrare is 2*r

    seed(-r, r);  // The circle and square are centered at the origin

	#pragma omp parallel num_threads(threads) private(i)
    	{
	NcircArr[omp_get_thread_num()] = 0;
    	for(i=0;i<num_trials/threads; i++)
	{
	    x = trandom();
	    y = trandom();

	    test = x*x + y*y;


	    if (test <= r*r) NcircArr[omp_get_thread_num()]++;
	}
	}


    pi = 4.0 * ((double)Ncirc/(double)num_trials);

    printf("\n %d trials, pi is %f \n",num_trials, pi);

    return 0;
}

