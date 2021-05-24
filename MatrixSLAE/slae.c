#include "slae.h"

bool solve_slae(size_t m, const Matrix* matrix, const Matrix* b_vector, Matrix* answer)
{
	Matrix inversed_matrix;
	if (try_get_inversed_matrix(matrix, &inversed_matrix))
	{
		puts("Inversed matrix:");
		print_matrix(&inversed_matrix, m);
		Matrix A_by_A_inv = multiply(matrix, &inversed_matrix);
		Matrix identity_matrix = create_identity_matrix(matrix->n);
		Matrix subtracted_matrix = subtract(&A_by_A_inv, &identity_matrix);
		printf("||A*A^(-1)-E|| = %lf\n", get_matrix_norm(&subtracted_matrix));
		free_matrix(&A_by_A_inv);
		free_matrix(&identity_matrix);
		free_matrix(&subtracted_matrix);

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
