#include <stdio.h>
#include <omp.h>

void run(bool _parallel, int _numThreads)
{
	#pragma omp parallel num_threads(_numThreads) if(_parallel)
	{
		printf("I'm thread %d out of %d thread(s)\n"
			,omp_get_thread_num(), omp_get_num_threads());
	}
}

int main()
{
	printf("-- run(true, 4) --\n");
	run(true, 4);
	printf("-- run(false, 4) --\n");
	run(false, 4);
}
