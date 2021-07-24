#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
    int a[4] = { 0 };
    int b[16] = { 0 };

    #pragma omp parallel num_threads(4)
    {
        int tID = omp_get_thread_num();
        a[tID] = tID * 10;

        //#pragma omp barrier

        #pragma omp for
        for (int i = 0; i < 16; i++)
            b[i] = 2 * a[(i + 1) % 4];
    }

    for (int i = 0; i < 4; i++)
        printf("a[%d] = %d\n", i, a[i]);
    for (int i = 0; i < 16; i++)
        printf("b[%d] = %d\n", i, b[i]);
}