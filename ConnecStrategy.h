#ifndef CONNECTIONSTRATEGY_H
#define CONNECTIONSTRATEGY_H

#include <vector>

#include "BoundaryStrategy.h"
#include "Patch.h"

class ConnecStrategy {
public:
    ConnecStrategy(double max_disp, BoundaryStrategy* boundary);
    virtual std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> compute_connecs(std::vector<Patch*> &sites) = 0;

protected:
    BoundaryStrategy* boundary_strategy;
    double max_disp;
};

// Connection strategy representing connection weight as a relative distance value between patches.
class SimpleConnecStrategy: public ConnecStrategy {
public:
    SimpleConnecStrategy(double max_disp, BoundaryStrategy* boundary): ConnecStrategy(max_disp, boundary) {}
    std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> compute_connecs(std::vector<Patch*> &sites) override;
};

// Connection strategy representing connections through individual patch connection radii and occupied wedges in the connection circle.
class WedgeConnecStrategy: public ConnecStrategy {
public:
    WedgeConnecStrategy(double max_disp, BoundaryStrategy* boundary): ConnecStrategy(max_disp, boundary) {}
    std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> compute_connecs(std::vector<Patch*> &sites) override;

private:
    std::pair<std::vector<std::pair<double, double>>, double> compute_interval_union(const std::pair<double, double>& qq,
        const std::vector<std::pair<double, double>>& input);
    double wrap_around(double value, double range); 
    std::vector<int> get_sorted_positions(const std::vector<double>& numbers);
    std::vector<std::vector<double>> compute_distances(const std::vector<Patch*> &sites);
};

#endif //CONNECTIONSETTINGSTRATEGY_H