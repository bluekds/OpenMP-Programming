#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
	int a[10] = { 0 };
	int b[10] = { 0 };

	#pragma omp parallel num_threads(4)
	{
		#pragma omp for
		for (int i = 0; i < 10; i++)
			a[i] = i;

		// implicit barrier here

		#pragma omp for
		for (int i = 0; i < 10; i++)
			b[i] = 2 * a[(i+1)%10];
	}


	printf("a = ");
	for (int i = 0; i < 10; i++)
		printf("%d ", a[i]);
	printf("\n");

	printf("b = ");
	for (int i = 0; i < 10; i++)
		printf("%d ", b[i]);
	printf("\n");

	return 0;
}
