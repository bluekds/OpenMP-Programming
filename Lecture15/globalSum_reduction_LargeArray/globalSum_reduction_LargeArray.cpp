#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 100
#define NUM_THREAD 8

void printArray(int* _array, int size = ARRAY_SIZE);

void main(void)
{
	int a[ARRAY_SIZE] = { 0 };
	for (int i = 0; i < ARRAY_SIZE; i++)
		a[i] = rand() % 10;
	printArray(a);

	int localSum[NUM_THREAD] = { 0 };

	#pragma omp parallel num_threads(NUM_THREAD)
	{
		int tID = omp_get_thread_num();
		
		#pragma omp for
		for (int i = 0; i < ARRAY_SIZE; i++)
			localSum[tID] += a[i];

		/* For printing intermediate status */
		//#pragma omp master
		//{
		//	for (int i = 0; i < NUM_THREAD; i++)
		//		printf("%d ", localSum[i]);
		//	printf("\n");
		//}
		//#pragma omp barrier

		int offset = 1;
		while (offset < NUM_THREAD) {
			if (tID % (2 * offset) == 0)
				localSum[tID] += localSum[tID + offset];

			#pragma omp barrier

			/* For printing intermediate status */
			//#pragma omp master
			//{
			//	for (int i = 0; i < NUM_THREAD; i++)
			//		printf("%d ", localSum[i]);
			//	printf("\n");
			//}
			//#pragma omp barrier

			offset *= 2;
		}
	}

	printf("Sum = %d\n", localSum[0]);

	//printArray(localSum, NUM_THREAD);
}

void printArray(int* _array, int _size)
{
	for (int i = 0; i < _size; i++)
		printf("%d ", _array[i]);
	printf("\n");
}