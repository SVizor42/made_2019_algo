/*


4. Порядковая статистика

Даны неотрицательные целые числа n, k и массив целых чисел из диапазона[0..109] размера n.
Требуется найти k - ю порядковую статистику.т.е.напечатать число, которое бы стояло на позиции с индексом k ∈[0..n - 1] в отсортированном массиве.
Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти : O(n).
Требуемое среднее время работы : O(n).
Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.

4_4.Реализуйте стратегию выбора опорного элемента “случайный элемент”.
Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

int getRandomPivot(int n) {
	return std::rand() % n;
}

int Partition(int* arr, int len, int k) {
	std::swap(arr[0], arr[k]);

	int i = len - 1;
	for (int j = i; j > 0; j--) {
		if (arr[j] > arr[0] || (arr[j] == arr[0] && j % 2 == 0)) {
			std::swap(arr[j], arr[i]);
			i--;
		}
	}

	std::swap(arr[0], arr[i]);
	return i;
}

int getKStatistics(int* arr, int len, int k) {
	int pivot = 0;
	for (;;) {
		pivot = getRandomPivot(len);
		pivot = Partition(arr, len, k);
		if (pivot == k) {
			return arr[pivot];
		}
		if (pivot > k) {
			len = pivot;
		}
		else {
			arr += (pivot + 1);
			len -= (pivot + 1);
			k -= (pivot + 1);
		}
	}
}

int main()
{
	int n = 0, k = 0;
	std::cin >> n >> k;

	std::vector<int> numbers(n);
	for (int i = 0; i < n; i++) {
		std::cin >> numbers[i];
	}

	std::cout << getKStatistics(&numbers[0], n, k) << std::endl;
}