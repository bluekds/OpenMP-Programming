#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void main()
{
	#pragma omp parallel for schedule(static, 2) num_threads(3)
	for (int i = 0; i < 12; i++)
	{
		int tID = omp_get_thread_num();
		printf("[%d] by thread %d\n", i, tID);
	}
}