#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "DS_timer.h"

#define NUM_ELEMENTS (1024*1024*1024)
#define NUM_BINS (10)
#define NUM_THREADS (4)
#define LOCAL_BIN_SIZE (128)

enum Algorithm {Serial, Parallel_critical, Paralle_lock4eachBin
	, Parallel_localBin, Parallel_localBin_wo_lock
	, Parallel_reduction, Parallel_reductionV2
	, END};

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
void histogram_Parallel_critical(float* _input, int* _output);
void histogram_Paralle_lock4eachBin(float* _input, int* _output);
void histogram_Parallel_localBin(float* _input, int* _output);
void histogram_Parallel_localBin_wo_lock(float* _input, int* _output);
void histogram_Parallel_reduction(float* _input, int* _output);
void histogram_Parallel_reductionV2(float* _input, int* _output);

int main(void)
{
	DS_timer timer(END);
	timer.initTimers();

	float* data = new float[NUM_ELEMENTS];
	int bins[Algorithm::END][NUM_BINS] = { 0 };
	genData(data);

	RUN_TEST(Serial);
	RUN_TEST(Parallel_critical);
	RUN_TEST(Paralle_lock4eachBin);
	RUN_TEST(Parallel_localBin);
	RUN_TEST(Parallel_localBin_wo_lock);
	RUN_TEST(Parallel_reduction);
	RUN_TEST(Parallel_reductionV2);

	timer.printTimer();
	delete[] data;

	return 0;
}

void histogram_Serial(float* _input, int* _output)
{
	for(int i = 0 ; i < NUM_ELEMENTS ; i++)
		_output[(int)_input[i]]++;
}

void histogram_Parallel_critical(float* _input, int* _output)
{
	#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 0; i < NUM_ELEMENTS; i++)
	{
		#pragma omp critical
		{
			_output[(int)_input[i]]++;
		}
	}
}

void histogram_Paralle_lock4eachBin(float* _input, int* _output) {
	omp_lock_t locks[NUM_BINS];
	for(int i = 0 ; i < NUM_BINS; i++)
		omp_init_lock(&locks[i]);

	#pragma omp parallel for num_threads(NUM_THREADS)
	for (int i = 0; i < NUM_ELEMENTS; i++)
	{
		int binID = (int)_input[i];
		omp_set_lock(&locks[binID]);
		_output[binID]++;
		omp_unset_lock(&locks[binID]);
	}

	for (int i = 0; i < NUM_BINS; i++)
		omp_destroy_lock(&locks[i]);
}

void histogram_Parallel_localBin(float* _input, int* _output) {
	
	int localBins[NUM_THREADS][LOCAL_BIN_SIZE] = { 0 };

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tID = omp_get_thread_num();
	//	int localBin[NUM_BINS] = { 0 };

		#pragma omp for
		for(int i = 0 ; i < NUM_ELEMENTS ; i++)
			localBins[tID][(int)_input[i]]++;

		#pragma omp for
		for (int i = 0; i < NUM_THREADS; i++)
			for(int j = 0 ; j < NUM_BINS ; j++)
				#pragma omp atomic
				_output[j] += localBins[tID][j];
	}
}

void histogram_Parallel_localBin_wo_lock(float* _input, int* _output) {

	// To avoid the cache coherency issue, we set the local bin size to 32 (> NUM_bins)
	int localBins[NUM_THREADS][LOCAL_BIN_SIZE] = { 0 };

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tID = omp_get_thread_num();

		#pragma omp for
		for (int i = 0; i < NUM_ELEMENTS; i++)
			localBins[tID][(int)_input[i]]++;

		#pragma omp for
		for (int i = 0; i < NUM_BINS ; i++)
			for (int j = 0 ; j < NUM_THREADS; j++)
				_output[i] += localBins[j][i];
	}
}

void histogram_Parallel_reduction(float* _input, int* _output) {

	// To avoid the cache coherency issue, we set the local bin size to 32 (> NUM_bins)
	int localBins[NUM_THREADS][LOCAL_BIN_SIZE] = { 0 };

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tID = omp_get_thread_num();

		#pragma omp for
		for (int i = 0; i < NUM_ELEMENTS; i++)
			localBins[tID][(int)_input[i]]++;

		// global sum
		int offset = 1;
		while (offset < NUM_THREADS)
		{
			for (int i = 0; i < NUM_BINS; i++)
			{
				if (tID % (2 * offset) == 0)
					localBins[tID][i] += localBins[tID + offset][i];
			}

			#pragma omp barrier

			offset *= 2;
		}
	}

	for (int i = 0; i < NUM_BINS; i++)
		_output[i] = localBins[0][i];
}

void histogram_Parallel_reductionV2(float* _input, int* _output) {
	// To avoid the cache coherency issue, we set the local bin size to 32 (> NUM_bins)
	int localBins[NUM_THREADS][LOCAL_BIN_SIZE] = { 0 };

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tID = omp_get_thread_num();

		#pragma omp for
		for (int i = 0; i < NUM_ELEMENTS; i++)
			localBins[tID][(int)_input[i]]++;

		// global sum
		int offset = 1;
		while (offset < NUM_THREADS)
		{
			if (tID % (2 * offset) == 0)
				for (int i = 0; i < NUM_BINS; i += 2)
					localBins[tID][i] += localBins[tID + offset][i];

			if (tID % (2 * offset) == (offset * 2 - 1))
				for (int i = 1; i < NUM_BINS; i += 2)
					localBins[tID][i] += localBins[tID - offset][i];

			#pragma omp barrier

			offset *= 2;
		}
	}

	for (int i = 1; i < NUM_BINS; i += 2)
	{
		_output[i-1] = localBins[0][i-1];
		_output[i] = localBins[NUM_THREADS - 1][i];
	}
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
	for(int i= 0 ; i < NUM_BINS ; i++)
		printf("%d ", _bins[i]);
	printf("\n");
}