#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#ifdef _WIN32
#include <Windows.h>
#else 
#include <unistd.h>
#define Sleep(x) sleep(x)
#endif

void main()
{
	#pragma omp parallel for schedule(guided) num_threads(3)
	for (int i = 0; i < 12; i++)
	{
		int tID = omp_get_thread_num();
		printf("[%d] by thread %d\n", i, tID);
		Sleep(1);
	}
}
