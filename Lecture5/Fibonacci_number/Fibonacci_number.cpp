#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


int main(void)
{
	int fibo[10];
	fibo[0] = 0;
	fibo[1] = 1;

	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 2; i < 10; i++) {
			fibo[i] = fibo[i - 1] + fibo[i - 2];
		}
	}

	for (int i = 0; i < 10; i++)
		printf("%d ", fibo[i]);
	printf("\n");

	return 0;
}
