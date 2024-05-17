#ifndef GENERALMETAPOP_SEASONALITY_H
#define GENERALMETAPOP_SEASONALITY_H

#include <vector>
#include "Params.h"

class Seasonality {
public:
    Seasonality(double alpha1): alpha1(alpha1) {}
    virtual double alpha(int day, double alpha0) = 0;

protected:
    double alpha1; // carrying capacity factor due to rainfall contribution
};

class SineRainfall: public Seasonality {
public:
    SineRainfall(SineRainfallParams *params);
    double alpha(int day, double alpha0) override;

private:
    double amp; // amplitude of rainfall fluctuations
};

class InputRainfall: public Seasonality {
public:
    InputRainfall(InputRainfallParams *params);
    double alpha(int day, double alpha0) override;

private:
    double resp; // carrying capacity's responsiveness to rainfall contribution
    std::vector<double> rainfall; // daily rainfall for every day of the year
};

#endif //GENERALMETAPOP_SEASONALITY_H