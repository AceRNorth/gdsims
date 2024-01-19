#ifndef DISPERSAL_H
#define DISPERSAL_H

#include <vector>
#include <array>
#include "constants.h"
#include "Params.h"
#include "Patch.h"

using namespace constants;

class Patch;

// Implements dispersion across all mosquito sites in the collection. 
class Dispersal {
public:
	Dispersal(DispersalParams* params);
	void set_connecs(double side, std::vector<Patch*> &sites);
	void adults_disperse(std::vector<Patch*> &sites);

private:
	double disp_rate; // adult dispersal rate
	double max_disp; // maximum distance at which two sites are connected (km)

	// connected patch indices ordered by each patch in sites, such that the first element contains the indices of all the patches
	// connected to the first sites patch, second element has all connection indices to the second sites patch, etc.
	std::vector<std::vector<int>> connec_indices; 
	// connection weights of the connected patches ordered by each patch in sites, such that the first element contains the connection
	// weights between the first patch in sites and all the patches connected to it, the second element has all connection weights
	// between the second sites element and all other patches connected to it, etc.
	std::vector<std::vector<double>> connec_weights; 

	double distance(double side, std::array<double, 2> point1, std::array<double, 2> point2);
	std::vector<std::array<long long int, num_gen>> M_dispersing_out(const std::vector<Patch*> &sites);
	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> F_dispersing_out(const std::vector<Patch*> &sites);
};

#endif //DISPERSAL_H