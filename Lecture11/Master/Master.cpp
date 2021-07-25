#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#ifdef _WIN32
#include <Windows.h>
#else 
#include <unistd.h>
#define Sleep(x) sleep(x)
#endif

int getOrder() {
	Sleep(10);
	return rand() % 4 + 1;
}

int main(void) {
	int order[4] = { 0 };
	#pragma omp parallel num_threads(4)
	{
		int isMaster = 0;
		int tID = omp_get_thread_num();

		#pragma omp master
		{
			isMaster = 1;
			for (int i = 0; i < 4; i++)
				order[i] = getOrder();
		}
		#pragma omp barrier

		if (!isMaster)
			printf("Thread-%d cooks %d\n", tID, order[tID]);
	}

	return 0;
}