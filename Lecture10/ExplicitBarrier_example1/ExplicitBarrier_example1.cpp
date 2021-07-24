#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#ifdef _WIN32
	#include <Windows.h>
#else 
	#include <unistd.h>
	#define Sleep(x) sleep(x)
#endif

int main(void) {
	int tID = 0;
	#pragma omp parallel private (tID)
	{
		tID = omp_get_thread_num();

		if (tID % 2 == 0) Sleep(10);
		printf("[%d] before\n", tID);

		//#pragma omp barrier

		printf("[%d] after\n", tID);
	}
	
	return 0;
}