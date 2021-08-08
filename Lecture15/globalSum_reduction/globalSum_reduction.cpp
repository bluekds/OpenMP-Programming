#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void main(void)
{
	int a[8] = { 0 };
	for (int i = 0; i < 8; i++)
		a[i] = rand() % 10;
	
	for (int i = 0; i < 8; i++)
		printf("%d\t", a[i]);
	printf("\n");

	#pragma omp parallel num_threads(8)
	{
		int tID = omp_get_thread_num();

		int offset = 1;
		while ( offset < 8){
			if (tID % (2 * offset) == 0)
				a[tID] += a[tID + offset];

			#pragma omp barrier

			/* For printing intermediate status */
			//#pragma omp master
			//{
			//	for (int i = 0; i < ARRAY_SIZE; i++)
			//		printf("%d\t", a[i]);
			//	printf("\n");
			//}
			//#pragma omp barrier

			offset *= 2;
		}
	}

	for (int i = 0; i < 8; i++)
		printf("%d\t", a[i]);
	printf("\n");
}