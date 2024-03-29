#include <cmath>
#include "Seasonality.h"
#include "constants.h"

using namespace constants;

// Computes the carrying-capacity alpha value for the given day and alpha0. Models rainfall contribution as a sine wave.
double Seasonality::alpha(int day, double alpha0)
{
    double alpha = alpha0 + alpha1*(1 + amp*std::sin(2 * PI * (day/365)));
	return alpha;
}