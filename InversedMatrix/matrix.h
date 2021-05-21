#ifndef _MATRIX
#define _MATRIX

#include <stdio.h>
#include <stdbool.h>

#define EPSILON 1e-10
#define ELEMENTS_PRINT_FORMAT "%.2e"

typedef struct 
{
	double** elements;
	size_t size;
} Matrix;

bool is_zero(double x);

void gaussian_determinant(Matrix* matrix, double* det);

void free_matrix(Matrix* matrix);

Matrix create_matrix(size_t size);

Matrix create_minor(const Matrix* matrix, size_t i, size_t j);

Matrix transpose_matrix(const Matrix* matrix);

Matrix get_inserved_matrix(const Matrix* matrix, double det);

void assert_allocation(const void* allocated_memory);

void print_matrix(const Matrix* matrix);

void read_matrix(Matrix* matrix);

#endif
