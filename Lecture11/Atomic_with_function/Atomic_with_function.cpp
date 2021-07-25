#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int ic2;
int func() {
	ic2 += 1;
	return 1;
}

int main()
{
	ic2 = 0;
	int ic = 0;
	#pragma omp parallel num_threads(4)
	{
		for (int i = 0; i < 10240; i++)
		{
			#pragma omp atomic
			ic += func();

			//#pragma omp critical
			//{
			//	ic += func();
			//}
		}
	}

	printf("ic = %d, ic2 = %d\n", ic, ic2);

	return 0;
}

