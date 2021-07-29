#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#ifdef _WIN32
#include <Windows.h>
#else 
#include <unistd.h>
#define Sleep(x) sleep(x)
#endif


int main()
{
	omp_lock_t lockVar;
	omp_init_lock(&lockVar);
	
	#pragma omp parallel num_threads(2) shared(lockVar)
	{
		int tID = omp_get_thread_num();

		if (tID == 0)
			omp_set_lock(&lockVar);
		#pragma omp barrier

		if (tID == 0) {
			printf("T1 process task A\n");
			Sleep(10);
			omp_unset_lock(&lockVar);
		}
		else {
			omp_set_lock(&lockVar);
			printf("T2 process task B\n");
			omp_unset_lock(&lockVar);
		}
	}

	omp_destroy_lock(&lockVar);
	
	return 0;
}