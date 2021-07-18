#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void main(void) {
	int tid = -1;
	int priVar = -1;

	#pragma omp parallel for num_threads(4) private(tid) firstprivate(priVar)
	for (int i = 0; i < 4; i++) {
		tid = omp_get_thread_num();
		priVar = priVar * tid * 10;
		printf("[Thread %d] priVar = %d\n", tid, priVar);
	}
}