#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
	int ic = 0;

	#pragma omp parallel num_threads(4)
	{
		for (int i = 0; i < 10240; i++)
		{
			#pragma omp atomic
			ic += 1;
		}
	}

	printf("count = %d\n", ic);

	return 0;
}

