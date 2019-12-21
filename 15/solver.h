#ifndef solver_h
#define solver_h

#include <iostream>
#include <vector>

static const size_t N_EXPERIMENTS = 10;

class Solver {
public:
	void solve(size_t samples);

private:
	void generateData(size_t count);

	std::vector< std::pair<double, double> > data;
	std::vector< std::pair<double, double> > results;
	double mean, stdev;
};

#endif /* solver_h */
