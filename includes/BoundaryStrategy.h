#ifndef BOUNDARYSTRATEGY_H
#define BOUNDARYSTRATEGY_H

#include "Point.h"

class BoundaryStrategy {
public:
    BoundaryStrategy(double side): side(side) {};
    virtual double distance(const Point& p1, const Point& p2) = 0;

protected:
    double side;
};

class ToroidalBoundaryStrategy: public BoundaryStrategy {
public:
    ToroidalBoundaryStrategy(double side): BoundaryStrategy(side) {};
    double distance(const Point &p1, const Point &p2) override;
};

class EdgeBoundaryStrategy: public BoundaryStrategy {
public:
    EdgeBoundaryStrategy(double side): BoundaryStrategy(side) {};
    double distance(const Point& p1, const Point& p2) override;
};

#endif //BOUNDARYSTRATEGY_H