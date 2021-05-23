#ifndef _INITIALIZATION_FORMULAS
#define _INITIALIZATION_FORMULAS

#include <stdlib.h>

typedef double (*initialization_formula_func)(size_t k, size_t n, size_t i, size_t j);

double initialization_formula_1(size_t k, size_t n, size_t i, size_t j)
{
	return n - max(i, j) + 1;
}

double initialization_formula_2(size_t k, size_t n, size_t i, size_t j)
{
	return max(i, j);
}

double initialization_formula_3(size_t k, size_t n, size_t i, size_t j)
{
	return abs(i - j);
}

double initialization_formula_4(size_t k, size_t n, size_t i, size_t j)
{
	return 1.0 / (i + j - 1);
}

#endif
