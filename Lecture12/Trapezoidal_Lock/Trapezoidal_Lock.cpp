#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "DS_timer.h"

#define f(_x) (_x*_x)
#define NUM_THREADS (4)
#define offset (16)

enum Algorithm {
	Serial, Parallel_offset, Parallel_Lock, END
};

double Trapezodial_Serial(double a, double b, int n, double h);
double Trapezodial_Parallel_offset(double a, double b, int n, double h);
double Trapezodial_Parallel_Lock(double a, double b, int n, double h);

#define RUN_TEST(_func, a, b, n, h) { \
	timer.onTimer(Algorithm::_func); \
	sum = Trapezodial_##_func(a, b, n, h);			\
	timer.offTimer(Algorithm::_func); \
	printf("[%s] area = %lf (%.2f x)\n", #_func, sum		\
		, timer.getTimer_ms(Algorithm::Serial) / timer.getTimer_ms(Algorithm::_func));	\
}

int main(void)
{
	DS_timer timer(Algorithm::END);
	timer.setTimerName(Algorithm::Serial, (char*)"Serial algorithm");
	timer.setTimerName(Algorithm::Parallel_offset, (char*)"Parallel algorithm");
	timer.setTimerName(Algorithm::Parallel_Lock, (char*)"Parallel algorithm - Lock");

	double a = -1, b = 1;
	int n = (1024 * 1024 * 1024);

	double h = (b - a) / n;
	printf("f(x) = x * x\n");
	printf("range = (%lf, %lf), n = %d, h = %.12lf\n", a, b, n, h);
	double sum = 0;

	RUN_TEST(Serial, a, b, n, h);
	RUN_TEST(Parallel_offset, a, b, n, h);
	RUN_TEST(Parallel_Lock, a, b, n, h);

	timer.printTimer();
}

/* Function definitions */
double Trapezodial_Serial(double a, double b, int n, double h)
{
	double sum = 0;
	for (int i = 0; i < n - 1; i++)
	{
		double x_i = a + h * i;
		double x_j = a + h * (i + 1);
		double d = (f(x_i) + f(x_j)) / 2.0;
		sum += d * h;
	}
	return sum;
}

double Trapezodial_Parallel_offset(double a, double b, int n, double h)
{
	double sum = 0;
	double local[NUM_THREADS * offset] = { 0 };
#pragma omp parallel num_threads(NUM_THREADS)
	{
		int tid = omp_get_thread_num() * offset;

#pragma omp for
		for (int i = 0; i < n - 1; i++)
		{
			double x_i = a + h * i;
			double x_j = a + h * (i + 1);
			double d = (f(x_i) + f(x_j)) / 2.0;
			local[tid] += d * h;
		}
	} // implicit barrier here

	  // Gathering the local sums in serial
	for (int i = 0; i < NUM_THREADS; i++)
		sum += local[i * offset];

	return sum;
}

double Trapezodial_Parallel_Lock(double a, double b, int n, double h)
{
	double sum = 0;

	omp_lock_t sumLock;
	omp_init_lock(&sumLock);

	for (int i = 0; i < n - 1; i++)
	{
		double x_i = a + h * i;
		double x_j = a + h * (i + 1);
		double d = (f(x_i) + f(x_j)) / 2.0;

		omp_set_lock(&sumLock);
		sum += d * h;
		omp_unset_lock(&sumLock);
	}

	omp_destroy_lock(&sumLock);
	return sum;
}
