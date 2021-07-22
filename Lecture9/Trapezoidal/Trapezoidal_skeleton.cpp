#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "DS_timer.h"

enum Algorithm {
	Serial, Parallel, END
};

double Trapezodial_Serial(double a, double b, int n, double h);
double Trapezodial_Parallel(double a, double b, int n, double h);

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
	timer.setTimerName(Algorithm::Parallel, (char*)"Parallel algorithm");

	double a = -1, b = 1;
	int n = (1024*1024*1024);

	double h = (b - a) / n;
	printf("f(x) = x * x\n");
	printf("range = (%lf, %lf), n = %d, h = %.12lf\n", a, b, n, h);
	double sum = 0;

	RUN_TEST(Serial, a, b, n, h);
	RUN_TEST(Parallel, a, b, n, h);

	timer.printTimer();
}

/* Function definitions */
double Trapezodial_Serial(double a, double b, int n, double h)
{
	double sum = 0;
	// Write your code here
	return sum;
}

double Trapezodial_Parallel(double a, double b, int n, double h)
{
	double sum = 0;
	// Write your code here
	return sum;
}