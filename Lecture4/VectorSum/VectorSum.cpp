#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "DS_timer.h"

#define VECTOR_SIZE (64*1024*1024)
#define numThreads (4)

int main(void)
{
	// Timer setup
	DS_timer timer(2);
	timer.setTimerName(0, (char*)"Serial algorithm");
	timer.setTimerName(1, (char*)"Parallel algorithm");
	timer.initTimers();

	// Make input vectors
	int* a = new int[VECTOR_SIZE];
	memset(a, 0, sizeof(int) * VECTOR_SIZE);
	int* b = new int[VECTOR_SIZE];
	memset(b, 0, sizeof(int) * VECTOR_SIZE);
	int* c_serial = new int[VECTOR_SIZE];
	memset(c_serial, 0, sizeof(int) * VECTOR_SIZE);
	int* c_parallel = new int[VECTOR_SIZE];
	memset(c_parallel, 0, sizeof(int) * VECTOR_SIZE);

	for (int i = 0; i < VECTOR_SIZE; i++) {
		a[i] = (int)(rand() & 0xFF);
		b[i] = (int)(rand() & 0xFF);
	}


	//****** Serial algorithm
	printf("Start the serial algorithm\n");

	timer.onTimer(0);

	for (int i = 0; i < VECTOR_SIZE; i++)
		c_serial[i] = a[i] + b[i];

	timer.offTimer(0);


	//****** Parallel algorithm
	printf("Start the parallel algorithm (%d threads)\n", numThreads);
	timer.onTimer(1);

	int numElePerThread = VECTOR_SIZE / numThreads;
	int start[numThreads] = { 0 };
	int end[numThreads] = { 0 };

	// 스레드별 작업 영역 지정
	for (int tID = 0; tID < numThreads; tID++) {
		start[tID] = numElePerThread * tID;
		end[tID] = numElePerThread * (tID + 1);

		if (tID == numThreads - 1) // for the last thread
			end[numThreads - 1] = VECTOR_SIZE;

		//printf("\t[T%d] %d ~ %d\n", tID, start[tID], end[tID]);
	}

	// 병렬처리 영역, 각 스레드는 할당된 영역에 대한 합 계산 수행
	#pragma omp parallel num_threads(numThreads)
	{
		int tID = omp_get_thread_num();
		for (int i = start[tID]; i < end[tID]; i++)
			c_parallel[i] = a[i] + b[i];
	}

	timer.offTimer(1);


	// Check the results
	bool isCorrect = true;
	for (int i = 0; i < VECTOR_SIZE; i++) {
		if (c_serial[i] != c_parallel[i]) {
			printf("%d ", i);
			isCorrect = false;
			break;
		}
	}
	(isCorrect ? printf("Results are matched! :)\n") : printf("Results are not matched :(\n"));

	// Print processing times
	timer.printTimer();

	delete[] a;
	delete[] b;
	delete[] c_serial;
	delete[] c_parallel;
	return 0;
}