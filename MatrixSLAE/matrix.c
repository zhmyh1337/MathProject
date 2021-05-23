#include "matrix.h"
#include "main.h"
#include "initialization_formulas.h"
#include "matrix_gaussian.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

bool is_zero(double x)
{
	return fabs(x) < EPSILON;
}

Matrix create_matrix(size_t n, size_t m)
{
	Matrix matrix;
	matrix.n = n;
	matrix.m = m;
	matrix.elements = malloc(n * sizeof(*matrix.elements));
	assert_allocation(matrix.elements);
	for (size_t i = 0; i < n; i++)
	{
		matrix.elements[i] = malloc(m * sizeof(**matrix.elements));
		assert_allocation(matrix.elements[i]);
	}
	return matrix;
}

void free_matrix(Matrix* matrix)
{
	for (size_t i = 0; i < matrix->n; i++)
	{
		free(matrix->elements[i]);
	}
	free(matrix->elements);
}

void read_matrix_from_file(Matrix* matrix, const char* file_name)
{
	FILE* file = fopen(file_name, "r");
	if (!file)
	{
		perror("File reading error");
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < matrix->n; i++)
	{
		for (size_t j = 0; j < matrix->m; j++)
		{
			if (fscanf(file, "%lf", &matrix->elements[i][j]) != 1)
			{
				puts("Incorrect format in file.");
				fclose(file);
				exit(EXIT_FAILURE);
			}
		}
	}
	fclose(file);
}

void initialize_matrix_with_formula(Matrix* matrix, const size_t k)
{
	initialization_formula_func func;
	switch (k)
	{
	case 1:
		func = initialization_formula_1;
		break;
	case 2:
		func = initialization_formula_2;
		break;
	case 3:
		func = initialization_formula_3;
		break;
	case 4:
		func = initialization_formula_4;
		break;
	default:
		puts("Incorrect formula.");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < matrix->n; i++)
	{
		for (size_t j = 0; j < matrix->m; j++)
		{
			matrix->elements[i][j] = func(k, matrix->n, i, j);
		}
	}
}

void print_matrix(const Matrix* matrix, const size_t limit)
{
	for (size_t i = 0; i < min(matrix->n, limit); i++)
	{
		for (size_t j = 0; j < min(matrix->m, limit); j++)
		{
			printf(" " ELEMENTS_PRINT_FORMAT, matrix->elements[i][j]);
		}
		puts("");
	}
}

bool try_get_inversed_matrix(const Matrix* matrix, Matrix* result)
{
	Matrix extended_matrix = create_matrix(matrix->n, matrix->n * 2);
	for (size_t i = 0; i < matrix->n; i++)
	{
		for (size_t j = 0; j < matrix->n; j++)
		{
			extended_matrix.elements[i][j] = matrix->elements[i][j];
			// Элементы единичной матрицы справа.
			extended_matrix.elements[i][j + matrix->n] = i == j ? 1.0 : 0.0;
		}
	}

	apply_gaussian(&extended_matrix, true);
	bool last_row_only_nulls = true;
	for (size_t i = 0; i < extended_matrix.n; i++)
	{
		last_row_only_nulls &= is_zero(extended_matrix.elements[extended_matrix.n - 1][i]);
	}
	// Матрица необратима.
	if (last_row_only_nulls)
	{
		free_matrix(&extended_matrix);
		return false;
	}

	Matrix inversed_matrix = create_matrix(matrix->n, matrix->n);
	for (size_t i = 0; i < matrix->n; i++)
	{
		for (size_t j = 0; j < matrix->n; j++)
		{
			inversed_matrix.elements[i][j] = extended_matrix.elements[i][j + matrix->n];
		}
	}
	*result = inversed_matrix;
	free_matrix(&extended_matrix);
	return true;
}

Matrix multiply(const Matrix* a, const Matrix* b)
{
	assert(a->m == b->n && "Matrices are not multiplicable.");

	Matrix result = create_matrix(a->n, b->m);
	for (size_t i = 0; i < result.n; i++)
	{
		for (size_t j = 0; j < result.m; j++)
		{
			double sum = 0.0;
			for (size_t k = 0; k < result.m; k++)
			{
				sum += a->elements[i][k] * b->elements[k][j];
			}
			result.elements[i][j] = sum;
		}
	}
	return result;
}

Matrix substitute(const Matrix* a, const Matrix* b)
{
	assert(a->n == b->n && a->m == b->m && "Matrices are not substitutable.");

	Matrix result = create_matrix(a->n, a->m);
	for (size_t i = 0; i < result.n; i++)
	{
		for (size_t j = 0; j < result.m; j++)
		{
			result.elements[i][j] = a->elements[i][j] - b->elements[i][j];
		}
	}
	return result;
}
