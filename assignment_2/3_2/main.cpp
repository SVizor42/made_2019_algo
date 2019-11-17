/*

3_1.Первые k элементов длинной последовательности

Дана очень длинная последовательность целых чисел длины n. Требуется вывести в отсортированном виде её наименьшие k элементов.
Последовательность может не помещаться в память.Время работы O(n* log(k)). Доп.память O(k). Использовать слияние.

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory.h>

void Merge(int* leftArr, int leftLen, int* rightArr, int rightLen, int* buffer) {
	int i = 0, j = 0;
	while (i < leftLen && j < rightLen) {
		if (leftArr[i] <= rightArr[j]) {
			buffer[i + j] = leftArr[i];
			i++;
		}
		else {
			buffer[i + j] = rightArr[j];
			j++;
		}
	}

	if (i < leftLen) {
		while (i < leftLen) {
			buffer[i + j] = leftArr[i];
			i++;
		}
	}
	else {
		while (j < rightLen) {
			buffer[i + j] = rightArr[j];
			j++;
		}
	}
}

void MergeSort(int* arr, int len) {
	if (len <= 1) {
		return;
	}
	int leftLen = len / 2;
	int rightLen = len - leftLen;
	MergeSort(arr, leftLen);
	MergeSort(arr + leftLen, rightLen);

	int* buffer = new int[len];
	Merge(arr, leftLen, arr + leftLen, rightLen, buffer);
	memcpy(arr, buffer, sizeof(int) * len);

	delete[] buffer;
}

int main()
{
	int n = 0, k = 0;
	std::cin >> n >> k;
	
	std::vector<int> numbers(n);
	for (int i = 0; i < n; i++) {
		std::cin >> numbers[i];
	}

	// Пробегаем сортировкой слиянием по исходному массиву с шагом k скользящим окном шириной в 2*k 
	for (int i = 0; i < n; i += k) {
		MergeSort(&numbers[i], std::min(2*k, n - i));
	}

	for (int j = 0; j < n; j++) {
		std::cout << numbers[j] << " ";
	}
	std::cout << std::endl;

	return 0;
}
