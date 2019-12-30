#ifndef travel_h
#define travel_h

#include <iostream>
#include <vector>

class TravelProblem {
public:
	explicit TravelProblem(size_t count) : n_cities(count) {};
	TravelProblem(const TravelProblem& copied) = delete;
	TravelProblem& operator=(const TravelProblem& copied) = delete;
	TravelProblem(TravelProblem&& moved) = delete;
	TravelProblem& operator=(TravelProblem&& moved) = delete;
	~TravelProblem() = default;

	void addRoad(const int from, const int to, const double weight);
	double approxSolution();
	double exactSolution();
	size_t getCityCount() const;

private:
	struct Edge {
		int from;
		int to;
		double weight;

		bool operator<(const Edge& other) const { return weight < other.weight; }
	};

	double getRoadLength(const int from, const int to) const;

	std::vector<Edge> roads;
	size_t n_cities;
};

#endif /* travel_h */
