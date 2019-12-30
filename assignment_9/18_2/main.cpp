/*

18_2. Выпуклая оболочка 2D
Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
Вариант 1. С помощью алгоритма Грэхема.
Вариант 2. С помощью алгоритма Джарвиса.
Обратите внимание: три и более точки из множества могут лежать на одной прямой.
Формат входного файла:
Количество точек, далее на каждой строке координаты точек x, y. Общее количество точек не превосходит 100000. Координаты точек - числа с плавающей точкой в диапазоне [-1000, 1000].
Формат выходного файла:
Выведите периметр выпуклой оболочки.

*/

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
	double x;
	double y;
	
	bool operator<(const Point& point) const {
		return ((x < point.x) 
			|| ((x == point.x) && (y > point.y)));
	}
};

// Check the orientation of ordered points (p, q, r)
int orientation(const Point& p, const Point& q, const Point& r) { 
	double val = (q.y - p.y) * (r.x - q.x) - 
		(q.x - p.x) * (r.y - q.y); 

	if (val == 0.) {
		return 0;  // collinear
	}

	return (val > 0.) ? 1 : 2; // clock or counterclock wise 
}

double distance(const Point& p1, const Point& p2) {
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) 
		+ (p1.y - p2.y) * (p1.y - p2.y));
}

std::vector<Point> convexHull(const std::vector<Point>& pts) { 
	// Check the number of points
	size_t point_count = pts.size();
	assert(point_count >= 3);

	// Find start (the leftmost) point 
	size_t start = 0; 
	for (size_t i = 1; i < point_count; i++) {
		if (pts[i] < pts[start]) {
			start = i;
		}
	}

	// Start from the leftmost point and keep moving until we meet the start point again  
	size_t current = start, next; 
	std::vector<Point> hull; 
	do { 

		hull.push_back(pts[current]); 
		next = (current + 1) % point_count; 
		for (size_t i = 0; i < point_count; i++) { 
			if (orientation(pts[current], pts[i], pts[next]) == 2
				|| ((orientation(pts[current], pts[i], pts[next]) == 0)
				&& distance(pts[current], pts[i]) > distance(pts[current], pts[next]))) {
				next = i; 
			}
		} 
		current = next; 
		
	} while (current != start); 

	return hull; 
}

double calcPerimeter(const std::vector<Point>& points) {
	double perimeter = 0.;
	for (size_t i = 0; i < points.size(); i++) {
		if (i < points.size() - 1) {
			perimeter += distance(points[i], points[i + 1]);
		}
		else {
			perimeter += distance(points[i], points[0]);
		}
	}

	return perimeter;
}

int main() {
	size_t point_count = 0;
	std::cin >> point_count;

	std::vector<Point> points(point_count);
	for (size_t i = 0; i < point_count; i++) {
		std::cin >> points[i].x >> points[i].y;
	}

	std::vector<Point> hull = convexHull(points);
	std::cout << std::setprecision(9);
	std::cout << calcPerimeter(hull) << std::endl;

	return 0;
}
