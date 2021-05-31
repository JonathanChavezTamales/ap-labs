#include <stdio.h>
#include <omp.h>

static long num_steps = 1000000;
double step;
int threads = 4;

int main(){
	double start_time, run_time;
	double x, pi, sum = 0;
	double sums[threads];

	step = 1.0/(double) num_steps;
	start_time = omp_get_wtime();

	#pragma omp parallel num_threads(threads)
	{
		int thread = omp_get_thread_num();
		int start = num_steps/threads * (thread);
		int end = start + num_steps/threads;
		
		if(thread != 0) start += 1;

		printf("thread %i: [%i, %i]\n", thread, start, end);

		for(int i=start; i<=end; i++){
			x = (i - 0.5) * step;
			sums[thread] += 4.0/(1.0 + x * x);
		}
	}

	for(int i=0; i<threads; i++) sum += sums[i];

	pi = step * sum;
    	run_time = omp_get_wtime() - start_time;
    	printf("\n pi with %d steps is %f in %f seconds ",num_steps,pi,run_time);


	return 0;
}
