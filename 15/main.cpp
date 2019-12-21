#include "solver.h"

int main() {

	Solver travel_solver;
	for (size_t i = 2; i <= 10; i++) {
		travel_solver.solve(i);
	}

	return 0;
}
