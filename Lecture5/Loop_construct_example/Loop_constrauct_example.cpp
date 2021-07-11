#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
	#pragma omp parallel num_threads(4)
	{
		#pragma omp for
		for (int i = 0; i < 8; i++) {
			printf("[Thread %d] executes loop iteration %d\n", omp_get_thread_num(), i);
		}
	}

	return 0;
}
