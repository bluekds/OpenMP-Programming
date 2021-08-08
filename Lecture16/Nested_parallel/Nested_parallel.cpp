#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void main(void) {
	omp_set_nested(1);

	printf("Nested parallelism : %s\n"
		, omp_get_nested() ? "On" : "Off");

	#pragma omp parallel num_threads(4)
	{
		int parentID = omp_get_thread_num();
		printf("Lv 1 - Thread %d\n", parentID);

		#pragma omp barrier

		#pragma omp parallel num_threads(2)
		{
			printf("\tLv 2 - Thread %d of %d\n"
				, omp_get_thread_num(), parentID);
		}
	}

}