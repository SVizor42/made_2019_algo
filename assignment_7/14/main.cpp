/*

14. Минимальное остовное дерево
Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
Вариант 1. С помощью алгоритма Прима.
Формат ввода
Первая строка содержит два натуральных числа n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000).
Следующие m строк содержат описание ребер по одному на строке.
Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).
Формат вывода
Выведите единственное целое число - вес минимального остовного дерева.

*/

#include <iostream>
#include <set>
#include <vector>

const int MAX_WEIGHT = 1000000;

using Graph = std::vector< std::vector< std::pair<size_t, size_t> > >;

size_t MST_solver(Graph& graph) {
	std::vector<bool> used(graph.size(), false);
	std::vector<int> min_weight(graph.size(), MAX_WEIGHT);
	min_weight[0] = 0;
	std::set< std::pair<size_t, size_t> > queue;
	queue.insert(std::make_pair(0, 0));
	for (size_t i = 0; i < graph.size(); i++) {
		size_t vertex = queue.begin()->second;
		queue.erase(queue.begin());
		used[vertex] = true;

		for (size_t j = 0; j < graph[vertex].size(); j++) {
			size_t to = graph[vertex][j].first, weight = graph[vertex][j].second;
			if (!used[to] && weight < min_weight[to]) {
				queue.erase(std::make_pair(min_weight[to], to));
				min_weight[to] = weight;
				queue.insert(std::make_pair(min_weight[to], to));
			}
		}
	}

	size_t result = 0;
	for (auto& weight : min_weight) {
		result += weight;
	}

	return result;
}

int main() {
	size_t n = 0, m = 0;
	std::cin >> n >> m;

	Graph graph(n);
	size_t from = 0, to = 0, weight = 0;
	for (size_t i = 0; i < m; i++) {
		std::cin >> from >> to >> weight;
		graph[from - 1].push_back(std::make_pair(to - 1, weight));
		graph[to - 1].push_back(std::make_pair(from - 1, weight));
	}

	std::cout << MST_solver(graph) << std::endl;
}