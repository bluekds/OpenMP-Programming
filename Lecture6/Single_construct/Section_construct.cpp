#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void){
	#pragma omp parallel num_threads(4)
	{
		printf("Thread %d is ready\n", omp_get_thread_num());
		#pragma omp barrier

		#pragma omp sections
		{
			#pragma omp section // section A
			{
				printf("Section A is executed by thread %d\n"
					, omp_get_thread_num());
			}

			#pragma omp section  // section B
			{
				printf("Section B is executed by thread %d\n"
					, omp_get_thread_num());
			}
		}
	}
}
