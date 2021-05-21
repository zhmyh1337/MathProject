#include "matrix.h"
#include <stdlib.h>

int main()
{
	size_t matrix_size;
	printf("Matrix size: ");
	if (scanf("%zu", &matrix_size) != 1)
	{
		puts("Incorrect input.");
		exit(EXIT_FAILURE);
	}

	Matrix matrix = create_matrix(matrix_size);
	read_matrix(&matrix);
	double det;
	gaussian_determinant(&matrix, &det);

	if (is_zero(det))
	{
		puts("Matrix is non invertible.");
		return 0;
	}

	Matrix inversed_matrix = get_inserved_matrix(&matrix, det);
	puts("");
	print_matrix(&inversed_matrix);

	free_matrix(&matrix);
	free_matrix(&inversed_matrix);
}
