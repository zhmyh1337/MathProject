#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>

#define INTERVAL_LEFT 0.0
#define INTERVAL_RIGHT M_PI
#define STEP 1e-6
#define INITIAL_APPROXIMATION 0.0
// Точное решение y(x) = exp(x)*sin(x):
// sin(x) = 0 <=> x = 0
#define ANALYTICAL_SOLUTION 0.0

int main(void)
{
	// Будем решать методом Эйлера. За шаг возьмем STEP.
	double y = INITIAL_APPROXIMATION, f, hf;
	for (double x = INTERVAL_LEFT; x <= INTERVAL_RIGHT; x += STEP)
	{
		f = y + exp(x) * cos(x);
		hf = STEP * f;
		y += hf;
	}

	printf("Approximate solution: %lf\n", y);
	printf("Analytical solution: %lf\n", ANALYTICAL_SOLUTION);
	printf("Deviation: %lf\n", fabs(y - ANALYTICAL_SOLUTION));
}
