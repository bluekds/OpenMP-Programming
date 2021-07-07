#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // OpenMP 라이브러리 함수 사용을 위한 헤더 파일

int main(void) {

	#pragma omp parallel num_threads(4)
	{
		printf("[Thread %d] Hello OpenMP!\n", omp_get_thread_num());
	}
	return 0;
}
