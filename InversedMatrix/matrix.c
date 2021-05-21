#include "matrix.h"
#include <math.h>
#include <stdlib.h>

bool is_zero(double x)
{
	return fabs(x) < EPSILON;
}

void assert_allocation(const void* allocated_memory)
{
	if (allocated_memory == NULL)
	{
		puts("Allocation error.");
		exit(EXIT_FAILURE);
	}
}

void print_matrix(const Matrix* matrix)
{
	for (size_t i = 0; i < matrix->size; i++)
	{
		for (size_t j = 0; j < matrix->size; j++)
		{
			printf(ELEMENTS_PRINT_FORMAT " ", matrix->elements[i][j]);
		}
		puts("");
	}
}

void read_matrix(Matrix* matrix)
{
	for (size_t i = 0; i < matrix->size; i++)
	{
		for (size_t j = 0; j < matrix->size; j++)
		{
			if (scanf("%lf", &matrix->elements[i][j]) != 1)
			{
				puts("Incorrect input.");
				exit(EXIT_FAILURE);
			}
		}
	}
}

void free_matrix(Matrix* matrix)
{
	for (size_t i = 0; i < matrix->size; i++)
	{
		free(matrix->elements[i]);
	}
	free(matrix->elements);
}

Matrix create_matrix(size_t size)
{
	Matrix matrix;
	matrix.size = size;;
	matrix.elements = malloc(size * sizeof(*matrix.elements));
	assert_allocation(matrix.elements);
	for (size_t i = 0; i < size; i++)
	{
		matrix.elements[i] = malloc(size * sizeof(**matrix.elements));
		assert_allocation(matrix.elements[i]);
	}
	return matrix;
}

Matrix copy_matrix(const Matrix* other)
{
	Matrix matrix = create_matrix(other->size);

	for (size_t i = 0; i < matrix.size; i++)
	{
		for (size_t j = 0; j < matrix.size; j++)
		{
			matrix.elements[i][j] = other->elements[i][j];
		}
	}

	return matrix;
}

Matrix create_minor(const Matrix* matrix, size_t rowIndex, size_t colIndex)
{
	Matrix newMatrix = create_matrix(matrix->size - 1);
	for (size_t i = 0; i < newMatrix.size; i++)
	{
		for (size_t j = 0; j < newMatrix.size; j++)
		{
			// Пропускаем rowIndex, colIndex элемент.
			newMatrix.elements[i][j] = matrix->elements[i >= rowIndex ? i + 1 : i][j >= colIndex ? j + 1 : j];
		}
	}
	return newMatrix;
}

Matrix transpose_matrix(const Matrix* matrix)
{
	Matrix newMatrix = create_matrix(matrix->size);
	for (size_t i = 0; i < newMatrix.size; i++)
	{
		for (size_t j = 0; j < newMatrix.size; j++)
		{
			newMatrix.elements[i][j] = matrix->elements[j][i];
		}
	}
	return newMatrix;
}

Matrix get_inserved_matrix(const Matrix* matrix, double det)
{
	// Присоединенная матрица.
	Matrix adjugate_matrix = create_matrix(matrix->size);
	for (size_t i = 0; i < adjugate_matrix.size; i++)
	{
		for (size_t j = 0; j < adjugate_matrix.size; j++)
		{
			// Считаем алгебраические дополнения.
			Matrix minor_matrix = create_minor(matrix, i, j);
			double minor_det;
			gaussian_determinant(&minor_matrix, &minor_det);
			adjugate_matrix.elements[i][j] = ((i ^ j) & 1 ? -1.0 : 1.0) * minor_det;
			free_matrix(&minor_matrix);
		}
	}

	// Транспонируем присоединенную матрицу.
	Matrix adjugate_matrix_transposed = transpose_matrix(&adjugate_matrix);
	free_matrix(&adjugate_matrix);

	for (size_t i = 0; i < adjugate_matrix_transposed.size; i++)
	{
		for (size_t j = 0; j < adjugate_matrix_transposed.size; j++)
		{
			// Делим элементы на определитель исходной матрицы.
			adjugate_matrix_transposed.elements[i][j] /= det;
		}
	}
	
	return adjugate_matrix_transposed;
}

// Метод Гаусса для подсчета определителя.
void gaussian_determinant(Matrix* matrix, double* det)
{
	if (!det)
		return;

	Matrix buffer_matrix = copy_matrix(matrix);

	*det = 1.0;

	int i = 0, j = 0;
	while (i < buffer_matrix. size && j < buffer_matrix.size)
	{
		if (is_zero(buffer_matrix.elements[i][j]))
		{
			int k;
			for (k = i + 1; k < buffer_matrix.size; k++)
			{
				if (!is_zero(buffer_matrix.elements[k][j]))
				{
					break;
				}
			}

			// No such rows.
			if (k == buffer_matrix.size)
			{
				bool notNullAbove = false;
				for (k = 0; k < i; k++)
				{
					notNullAbove |= !is_zero(buffer_matrix.elements[k][j]);
				}

				if (notNullAbove)
				{
					int foundColumn = -1;
					for (k = j + 1; k < buffer_matrix.size; ++k)
					{
						if (!is_zero(buffer_matrix.elements[i][k]))
						{
							foundColumn = k;
							break;
						}
					}

					// No such columns.
					if (foundColumn == -1)
					{
						j++;
					}
					else
					{
						*det *= -1.0;
						for (int l = 0; l < buffer_matrix.size; l++)
						{
							double tmp = buffer_matrix.elements[l][j];
							buffer_matrix.elements[l][j] = buffer_matrix.elements[l][foundColumn];
							buffer_matrix.elements[l][foundColumn] = tmp;
						}
					}
				}
				else
				{
					j++;
				}
			}
			else
			{
				*det *= -1.0;
				for (int l = j; l < buffer_matrix.size; l++)
				{
					double tmp = buffer_matrix.elements[i][l];
					buffer_matrix.elements[i][l] = buffer_matrix.elements[k][l];
					buffer_matrix.elements[k][l] = tmp;
				}
			}
		}
		else
		{
			double coeff = buffer_matrix.elements[i][j];
			*det *= coeff;
			for (int k = j; k < buffer_matrix.size; ++k)
			{
				buffer_matrix.elements[i][k] = buffer_matrix.elements[i][k] / coeff;
			}

			for (int k = 0; k < buffer_matrix.size; k++)
			{
				if (k == i)
				{
					continue;
				}

				coeff = buffer_matrix.elements[k][j] / buffer_matrix.elements[i][j];
				for (int l = j; l < buffer_matrix.size; l++)
				{
					double tmp = coeff * buffer_matrix.elements[i][l];
					buffer_matrix.elements[k][l] = buffer_matrix.elements[k][l] - tmp;
				}
			}

			i++; j++;
		}
	}

	for (size_t i = 0; i < buffer_matrix.size; i++)
	{
		// Умножаем на диагональные элементы.
		*det *= buffer_matrix.elements[i][i];
	}

	free_matrix(&buffer_matrix);
}
