#ifndef _MATRIX
#define _MATRIX

#include <stdio.h>
#include <stdbool.h>

#define EPSILON 1e-10
#define ELEMENTS_PRINT_FORMAT "%10.3e"

typedef struct 
{
	double** elements;
	size_t n;
	size_t m;
} Matrix;

bool is_zero(double x);

Matrix create_matrix(size_t n, size_t m);

void free_matrix(Matrix* matrix);

void read_matrix_from_file(Matrix* matrix, const char* file_name);

void initialize_matrix_with_formula(Matrix* matrix, const size_t k);

void print_matrix(const Matrix* matrix, const size_t limit);

bool try_get_inversed_matrix(const Matrix* matrix, Matrix* result);

Matrix multiply(const Matrix* a, const Matrix* b);

Matrix substitute(const Matrix* a, const Matrix* b);

#endif
