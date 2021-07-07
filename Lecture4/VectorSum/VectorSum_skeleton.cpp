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


	timer.onTimer(0);
	printf("Start the serial algorithm\n");
	/** Serial algorithm의 결과는 c_serial 배열에 저장 **/
	/** 여기에 serial algorithm 작성 **/

	timer.offTimer(0);



	timer.onTimer(1);
	printf("Start the parallel algorithm (%d threads)\n", numThreads);
	/** parallel algorithm의 결과는 c_parallel 배열에 저장 **/
	/** 여기에 parallel algorithm 작성 **/

	timer.offTimer(1);

	
	// Check the results
	bool isCorrect = true;
	for (int i = 0; i < VECTOR_SIZE; i++) {
		if (c_serial[i] != c_parallel[i]) {
			isCorrect = false;
			break;
		}
	}
	(isCorrect ? printf("Results are matched! :)\n") : printf("Results are not matched :(\n"));

	// Print processing times
	timer.printTimer();

	return 0;
}
