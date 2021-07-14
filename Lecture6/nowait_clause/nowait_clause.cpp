#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int computeValA(int i) { return i; }
int computeValB(int i) { return i*2; }

int main(void)
{
	int a[10] = { 0 };
	int b[10] = { 0 };

	#pragma omp parallel num_threads(4)
	{
		#pragma omp parallel for
		for (int i = 0; i < 10; i++)
			a[i] = computeValA(i);

		#pragma omp single nowait
		{
			printf("Work A was finished!\n");
		}

		#pragma omp for
		for (int i = 0; i < 10; i++)
			b[i] = computeValB(i);
	}

	printf("Work B was finished\n");
}
