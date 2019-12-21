#include <algorithm>
#include "DSU.h"
#include "travel.h"

void TravelProblem::addRoad(const int from, const int to, const double weight) {
	roads.push_back(Edge{ from, to, weight });
}

double TravelProblem::getRoadLength(const int from, const int to) const {
	for (const auto& road : roads) {
		if ((from == road.from && to == road.to) ||
			(to == road.from && from == road.to))
			return road.weight;
	}
	return -1;
}

double TravelProblem::approxSolution() {
	double result = 0;
	DSU dsu(n_cities);

	std::sort(roads.begin(), roads.end());
	for (const auto& road : roads) {
		int from = road.from;
		int to = road.to;
		double weight = road.weight;
		if (dsu.find(from) != dsu.find(to)) {
			result += weight;
			dsu.merge(from, to);
		}
	}
	return result;
}

double TravelProblem::exactSolution() {
	std::vector<int> cities(n_cities);
	for (size_t i = 0; i < cities.size(); i++)
		cities[i] = i;

	double result = std::numeric_limits<double>::max();
	do {
		double current_path = 0;
		int path_length = 0;
		for (size_t i = 0; i < cities.size() - 1; i++) {
			double weight = getRoadLength(cities[i], cities[i + 1]);
			if (weight >= 0) {
				current_path += weight;
				path_length++;
			}
		}
		if (path_length == n_cities - 1) {
			result = std::min(result, current_path);
		}
	} while (next_permutation(cities.begin(), cities.end()));
	return result;
}

size_t TravelProblem::getCityCount() const {
	return n_cities;
}
