#include <cmath>
#include "BoundaryStrategy.h"

// Calculates the periodic distance between two points in the simulation area with boundaries x = side, y = side
double ToroidalBoundaryStrategy::distance(const Point &p1, const Point &p2)
{
    double x_dist = 0;
	double y_dist = 0;

	if (std::abs(p1.x - p2.x) > side - std::abs(p1.x - p2.x)) {
		x_dist = side - std::abs(p1.x - p2.x);
	} 
	else {
		x_dist = std::abs(p1.x - p2.x);
	}

	if (std::abs(p1.y - p2.y) > side - std::abs(p1.y - p2.y)) {
		y_dist = side - std::abs(p1.y - p2.y);
	}
	else {
		y_dist = std::abs(p1.y - p2.y);
	}

	return std::sqrt((x_dist * x_dist) + (y_dist * y_dist));
}


// Calculates the distance between two points
double EdgeBoundaryStrategy::distance(const Point &p1, const Point &p2)
{
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}