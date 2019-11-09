/*
2_2.Старый компьютер

Для сложения чисел используется старый компьютер.
Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
Таким образом для нахождения суммы чисел 1, 2, 3 может потребоваться разное время, в зависимости от порядка вычислений. 
((1 + 2) + 3) -> 1 + 2 + 3 + 3 = 9 ((1 + 3) + 2) -> 1 + 3 + 4 + 2 = 10 ((2 + 3) + 1) -> 2 + 3 + 5 + 1 = 11 
Требуется написать программу, которая определяет минимальное время, достаточное для вычисления суммы заданного набора чисел.
Требуемое время работы O(n * log(n)).

Формат ввода
Вначале вводится n(n < 100) - количество чисел.Затем вводится n строк - значения чисел(значение каждого числа 
не превосходит 1 000 000 000, сумма всех чисел не превосходит 2 * 1 000 000 000).

Формат вывода
Натуральное число - минимальное время.
*/

#include <vector>
#include <algorithm>
#include <iostream>

class MinHeap {
public:
	MinHeap();
	void Add(int node);
	void SiftUp(int node);
	void SiftDown(int node);
	int getParent(int node);
	int getLeftChild(int node);
	int getRightChild(int node);
	int getMin();
	int size();

private:
	std::vector<int> heap;
};

MinHeap::MinHeap() {
	heap.resize(0);
}

void MinHeap::Add(int node) {
	heap.push_back(node);
	SiftUp(size() - 1);
}

void MinHeap::SiftUp(int node) {
	if (node) {
		int parent = getParent(node);
		if (heap[parent] > heap[node]) {
			std::swap(heap[node], heap[parent]);
			SiftUp(parent);
		}
	}
}

void MinHeap::SiftDown(int node) {
	int left = getLeftChild(node);
	int right = getRightChild(node);
	int min = node;
	if (left < size() && heap[left] < heap[node]) {
		min = left;
	}
	if (right < size() && heap[right] < heap[min]) {
		min = right;
	}
	if (min != node)
	{
		std::swap(heap[node], heap[min]);
		SiftDown(min);
	}
}

int MinHeap::getParent(int node) {
	if (!node) {
		return 0;
	}
	else {
		return (node - 1) / 2;
	}
}

int MinHeap::getLeftChild(int node) {
	return 2 * node + 1;
}

int MinHeap::getRightChild(int node) {
	return 2 * node + 2;
}

int MinHeap::getMin() {
	int result = heap[0];
	std::swap(heap[0], heap[size() - 1]);
	heap.pop_back();
	SiftDown(0);
	return result;
}

int MinHeap::size() {
	return heap.size();
}

int solve(MinHeap& heap) {
	int result = 0;
	if (heap.size() > 1) {
		// Пока в Минкуче есть элементы, будем вынимать из нее 2 самых наименьших, 
		// находить их сумму и возвращать полученную сумму обратно в кучу
		int min = 0;
		while (heap.size()) {
			min = heap.getMin();
			if (heap.size()) {
				min += heap.getMin();
				result += min;
				heap.Add(min);
			}
		}
	}
	else {
		result = heap.getMin();
	}
	return result;
}

int main() {
	int numbers_count = 0;
	std::cin >> numbers_count;;

	int number;
	MinHeap heap;
	for (int i = 0; i < numbers_count; i++) {
		std::cin >> number;
		heap.Add(number);
	}

	std::cout << solve(heap) << std::endl;

	return 0;
}