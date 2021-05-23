#include "slae.h"

bool solve_slae(size_t m, const Matrix* matrix, const Matrix* b_vector, Matrix* answer)
{
	Matrix inversed_matrix;
	if (try_get_inversed_matrix(&matrix, &inversed_matrix))
	{
		puts("Inversed matrix:");
		print_matrix(&inversed_matrix, m);

		*answer = multiply(&inversed_matrix, b_vector);

		free_matrix(&inversed_matrix);
		return true;
	}
	else
	{
		puts("Matrix is non invertible.");
		return false;
	}
}
