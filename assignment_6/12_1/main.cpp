/*

12. Мосты
Ребро неориентированного графа называется мостом, если удаление этого ребра из графа увеличивает число компонент связности.
Дан неориентированный граф, требуется найти в нем все мосты.

Ввод: Первая строка входного файла содержит два целых числа n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 1 ≤ m ≤ 200000).
Следующие m строк содержат описание ребер по одному на строке. Ребро номер i описывается двумя натуральными числами bi, ei — номерами концов ребра (1 ≤ bi, ei ≤ n).
Вывод: Первая строка выходного файла должна содержать одно натуральное число b — количество мостов в заданном графе. На следующей строке выведите b целых чисел — номера ребер, которые являются мостами, в возрастающем порядке. Ребра нумеруются с единицы в том порядке, в котором они заданы во входном файле.

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>

struct Edge {
	int to;
	int id;
};

struct Frame {
	int to;
	int from;
	int index;
};

using Graph = std::vector< std::vector<Edge> >;

class BridgeSolver {
public:
	BridgeSolver(Graph _graph);
	std::vector<size_t> FindBridges();
	void DFS(int v);

private:
	Graph graph;
	std::vector<size_t> bridges;
	std::vector<char> used;
	std::vector<int> entry;
	std::vector<int> lowest;
	size_t timer;
};

BridgeSolver::BridgeSolver(Graph _graph)
	: graph(_graph)
	, used(_graph.size(), false)
	, entry(_graph.size(), -1)
	, lowest(_graph.size(), -1)
	, timer(0)
{}

std::vector<size_t> BridgeSolver::FindBridges() {
	for (int i = 0; i < graph.size(); i++)
		used[i] = false;

	for (int i = 0; i < graph.size(); i++)
		if (!used[i])
			DFS(i);

	std::sort(bridges.begin(), bridges.end());
	return bridges;
}

void BridgeSolver::DFS(int v) {
	std::stack<Frame> vertex_stack;
	int to = -1;
	int parent = -1;
	int index = 0;
	bool bottom = false;
	for ( ; ; ) {
		if (index) {
			lowest[v] = std::min(lowest[v], lowest[to]);
			if (lowest[to] > entry[v]) {
				bridges.push_back(graph[v][index - 1].id);
			}
			index++;
		}
		else {
			used[v] = true;
			entry[v] = lowest[v] = timer++;
			index = 1;
		}

		bottom = false;
		while (index <= graph[v].size()) {
			Edge edge = graph[v][index - 1];
			to = edge.to;
			if (edge.id == parent) {
				index++;
				continue;
			}
			if (used[to]) {
				lowest[v] = std::min(lowest[v], entry[to]);
				index++;
			}
			else {
				vertex_stack.push(Frame{ v, parent, index - 1 });
				parent = edge.id;
				v = to;
				index = 0;
				bottom = true;
				break;
			}
		}

		if (bottom) {
			continue;
		}

		if (vertex_stack.size()) {
			Frame frame(vertex_stack.top());
			vertex_stack.pop();
			to = v;
			v = frame.to;
			parent = frame.from;
			index = frame.index + 1;
		}
		else {
			break;
		}
	}
}

int main() {

	std::ifstream in("bridges.in");
	std::ofstream out("bridges.out");

	int n = 0, m = 0;
	in >> n >> m;

	Graph graph(n);
	int from = 0, to = 0;
	for (int i = 0; i < m; i++) {
		in >> from >> to;
		graph[from - 1].push_back(Edge{ to - 1, i + 1 });
		graph[to - 1].push_back(Edge{ from - 1, i + 1 });
	}

	BridgeSolver solver(graph);
	std::vector<size_t> result = solver.FindBridges();

	out << result.size() << std::endl;
	for (auto bridge : result) {
		out << bridge << " ";
	}

	return 0;
}