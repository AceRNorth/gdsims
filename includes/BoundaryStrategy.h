#ifndef GENERALMETAPOP_BOUNDARYSTRATEGY_H
#define GENERALMETAPOP_BOUNDARYSTRATEGY_H

#include "Point.h"

/**
 * Boundary strategy base class. Defines the interface for boundary-type classes. 
 * @details BoundaryStrategy classes implement methods specific to the boundary type of the model. The simulation area is assumed to have boundary conditions at x = ``side_x``, y = ``side_y``, which are calculated from provided input coordinates or assumed to be 1 otherwise. 
 */
class BoundaryStrategy {
public:
    /**
     * BoundaryStrategy constructor.
     * @param[in] side_x size of one side of the simulation area (x-axis)
     * @param[in] side_y size of one side of the simulation area (y-axis)
     */
    BoundaryStrategy(double side_x, double side_y): side_x(side_x), side_y(side_y) {};
    ~BoundaryStrategy() {}; 
    virtual double distance(const Point& p1, const Point& p2) = 0;
    virtual Point relative_pos(const Point &p1, const Point &p2) = 0;

protected:
    double side_x; /**< Size of one side of the simulation area (x-axis). */
    double side_y; /**< Size of one side of the simulation area (y-axis). */
};

/**
 * Implements the methods for toroidal boundary conditions.
 * @details The simulation area is assumed to have periodic boundary conditions at x = ``side_x``, y = ``side_y``.
 */
class ToroidalBoundaryStrategy: public BoundaryStrategy {
public:
    ToroidalBoundaryStrategy(double side_x, double side_y): BoundaryStrategy(side_x, side_y) {};
    double distance(const Point &p1, const Point &p2) override;
    Point relative_pos(const Point &p1, const Point &p2) override;
};

/**
 * Implements methods for edge boundary conditions.
 * @details The simulation area is assumed to have edge boundary conditions at x = ``side_x``, y = ``side_y``.
 */
class EdgeBoundaryStrategy: public BoundaryStrategy {
public:
    EdgeBoundaryStrategy(double side_x, double side_y): BoundaryStrategy(side_x, side_y) {};
    double distance(const Point& p1, const Point& p2) override;
    Point relative_pos(const Point &p1, const Point &p2) override;
};

#endif //GENERALMETAPOP_BOUNDARYSTRATEGY_H