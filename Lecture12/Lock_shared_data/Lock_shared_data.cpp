#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
	int sharedData = 0;

	omp_lock_t lockVar;
	omp_init_lock(&lockVar);

	#pragma omp parallel num_threads(4)
	{
		for (int i = 0; i < 10240; i++)
		{
			omp_set_lock(&lockVar);
			sharedData = sharedData + 1;
			omp_unset_lock(&lockVar);
		}
	}

	printf("sharedData = %d\n", sharedData);

	return 0;
}

