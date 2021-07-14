#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 6
float computeVal(int i) { return (float)i; }

int main(void)
{
	float sum = 0;
	float b[10] = { 0.f };

	#pragma omp parallel num_threads(4)
	{
		#pragma omp parallel for
		for (int i = 0; i < N; i++)
			b[i] = computeVal(i);

		#pragma omp single
		{
			for (int i = 0; i < N; i++)
				sum += b[i];
		}

		#pragma omp for
		for (int i = 0; i < N; i++)
			b[i] = b[i]/sum;
	}

	sum = 0.f;
	for (int i = 0; i < N; i++) {
		printf("b[%d] = %.2f\n", i, b[i]);
		sum += b[i];
	}
	printf("Sum of b[] = %.2f\n", sum);
}
