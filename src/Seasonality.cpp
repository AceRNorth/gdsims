#include <cmath>
#include <vector>
#include "Seasonality.h"
#include "constants.h"

SineRainfall::SineRainfall(SineRainfallParams *params): Seasonality(params->alpha1) 
{
    amp = params->amp;
}

// Computes the carrying-capacity alpha value for the given day and alpha0. Models rainfall contribution as a sine wave.
double SineRainfall::alpha(int day, double alpha0)
{
    double alpha = alpha0 + alpha1*(1 + amp*std::sin(2 * constants::pi * day/365));
	return alpha;
}

InputRainfall::InputRainfall(InputRainfallParams *params): Seasonality(params->alpha1) 
{
    resp = params->resp;

    if (!((params->rainfall).empty())) {
        rainfall = params->rainfall;
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