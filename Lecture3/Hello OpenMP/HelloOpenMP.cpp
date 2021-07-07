#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // OpenMP ���̺귯�� �Լ� ����� ���� ��� ����

int main(void) {

	#pragma omp parallel num_threads(4)
	{
		printf("[Thread %d] Hello OpenMP!\n", omp_get_thread_num());
	}
	return 0;
}
