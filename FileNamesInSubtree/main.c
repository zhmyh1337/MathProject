#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>

bool is_regular_file(const char* path)
{
	struct stat path_stat;
	return !stat(path, &path_stat) && S_ISREG(path_stat.st_mode);
}

off_t get_file_size(const char* path)
{
	struct stat path_stat;
	return stat(path, &path_stat) ? 0 : path_stat.st_size;
}

void assert_allocation(const void* allocated_memory)
{
	if (allocated_memory == NULL)
	{
		puts("Allocation error.");
		exit(EXIT_FAILURE);
	}
}

typedef struct  
{
	char name[PATH_MAX];
	off_t size;
} file_with_size;

// Динамический массив.
typedef struct
{
	void* data;
	size_t size;
	size_t capacity;
} storage;

void enumerate_files_recursively(const char* basePath, storage* files_storage)
{
	char path[PATH_MAX];
	dirent* dp;
	DIR* dir = opendir(basePath);

	if (!dir)
		return;

	while ((dp = readdir(dir)) != NULL)
	{
		// Пропускаем текущую директорию и родительскую.
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			// Убеждаемся, что путь не превышает максимальной длины.
			if (strlen(basePath) + 1 + strlen(dp->d_name) + 1 <= PATH_MAX)
			{
				// Указываем текущий путь.
				strcpy(path, basePath);
				strcat(path, "/");
				strcat(path, dp->d_name);

				if (is_regular_file(path))
				{
					// Если вместимость динамического массива мала, увеличиваем ее.
					if (++files_storage->size > files_storage->capacity)
					{
						files_storage->capacity *= 2;
						files_storage->data = realloc(files_storage->data, files_storage->capacity * sizeof(file_with_size));
						assert_allocation(files_storage->data);
					}

					// Сохраняем информацию о файле в динамический массив.
					file_with_size* file = &((file_with_size*)files_storage->data)[files_storage->size - 1];
					strcpy(file->name, dp->d_name);
					file->size = get_file_size(path);
				}

				enumerate_files_recursively(path, files_storage);
			}
		}
	}

	closedir(dir);
}

int comparator(const file_with_size* file1, const file_with_size* file2)
{
	return file1->size == file2->size ? 0 :
		file1->size < file2->size ? -1 : 1;
}

int main(int argc, char** argv)
{
	storage files_storage;
	files_storage.capacity = 1;
	files_storage.size = 0;
	files_storage.data = malloc(files_storage.capacity * sizeof(file_with_size));
	assert_allocation(files_storage.data);

	// Если аргументы не переданы, сканируем текущую папку, иначе сканируем ту папку,
	// что была передана в аргументы запуска.
	enumerate_files_recursively(argc == 1 ? "." : argv[1], &files_storage);

	qsort(files_storage.data, files_storage.size, sizeof(file_with_size), comparator);

	for (size_t i = 0; i < files_storage.size; i++)
	{
		file_with_size* file = &((file_with_size*)files_storage.data)[i];
		printf("%s %ld\n", file->name, file->size);
	}
}
