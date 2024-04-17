#include <cmath>
#include "Seasonality.h"
#include "constants.h"

using namespace constants;

SineRainfall::SineRainfall(double alpha1, double ampl): Seasonality(alpha1) 
{
    amp = ampl;
}

// Computes the carrying-capacity alpha value for the given day and alpha0. Models rainfall contribution as a sine wave.
double SineRainfall::alpha(int day, double alpha0)
{
    double alpha = alpha0 + alpha1*(1 + amp*std::sin(2 * PI * day/365));
	return alpha;
}

InputRainfall::InputRainfall(double alpha1, double res, std::vector<double> rain): Seasonality(alpha1) 
{
    resp = res;

    if (!rain.empty()) {
        rainfall = rain;
    }
    else {
        for (int i = 0; i < 365; ++i) {
            rainfall.push_back(0);
        }
    }
}

// Computes the carrying-capacity alpha value for the given day and alpha0. Models rainfall contribution as an exponential.
double InputRainfall::alpha(int day, double alpha0)
{
    double r_d;
    if (rainfall.size() == 365) {
        r_d = rainfall.size() > 365 ? rainfall.at(day) : rainfall.at(day%365);
    }
    else {
        r_d = rainfall.at(day);
    }
    double alpha = alpha0 + alpha1*(1 - std::exp(-resp * r_d));
    return alpha;
}