#ifndef GENERALMETAPOP_SEASONALITY_H
#define GENERALMETAPOP_SEASONALITY_H

#include <vector>
#include "Params.h"

/**
 * Seasonality base class. Defines the interface for seasonality classes.
 */
class Seasonality {
public:
    /** Seasonality constructor.
     * @param[in] alpha1 carrying capacity factor accounting for rainfall contribution
     */
    Seasonality(double alpha1): alpha1(alpha1) {}
    virtual ~Seasonality() {}
    virtual double alpha(int day, double alpha0) = 0;

protected:
    double alpha1; /**< Carrying capacity factor accounting for rainfall contribution. */ 
};

/**
 * Implements seasonality by modelling the rainfall contribution to carrying capacity as a sinusoid wave. 
 * @details Most useful for theoretical applications. 
 */
class SineRainfall: public Seasonality {
public:
    SineRainfall(SineRainfallParams *params);
    ~SineRainfall() {};
    double alpha(int day, double alpha0) override;

private:
    double amp; /**< Amplitude of rainfall fluctuations. */ 
};

/**
 * Implements seasonality by modelling the rainfall contribution to carrying capacity from daily rainfall data. 
 * @note This class expects rainfall data for either every day of a year (365 days) or every day of the simulation (max_t).Annual data will be automatically looped for simulations longer than a year.
 */
class InputRainfall: public Seasonality {
public:
    InputRainfall(InputRainfallParams *params);
    ~InputRainfall() {};
    double alpha(int day, double alpha0) override;

private:
    double resp; /**< Carrying capacity's responsiveness to the rainfall contribution. */ 
    std::vector<double> rainfall; /**< Daily rainfall data. */
};

#endif //GENERALMETAPOP_SEASONALITY_H