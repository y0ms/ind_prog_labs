#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAX_ALIGNED_SIZE ((short) 100)

void input_matrix_size(const char* prompt, short* n, short* m) {
	do {
		std::cout << prompt << std::endl;
		std::cin >> *n >> *m;
	} while (*n <= 0 && *m <= 0);
}

void fill_matrix_zeros(int** matrix, short n, short m) {
	for (short i = 0; i < n; i++)
		for (short j = 0; j < m; j++)
			matrix[i][j] = 0;
}

int** allocate_matrix(short n, short m) {
	int** matrix = new int*[n];
	for (short i = 0; i < n; i++)
		matrix[i] = new int[m];

	return matrix;
}

int** allocate_matrix_fill_zeros(short n, short m) {
	int** matrix = allocate_matrix(n, m);
	fill_matrix_zeros(matrix, n, m);

	return matrix;
}

void fill_matrix_manually(int** matrix, short n, short m) {
	for (short i = 0; i < n; i++)
		for (short j = 0; j < m; j++)
			std::cin >> matrix[i][j];
}

void fill_matrix_randomly(int** matrix, short n, short m) {
	for (short i = 0; i < n; i++)
		for (short j = 0; j < m; j++)
			matrix[i][j] = rand() % 10;
}

int** expand_matrix(int** matrix, short n, short m, short en, short em) {
	int** expanded_matrix = allocate_matrix(en, em);
	fill_matrix_zeros(expanded_matrix, en, em);

	for (short i = 0; i < n; i++)
		for (short j = 0; j < m; j++)
			expanded_matrix[i][j] = matrix[i][j];

	return expanded_matrix;
}

void print_matrix_with_text(const char* title, int** matrix, short n, short m) {
	std::cout << std::endl << title << std::endl << std::endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			std::cout << matrix[i][j] << " ";
		std::cout << std::endl;
	}
}

int** get_sub_matrix(int** matrix, short i0, short in, short j0, short jm) {
	int** sub_matrix = new int* [in - i0];
	for (int i = 0; i < in - i0; i++) {
		sub_matrix[i] = new int[jm-j0];
		for (int j = 0; j < jm - j0; j++)
			sub_matrix[i][j] = matrix[i+i0][j+j0];
	}

	return sub_matrix;
}

void delete_matrix(int** matrix, short n) {
	for(int i = 0; i < n; i++)
		delete[] matrix[i];

	delete[] matrix;
}

int main() {
	srand(time(NULL));
	system("chcp 1251");
	std::cout << "Вас приветствует программа" << std::endl <<
		"быстрого перемножения матриц A и B методом Штрассена" << std::endl << std::endl;

	short n_a, m_a, n_b, m_b, fill_type, expanded_size = 2, half_expanded_size;

	input_matrix_size("Введите размеры матрицы A: ", &n_a, &m_a);
	input_matrix_size("Введите размеры матрицы B: ", &n_b, &m_b);

	int** matrix_a = allocate_matrix(n_a, m_a);
	int** matrix_b = allocate_matrix(n_b, m_b);

	//Выбор способа заполнения и заполнение матриц
	do {
		std::cout << "Выберите способ заполнения матриц" << std::endl <<
			"1 - Вручную" << std::endl << "2 - Случайным образом" << std::endl;
		std::cin >> fill_type;
	} while (fill_type < 1 || fill_type > 2);

	switch (fill_type) {
		case 1:
			fill_matrix_manually(matrix_a, n_a, m_a);
			fill_matrix_manually(matrix_b, n_b, m_b);
			break;
		case 2:
			fill_matrix_randomly(matrix_a, n_a, m_a);
			fill_matrix_randomly(matrix_b, n_b, m_b);
			break;
	}

	print_matrix_with_text("Матрица A", matrix_a, n_a, m_a);
	print_matrix_with_text("Матрица B", matrix_b, n_b, m_b);

	// Приведение матриц к требуемому размеру
	while (expanded_size < n_a || expanded_size < n_b || expanded_size < m_a || expanded_size < m_b)
		expanded_size *= 2;
	half_expanded_size = expanded_size / 2;

	int** expanded_matrix_a = expand_matrix(matrix_a, n_a, m_a, expanded_size, expanded_size);
	int** expanded_matrix_b = expand_matrix(matrix_b, n_b, m_b, expanded_size, expanded_size);

	std::cout << "Приведенные матрицы" << std::endl;
	print_matrix_with_text("Матрица A", expanded_matrix_a, expanded_size, expanded_size);
	print_matrix_with_text("Матрица B", expanded_matrix_b, expanded_size, expanded_size);


	//Разбиение матриц на подматрицы и их заполнение
	int** sub_expanded_matrix_a_1 = get_sub_matrix(expanded_matrix_a, 0, half_expanded_size, 0, half_expanded_size);
	int** sub_expanded_matrix_a_2 = get_sub_matrix(expanded_matrix_a, 0, half_expanded_size, half_expanded_size, expanded_size);
	int** sub_expanded_matrix_a_3 = get_sub_matrix(expanded_matrix_a, half_expanded_size, expanded_size, 0, half_expanded_size);
	int** sub_expanded_matrix_a_4 = get_sub_matrix(expanded_matrix_a, half_expanded_size, expanded_size, half_expanded_size, expanded_size);

	int** sub_expanded_matrix_b_1 = get_sub_matrix(expanded_matrix_b, 0, half_expanded_size, 0, half_expanded_size);
	int** sub_expanded_matrix_b_2 = get_sub_matrix(expanded_matrix_b, 0, half_expanded_size, half_expanded_size, expanded_size);
	int** sub_expanded_matrix_b_3 = get_sub_matrix(expanded_matrix_b, half_expanded_size, expanded_size, 0, half_expanded_size);
	int** sub_expanded_matrix_b_4 = get_sub_matrix(expanded_matrix_b, half_expanded_size, expanded_size, half_expanded_size, expanded_size);


	//Создание промежуточных матриц
	int** intermediate_matrix_1 = allocate_matrix_fill_zeros(half_expanded_size, half_expanded_size);
	int** intermediate_matrix_2 = allocate_matrix_fill_zeros(half_expanded_size, half_expanded_size);
	int** intermediate_matrix_3 = allocate_matrix_fill_zeros(half_expanded_size, half_expanded_size);
	int** intermediate_matrix_4 = allocate_matrix_fill_zeros(half_expanded_size, half_expanded_size);
	int** intermediate_matrix_5 = allocate_matrix_fill_zeros(half_expanded_size, half_expanded_size);
	int** intermediate_matrix_6 = allocate_matrix_fill_zeros(half_expanded_size, half_expanded_size);
	int** intermediate_matrix_7 = allocate_matrix_fill_zeros(half_expanded_size, half_expanded_size);


	//Вычисление значений промежуточных матриц
	for (short i = 0; i < half_expanded_size; i++)
		for (short j = 0; j < half_expanded_size; j++)
			for (short z = 0; z < half_expanded_size; z++) {
				intermediate_matrix_1[i][j] += (sub_expanded_matrix_a_1[i][z] + sub_expanded_matrix_a_4[i][z]) * (sub_expanded_matrix_b_1[z][j] + sub_expanded_matrix_b_4[z][j]);
				intermediate_matrix_2[i][j] += (sub_expanded_matrix_a_3[i][z] + sub_expanded_matrix_a_4[i][z]) * sub_expanded_matrix_b_1[z][j];
				intermediate_matrix_3[i][j] += sub_expanded_matrix_a_1[i][z] * (sub_expanded_matrix_b_2[z][j] - sub_expanded_matrix_b_4[z][j]);
				intermediate_matrix_4[i][j] += sub_expanded_matrix_a_4[i][z] * (sub_expanded_matrix_b_3[z][j] - sub_expanded_matrix_b_1[z][j]);
				intermediate_matrix_5[i][j] += (sub_expanded_matrix_a_1[i][z] + sub_expanded_matrix_a_2[i][z]) * sub_expanded_matrix_b_4[z][j];
				intermediate_matrix_6[i][j] += (sub_expanded_matrix_a_3[i][z] - sub_expanded_matrix_a_1[i][z]) * (sub_expanded_matrix_b_1[z][j] + sub_expanded_matrix_b_2[z][j]);
				intermediate_matrix_7[i][j] += (sub_expanded_matrix_a_2[i][z] - sub_expanded_matrix_a_4[i][z]) * (sub_expanded_matrix_b_3[z][j] + sub_expanded_matrix_b_4[z][j]);
			}


	//Создание вспомогательных матриц
	int** temp_matrix_1 = allocate_matrix(half_expanded_size, half_expanded_size);
	int** temp_matrix_2 = allocate_matrix(half_expanded_size, half_expanded_size);
	int** temp_matrix_3 = allocate_matrix(half_expanded_size, half_expanded_size);
	int** temp_matrix_4 = allocate_matrix(half_expanded_size, half_expanded_size);

	//Подсчет значений вспомогательных матриц из промежуточных
	for (short i = 0; i < half_expanded_size; i++)
		for (short j = 0; j < half_expanded_size; j++) {
			temp_matrix_1[i][j] = intermediate_matrix_1[i][j] + intermediate_matrix_4[i][j] - intermediate_matrix_5[i][j] + intermediate_matrix_7[i][j];
			temp_matrix_2[i][j] = intermediate_matrix_3[i][j] + intermediate_matrix_5[i][j];
			temp_matrix_3[i][j] = intermediate_matrix_2[i][j] + intermediate_matrix_4[i][j];
			temp_matrix_4[i][j] = intermediate_matrix_1[i][j] - intermediate_matrix_2[i][j] + intermediate_matrix_3[i][j] + intermediate_matrix_6[i][j];
		}


	//Создание результирующей матрицы
	int** result_matrix = allocate_matrix(expanded_size, expanded_size);

	// Занесение информации из вспомогательных матриц в результирующую
	for (short i = 0; i < half_expanded_size; i++)
		for (short j = 0; j < half_expanded_size; j++) {
			result_matrix[i][j] = temp_matrix_1[i][j];
			result_matrix[i][j + half_expanded_size] = temp_matrix_2[i][j];
			result_matrix[i + half_expanded_size][j] = temp_matrix_3[i][j];
			result_matrix[i + half_expanded_size][j + half_expanded_size] = temp_matrix_4[i][j];
		}


	//Выравнивание границ результирующей матрицы
	short aligned_n = MAX_ALIGNED_SIZE, aligned_m = MAX_ALIGNED_SIZE;
	for (short i = 0, counter_n, counter_m; i < expanded_size; i++) {
		counter_n = 0;
		counter_m = 0;

		for (short j = 0; j < expanded_size; j++){
			if (result_matrix[i][j] != 0) {
				counter_n++;
				aligned_n = MAX_ALIGNED_SIZE;
			}
			if (result_matrix[j][i] != 0) {
				counter_m++;
				aligned_m = MAX_ALIGNED_SIZE;
			}
		}

		if (counter_n == 0 && i < aligned_n)
				aligned_n = i;
		if (counter_m == 0 && i < aligned_m)
				aligned_m = i;
	}

	int** aligned_result_matrix = allocate_matrix(aligned_n, aligned_m);
	for (short i = 0; i < aligned_n; i++)
		for (short j = 0; j < aligned_m; j++)
			aligned_result_matrix[i][j] = result_matrix[i][j];

	print_matrix_with_text("Результирующая матрица", aligned_result_matrix, aligned_n, aligned_m);

	system("pause");


	//Очистка динамической памяти
	delete_matrix(matrix_a, n_a);
	delete_matrix(matrix_b, n_b);

	delete_matrix(expanded_matrix_a, expanded_size);
	delete_matrix(expanded_matrix_b, expanded_size);

	delete_matrix(sub_expanded_matrix_a_1, half_expanded_size);
	delete_matrix(sub_expanded_matrix_a_2, half_expanded_size);
	delete_matrix(sub_expanded_matrix_a_3, half_expanded_size);
	delete_matrix(sub_expanded_matrix_a_4, half_expanded_size);
	delete_matrix(sub_expanded_matrix_b_1, half_expanded_size);
	delete_matrix(sub_expanded_matrix_b_2, half_expanded_size);
	delete_matrix(sub_expanded_matrix_b_3, half_expanded_size);
	delete_matrix(sub_expanded_matrix_b_4, half_expanded_size);

	delete_matrix(intermediate_matrix_1, half_expanded_size);
	delete_matrix(intermediate_matrix_2, half_expanded_size);
	delete_matrix(intermediate_matrix_3, half_expanded_size);
	delete_matrix(intermediate_matrix_4, half_expanded_size);
	delete_matrix(intermediate_matrix_5, half_expanded_size);
	delete_matrix(intermediate_matrix_6, half_expanded_size);
	delete_matrix(intermediate_matrix_7, half_expanded_size);

	delete_matrix(temp_matrix_1, half_expanded_size);
	delete_matrix(temp_matrix_2, half_expanded_size);
	delete_matrix(temp_matrix_3, half_expanded_size);
	delete_matrix(temp_matrix_4, half_expanded_size);

	delete_matrix(result_matrix, expanded_size);
	delete_matrix(aligned_result_matrix, aligned_n);

	return 0;
}
