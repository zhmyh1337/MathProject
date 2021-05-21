#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define CHARACTERS 256
#define MAX_STRING_LENGTH 256

/*
Очевидно, что при произвольном количестве перестановок символов на позициях
i_1, i_2, ..., i_k из строки S можно получить строку T тогда и только тогда,
когда суммарные количества содержаний каждого символа на этих позициях у строк S и T равны.

Таким образом, две строки попадут в одну группу эквивалентности, если эти суммарные количества
у этих строк будут равны как для четных позиций, так и для нечетных.

Для каждой строки предпросчитаем количества вхождений каждого символа на четных и нечетных позициях,
после чего сделаем разбиение на группы эквивалентности через своеобразное "решето":
для этого нам понадобится O(arrSize ^ 2) сравнений между строками, каждое сравнение за O(CHARACTERS).
*/

void assert_allocation(const void* allocated_memory)
{
	if (allocated_memory == NULL)
	{
		puts("Allocation error.");
		exit(EXIT_FAILURE);
	}
}

// Читаем в строку str с контролем переполнения по максимальной длине max_length (включает нуль-терминатор).
// При ошибке функция вернет false, иначе true.
// Если outlen != NULL и нет ошибок, функция запишет в него длину строки без символа переноса строки.
bool safe_string_read(char* str, size_t max_length, size_t* outlen)
{
	// Наверняка можем сказать, что строка полностью поместилась в буфер,
	// только если последний ее символ - символ переноса строки.
	size_t len;
	if (fgets(str, (int)max_length, stdin) && str[(len = strlen(str)) - 1] == '\n')
	{
		// Убираем символ переноса строки.
		str[--len] = '\0';
		if (outlen)
			*outlen = len;
		return true;
	}
	return false;
}

int main(void)
{
	size_t arr_size;
	printf("Array size: ");
	if (scanf("%zu", &arr_size) != 1)
	{
		puts("Incorrect input.");
		return EXIT_FAILURE;
	}
	// Символ перевода строки.
	getchar();

	// Массив строк.
	char** arr = malloc(arr_size * sizeof(*arr));
	assert_allocation(arr);
	// Массив длин строк.
	size_t* lengths = malloc(arr_size * sizeof(*lengths));
	assert_allocation(lengths);
	// Словарь количеств на нечетных позициях.
	size_t (*dict_odd)[CHARACTERS] = calloc(arr_size, sizeof(*dict_odd));
	assert_allocation(dict_odd);
	// Словарь количеств на четных позициях.
	size_t (*dict_even)[CHARACTERS] = calloc(arr_size, sizeof(*dict_even));
	assert_allocation(dict_even);

	// Считываем строки.
	for (size_t i = 0; i < arr_size; i++)
	{
		arr[i] = malloc(MAX_STRING_LENGTH);
		assert_allocation(arr[i]);
		if (!safe_string_read(arr[i], MAX_STRING_LENGTH, &lengths[i]))
		{
			puts("Incorrect input (possibly too many characters).");
			return EXIT_FAILURE;
		}
	}

	// Считаем словари количеств.
	for (size_t i = 0; i < arr_size; i++)
	{
		for (size_t j = 0; j < lengths[i]; j++)
		{
			// Инкрементируем количество символа в словаре.
			(*(j & 1 ? &dict_odd[i][(size_t)arr[i][j]] : &dict_even[i][(size_t)arr[i][j]]))++;
		}
	}

	// Группы эквивалентности.
	size_t* equivalence_groups = malloc(arr_size * sizeof(*equivalence_groups));
	// Инициализируем группы эквивалентности значениями -1 (если точнее, то байтами 0xFF),
	// это будет соответствовать тому, что группа еще не задана.
	memset(equivalence_groups, -1, arr_size * sizeof(*equivalence_groups));
	// Количество групп эквивалентностей и номер следующей.
	size_t equivalence_groups_count = 0;

	// Задаем группы решетом.
	for (size_t i = 0; i < arr_size; i++)
	{
		// Группа эквивалентности не задана, значит начинаем новую,
		// иначе эта группа уже полностью создана, пропускаем.
		if (equivalence_groups[i] == (size_t)-1)
			equivalence_groups[i] = equivalence_groups_count++;
		else
			continue;

		for (size_t j = i + 1; j < arr_size; j++)
		{
			// Эквивалентность выполняется.
			if (lengths[i] == lengths[j] &&
				!memcmp(dict_even[i], dict_even[j], CHARACTERS * sizeof(**dict_even)) &&
				!memcmp(dict_odd[i], dict_odd[j], CHARACTERS * sizeof(**dict_odd)))
			{
				equivalence_groups[j] = equivalence_groups[i];
			}
		}
	}

	// Выводим.
	puts("");
	printf("Equivalence groups count = %zu\n", equivalence_groups_count);
	puts("Equivalence groups:");
	for (size_t i = 0; i < equivalence_groups_count; i++)
	{
		puts("");
		for (size_t j = 0; j < arr_size; j++)
		{
			if (equivalence_groups[j] == i)
			{
				puts(arr[j]);
			}
		}
	}
}
