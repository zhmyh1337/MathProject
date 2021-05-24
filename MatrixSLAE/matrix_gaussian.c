#include "matrix_gaussian.h"

// Метод Гаусса.
void apply_gaussian(Matrix* matrix, bool only_left_side_columns_swapping)
{
	int i = 0, j = 0;
	while (i < matrix->n && j < matrix->m)
	{
		if (is_zero(matrix->elements[i][j]))
		{
			int k;
			for (k = i + 1; k < matrix->n; k++)
			{
				if (!is_zero(matrix->elements[k][j]))
				{
					break;
				}
			}

			// No such rows.
			if (k == matrix->n)
			{
				bool notNullAbove = false;
				for (k = 0; k < i; k++)
				{
					notNullAbove |= !is_zero(matrix->elements[k][j]);
				}

				if (notNullAbove)
				{
					int foundColumn = -1;
					for (k = j + 1; k < matrix->m / (only_left_side_columns_swapping ? 2 : 1); ++k)
					{
						if (!is_zero(matrix->elements[i][k]))
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
						for (int l = 0; l < matrix->n; l++)
						{
							double tmp = matrix->elements[l][j];
							matrix->elements[l][j] = matrix->elements[l][foundColumn];
							matrix->elements[l][foundColumn] = tmp;
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
				for (int l = j; l < matrix->m; l++)
				{
					double tmp = matrix->elements[i][l];
					matrix->elements[i][l] = matrix->elements[k][l];
					matrix->elements[k][l] = tmp;
				}
			}
		}
		else
		{
			double coeff = matrix->elements[i][j];
			for (int k = j; k < matrix->m; ++k)
			{
				matrix->elements[i][k] = matrix->elements[i][k] / coeff;
			}

			for (int k = 0; k < matrix->n; k++)
			{
				if (k == i)
				{
					continue;
				}

				coeff = matrix->elements[k][j] / matrix->elements[i][j];
				for (int l = j; l < matrix->m; l++)
				{
					double tmp = coeff * matrix->elements[i][l];
					matrix->elements[k][l] = matrix->elements[k][l] - tmp;
				}
			}

			i++; j++;
		}
	}
}
