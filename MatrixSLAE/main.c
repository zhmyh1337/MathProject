#include "main.h"
#include "matrix.h"
#include "slae.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void assert_allocation(const void* allocated_memory)
{
	if (allocated_memory == NULL)
	{
		puts("Allocation error.");
		exit(EXIT_FAILURE);
	}
}

static void incorrect_parameters()
{
	puts("Incorrect parameters.");
	exit(EXIT_FAILURE);
}

static Matrix generate_b_vector(const Matrix* matrix, size_t n)
{
	Matrix result = create_matrix(n, 1);
	for (size_t i = 0; i < n; i++)
	{
		double sum = 0.0;
		for (size_t k = 0; k < (n + 1) / 2; k++)
		{
			sum += matrix->elements[i][2 * k];
		}
		result.elements[i][0] = sum;
	}
	return result;
}

static double get_answer_error_norm(const Matrix* answer)
{
	Matrix exact_solution = create_matrix(answer->n, 1);
	Matrix subtracted_vector = subtract(answer, &exact_solution);
	for (size_t i = 0; i < exact_solution.n; i++)
	{
		exact_solution.elements[i][0] = (i + 1) % 2;
	}
	double result = get_vector_norm(&subtracted_vector);
	free_matrix(&exact_solution);
	free_matrix(&subtracted_vector);
	return result;
}

int main(int argc, char** argv)
{
	if (argc <= 3)
	{
		incorrect_parameters();
	}
	size_t n, m, k;
	if (sscanf(argv[1], "%zu", &n) != 1 ||
		sscanf(argv[2], "%zu", &m) != 1 ||
		sscanf(argv[3], "%zu", &k) != 1)
	{
		incorrect_parameters();
	}
	// Имя файла не предоставлено.
	if (k == 0 && argc <= 4)
	{
		incorrect_parameters();
	}

	Matrix matrix = create_matrix(n, n);
	if (k != 0)
	{
		initialize_matrix_with_formula(&matrix, k);
	}
	else
	{
		read_matrix_from_file(&matrix, argv[4]);
	}
	Matrix b_vector = generate_b_vector(&matrix, n);
	puts("b vector:");
	print_matrix(&b_vector, m);

	clock_t start_time = clock();

	Matrix answer;
	if (solve_slae(m, &matrix, &b_vector, &answer))
	{
		puts("Answer:");
		print_matrix(&answer, m);
		Matrix A_by_x = multiply(&matrix, &answer);
		Matrix A_by_x_minus_b = subtract(&A_by_x, &b_vector);
		printf("||Ax-b|| / ||b|| = %lf\n", get_matrix_norm(&A_by_x_minus_b) / get_matrix_norm(&b_vector));
		printf("error norm between answer and (1, 0, 1, 0, 1, ...) = %lf\n", get_answer_error_norm(&answer));
		free_matrix(&A_by_x);
		free_matrix(&A_by_x_minus_b);
	}
	clock_t finish_time = clock();

	puts("");
	printf("Elapsed time: %lf ms.", 1000.0 * (finish_time - start_time) / CLOCKS_PER_SEC);
	free_matrix(&matrix);
	free_matrix(&b_vector);
	free_matrix(&answer);
}
