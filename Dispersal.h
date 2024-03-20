#ifndef DISPERSAL_H
#define DISPERSAL_H

#include <vector>
#include <array>
#include <utility>
#include "constants.h"
#include "Params.h"
#include "Patch.h"
#include "Point.h"
#include "BoundaryStrategy.h"

using namespace constants;

class Patch;

// Implements dispersion across all mosquito sites in the collection. 
class Dispersal {
public:
	Dispersal(DispersalParams* params, BoundaryType boundary, double side);
	~Dispersal();
	virtual void set_connecs(std::vector<Patch*> &sites) = 0;
	virtual void adults_disperse(std::vector<Patch*> &sites) = 0;

protected:
	double disp_rate; // adult dispersal rate
	double max_disp; // maximum distance at which two sites are connected (km)

	// connected patch indices ordered by each patch in sites, such that the first element contains the indices of all the patches
	// connected to the first sites patch, second element has all connection indices to the second sites patch, etc.
	std::vector<std::vector<int>> connec_indices; 
	// connection weights of the connected patches ordered by each patch in sites, such that the first element contains the connection
	// weights between the first patch in sites and all the patches connected to it, the second element has all connection weights
	// between the second sites element and all other patches connected to it, etc.
	std::vector<std::vector<double>> connec_weights; 

	BoundaryStrategy* boundary_strategy;

	std::vector<std::array<long long int, num_gen>> M_dispersing_out(const std::vector<Patch*> &sites);
	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> F_dispersing_out(const std::vector<Patch*> &sites);
};


class DistanceKernelDispersal: public Dispersal {
public:
	DistanceKernelDispersal(DispersalParams* params, BoundaryType boundary, double side): Dispersal(params, boundary, side) {};
	void set_connecs(std::vector<Patch*> &sites) override;
	void adults_disperse(std::vector<Patch*> &sites) override;

private:
	std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> compute_connecs(std::vector<Patch*> &sites);
};

class WedgeDispersal: public Dispersal {
public:
	WedgeDispersal(DispersalParams* params, BoundaryType boundary, double side): Dispersal(params, boundary, side) {};
	void set_connecs(std::vector<Patch*> &sites) override;
	void adults_disperse(std::vector<Patch*> &sites) override;

private:
	std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> compute_connecs(std::vector<Patch*> &sites);
	std::pair<std::vector<std::pair<double, double>>, double> compute_interval_union(const std::pair<double, double>& qq,
        const std::vector<std::pair<double, double>>& input);
    double wrap_around(double value, double range); 
    std::vector<int> get_sorted_positions(const std::vector<double>& numbers);
    std::vector<std::vector<double>> compute_distances(const std::vector<Patch*> &sites);
};

#endif //DISPERSAL_H