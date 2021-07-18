#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void main(void)
{
	int a = 0;
	int result[4] = { 0 };

	#pragma omp parallel for num_threads(4)
	for (int i = 0; i < 4; i++) {
		a = a + i;
		a = a * a;

		result[i] = a;
	}

	for (int i = 0; i < 4; i++)
		printf("result[%d] a = %d\n", i, result[i]);
}
