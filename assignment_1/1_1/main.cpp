/*
1_1.Очередь с динамическим буфером

Реализовать очередь с динамическим зацикленным буфером.
Обрабатывать команды push back и pop front.

Формат ввода
В первой строке количество команд n.n ≤ 1000000.
Каждая команда задаётся как 2 целых числа : a b.

a = 2 - pop front
a = 3 - push back

Если дана команда pop front, то число b - ожидаемое значение.
Если команда pop front вызвана для пустой структуры данных, то ожидается “ - 1”.

Формат вывода
Требуется напечатать YES - если все ожидаемые значения совпали.
Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
*/

#include <assert.h>
#include <iostream>

class Queue {
public:
	~Queue();

	// Проверка очереди на пустоту
	bool empty() const;
	// Проверка на переполнение текущего буфера
	bool full() const;
	// Добавление элемента
	void push(int value);
	// Извлечение элемента
	int pop();

private:
	// Указатель и размер динамического буфера
	int* data = nullptr;
	int size = 0;

	int currentSize = 0;

	int tail = 0;
	int head = 0;
};

Queue::~Queue() {
	delete[] data;
	data = nullptr;
}

bool Queue::empty() const {
	return (currentSize == 0);
}

bool Queue::full() const {
	return (currentSize == size);
}

void Queue::push(int value) {
	// Если очередь пуста
	if (empty()) {
		if (data == nullptr) 
			data = new int[++size];
	}
	else {
		// Если существующего буфера недостаточно
		if (full()) {
			size *= 2;
			int* newData = new int[size];
			for (int i = 0; i < currentSize; i++) {
				newData[i] = data[(head + i) % currentSize];
			}

			delete[] data;
			data = newData;
			head = 0;
			tail = currentSize;
		}
	}
	data[tail % size] = value;
	tail = tail % size + 1;
	currentSize++;
}

int Queue::pop() {
	if (empty()) {
		return -1;
	}
	else {
		int result = data[head % size];
		head = (head % size) + 1;
		currentSize--;
		return result;
	}
}

int main()
{
	int commands_count = 0;
	std::cin >> commands_count;

	Queue queue;
	for (int i = 0; i < commands_count; i++) {
		int command = 0, value = 0;
		std::cin >> command >> value;
		if (command == 3) {
			queue.push(value);
		}
		else if (command == 2) {
			if (queue.empty()) {
				if (value != -1) {
					std::cout << "NO" << std::endl;
					return 0;
				}
			}
			else if (queue.pop() != value) {
				std::cout << "NO" << std::endl;
				return 0;
			}
		}
	}
	std::cout << "YES" << std::endl;
	return 0;
}