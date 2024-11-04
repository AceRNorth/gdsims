#ifndef GENERALMETAPOP_BOUNDARYSTRATEGY_H
#define GENERALMETAPOP_BOUNDARYSTRATEGY_H

#include "Point.h"

/**
 * Boundary strategy base class. Defines the interface for boundary-type classes. 
 * @details BoundaryStrategy classes implement methods specific to the boundary type of the model. The simulation area is assumed to have boundary conditions at x = ``side``, y = ``side``, and have ranges [0, ``side``] in all directions.
 * @see AreaParams::side
 */
class BoundaryStrategy {
public:
    /**
     * BoundaryStrategy constructor.
     * @param[in] side size of one side of the simulation square
     */
    BoundaryStrategy(double side): side(side) {};
    ~BoundaryStrategy() {}; 
    virtual double distance(const Point& p1, const Point& p2) = 0;
    virtual Point relative_pos(const Point &p1, const Point &p2) = 0;

protected:
    double side; /**< Size of one side of the simulation square. */
};

/**
 * Implements the methods for toroidal boundary conditions.
 * @details The simulation area is assumed to have periodic boundary conditions at x = ``side``, y = ``side``, and have ranges [0, ``side``] in all directions.
 * @see AreaParams::side
 */
class ToroidalBoundaryStrategy: public BoundaryStrategy {
public:
    ToroidalBoundaryStrategy(double side): BoundaryStrategy(side) {};
    double distance(const Point &p1, const Point &p2) override;
    Point relative_pos(const Point &p1, const Point &p2) override;
};

/**
 * Implements methods for edge boundary conditions.
 * @details The simulation area is assumed to have edge boundary conditions at x = ``side``, y = ``side``, and have ranges [0, ``side``] in all directions.
 * @see AreaParams::side
 */
class EdgeBoundaryStrategy: public BoundaryStrategy {
public:
    EdgeBoundaryStrategy(double side): BoundaryStrategy(side) {};
    double distance(const Point& p1, const Point& p2) override;
    Point relative_pos(const Point &p1, const Point &p2) override;
};

#endif //GENERALMETAPOP_BOUNDARYSTRATEGY_H