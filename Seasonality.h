#ifndef SEASONALITY_H
#define SEASONALITY_H

#include <vector>

class Seasonality {
public:
    Seasonality(double alpha1): alpha1(alpha1) {}
    virtual double alpha(int day, double alpha0) = 0;

protected:
    double alpha1; // carrying capacity factor due to rainfall contribution
};

class SineRainfall: public Seasonality {
public:
    SineRainfall(double alpha1, double amp);
    double alpha(int day, double alpha0) override;

private:
    double amp;
};

class InputRainfall: public Seasonality {
public:
    InputRainfall(double alpha1, double res, std::vector<double> rain);
    double alpha(int day, double alpha0) override;

private:
    double resp; // carrying capacity's responsiveness to rainfall factor
    std::vector<double> rainfall; // daily rainfall for every day of the year
};

#endif //SEASONALITY_H