#include <algorithm>
#include <cmath>
#include "solver.h"
#include "travel.h"

void Solver::generateData(size_t count) {
	data.clear();
	for (size_t i = 0; i < count; i++) {
		const double epsilon = std::numeric_limits<double>::min();
		const double two_pi = 2.0 * 3.14159265358979323846;

		double u1, u2;
		do {
			u1 = rand() * (1.0 / RAND_MAX);
			u2 = rand() * (1.0 / RAND_MAX);
		} while (u1 <= epsilon);

		double z0, z1;
		z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
		z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
		data.push_back(std::make_pair(z0, z1));
	}
}

void Solver::solve(size_t count) {
	results.clear();
	size_t experiments_count = N_EXPERIMENTS;
	for (size_t experiment = 0; experiment < experiments_count; experiment++) {
		generateData(count);

		TravelProblem trip(count);
		for (int i = 0; i < count; i++) {
			for (int j = i + 1; j < count; j++) {
				double x = data[i].first, y = data[i].second;
				trip.addRoad(i, j, x * x + y * y);
			}
		}
		double exact_solution = trip.exactSolution();
		double approx_solution = trip.approxSolution();
		results.push_back(std::make_pair(exact_solution, approx_solution));
	}

	mean = 0;
	for (const auto& result : results) {
		mean += result.second / result.first;
	}
	mean /= experiments_count;

	stdev = 0;
	for (const auto& result : results) {
		stdev += pow((result.second / result.first) - mean, 2);
	}
	stdev = sqrt(stdev / experiments_count);

	std::cout << "Experiment N=" << count << ": ";
	std::cout << "mean=" << mean << " std=" << stdev << std::endl;
}

