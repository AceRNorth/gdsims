#ifndef SEASONALITY_H
#define SEASONALITY_H

class Seasonality {
public:
    Seasonality(double alpha1, double amp): alpha1(alpha1), amp(amp) {}
    double alpha(int day, double alpha0);

private:
    // seasonality parameters
    double alpha1; //comment?
    double amp; //comment?
};

#endif //SEASONALITY_H