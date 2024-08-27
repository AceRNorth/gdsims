#ifndef GENERALMETAPOP_BOUNDARYSTRATEGY_H
#define GENERALMETAPOP_BOUNDARYSTRATEGY_H

#include "Point.h"

/**
 * Boundary strategy base class. Defines the interface for boundary-type classes. 
 */
class BoundaryStrategy {
public:
    /**
     * BoundaryStrategy constructor.
     * @param[in] side size of one side of the simulation square
     */
    BoundaryStrategy(double side): side(side) {};
    virtual double distance(const Point& p1, const Point& p2) = 0;

protected:
    double side; /**< Size of one side of the simulation square. */
};

/**
 * Implements the distance method for toroidal boundary conditions.
 * @details The simulation area is assumed to have periodic boundary conditions at x = side, y = side, and have ranges [0, side] in all directions.
 */
class ToroidalBoundaryStrategy: public BoundaryStrategy {
public:
    ToroidalBoundaryStrategy(double side): BoundaryStrategy(side) {};
    double distance(const Point &p1, const Point &p2) override;
};

/**
 * Implements the distance method for edge boundary conditions.
 * @details The simulation area is assumed to have edge boundary conditions at x = side, y = side, and have ranges [0, side] in all directions.
 */
class EdgeBoundaryStrategy: public BoundaryStrategy {
public:
    EdgeBoundaryStrategy(double side): BoundaryStrategy(side) {};
    double distance(const Point& p1, const Point& p2) override;
};

#endif //GENERALMETAPOP_BOUNDARYSTRATEGY_H