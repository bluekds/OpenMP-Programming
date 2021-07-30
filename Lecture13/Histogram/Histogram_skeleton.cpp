#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "DS_timer.h"

#define NUM_ELEMENTS (1024*1024*1024)
#define NUM_BINS (10)
#define NUM_THREADS (4)

enum Algorithm {
	Serial, Parallel
	, END
};

#define RUN_TEST(_func) { \
	timer.setTimerName(Algorithm::_func, (char*)#_func);	\
	timer.onTimer(Algorithm::_func); \
	histogram_##_func(data, bins[Algorithm::_func]);		\
	timer.offTimer(Algorithm::_func); \
	printf("%s :\t", (char*)#_func);	\
	printResults(bins[Algorithm::_func]);	\
}

float getRandNum();
void genData(float* _data);
void printResults(int* _bins);

void histogram_Serial(float* _input, int* _output);
void histogram_Parallel(float* _input, int* _output);

int main(void)
{
	DS_timer timer(END);
	timer.initTimers();

	float* data = new float[NUM_ELEMENTS];
	int bins[Algorithm::END][NUM_BINS] = { 0 };
	genData(data);

	RUN_TEST(Serial);
	RUN_TEST(Parallel);

	timer.printTimer();
	delete[] data;

	return 0;
}

void histogram_Serial(float* _input, int* _output)
{
	// Write the serial algorithm here
}

void histogram_Parallel(float* _input, int* _output)
{
	// Write your parallel algorithm here
}



// Utility functions
float getRandNum()
{
	return (rand() % NUM_BINS + (rand() % 100) / 100.0f);
}

void genData(float* _data)
{
#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 0; i < NUM_ELEMENTS; i++)
		_data[i] = getRandNum();
}

void printResults(int* _bins)
{
	for (int i = 0; i < NUM_BINS; i++)
		printf("%d ", _bins[i]);
	printf("\n");
}