#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 8
#define NUM_THREAD 8

void main(void)
{
	int a[ARRAY_SIZE] = { 0 };
	for (int i = 0; i < ARRAY_SIZE; i++)
		a[i] = rand() % 10;

	for (int i = 0; i < ARRAY_SIZE; i++)
		printf("%d\t", a[i]);
	printf("\n");

	int sum = 0;
	#pragma omp parallel for reduction(+:sum) num_threads(NUM_THREAD)
	for (int i = 0 ; i < ARRAY_SIZE ; i++) {
		sum += a[i];
	}

	printf("Sum = %d\n", sum);
}