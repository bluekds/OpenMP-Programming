#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <DS_timer.h>
#include <DS_definitions.h>

#define m (10000)
#define n (10000)

#define GenFloat (rand() % 100 + ((float)(rand() % 100) / 100.0))
void genRandomInput();

float A[m][n];
float X[n];
float Y_serial[m];
float Y_parallel[m];

int main(int argc, char* argv[])
{
	DS_timer timer(2);
	timer.setTimerName(0, (char*)"Serial");
	timer.setTimerName(1, (char*)"Parallel");

	genRandomInput();

	// Serial code --> Y_serial[]
	timer.onTimer(0);

	timer.offTimer(0);

	// Parallel code --> Y_parallel[]
	timer.onTimer(1);

	timer.offTimer(1);

	// Check results
	LOOP_I(m) {
		if (Y_serial[i] != Y_parallel[i]) {
			printf("Results are not matched :(");
			EXIT_WIHT_KEYPRESS;
		}
	}

	timer.printTimer();
	EXIT_WIHT_KEYPRESS;
}

void genRandomInput(void) {
	// A matrix
	LOOP_INDEX(row, m) {
		LOOP_INDEX(col, n) {
			A[row][col] = GenFloat;
		}
	}

	LOOP_I(n)
		X[i] = GenFloat;

	memset(Y_serial, 0, sizeof(float) * m);
	memset(Y_parallel, 0, sizeof(float) * m);
}