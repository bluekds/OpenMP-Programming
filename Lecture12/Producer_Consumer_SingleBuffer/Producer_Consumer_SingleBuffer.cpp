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
	int buf = 0; // 0: empty, otherwise: full

	omp_lock_t lock;
	omp_init_lock(&lock);

	bool isFinish = false;

	#pragma omp parallel sections shared(isFinish, lock) num_threads(2)
	{
		#pragma omp section // Producer
		{
			int numProduce = 10;
			while (numProduce > 1) {
				omp_set_lock(&lock);
				if (buf == 0) {
					numProduce--;
					buf = numProduce;
					printf("Producer puts the job-%d\n", buf);
				}
				omp_unset_lock(&lock);
				Sleep(500);
			}
			isFinish = true;
		}

		#pragma omp section // Consumer
		{
			int get = 0;
			while (!isFinish) {
				omp_set_lock(&lock);
				if (buf > 0) {
					get = buf;
					buf = 0;
					printf("Consumer takes the job-%d\n", get);
				}
				omp_unset_lock(&lock);
				Sleep(500);
			}
		}
	}
	omp_destroy_lock(&lock);
	printf("Finished!\n");
}