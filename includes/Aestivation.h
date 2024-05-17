#ifndef GENERALMETAPOP_AESTIVATION_H
#define GENERALMETAPOP_AESTIVATION_H

#include <vector>
#include <array>
#include "Params.h"
#include "Patch.h"
#include "constants.h"

class Patch;

// Implement aestivative behaviour for the collection of mosquito sites. 
class Aestivation {
public:
	Aestivation(AestivationParams *params, int sites_size);
	void hide(std::vector<Patch*> &sites);
	void wake(int day, std::vector<Patch*> &sites);
	bool is_hide_time(int day);
	bool is_wake_time(int day);

private:
	double psi; // aestivation rate
	double mu_aes; // aestivation mortality
	int t_hide1; // start day of aestivation-entering period (day number of the year), not included
	int t_hide2; // end day of aestivation-entering period (day number of the year)
	int t_wake1; // start day of aestivation-waking period (day number of the year), not included
	int t_wake2; // end day of aestivation-waking period (day number of the year)

	// number of mated female mosquitoes F_{ij} with female genotype i and carrying mated male genotype j that have gone into
	// aestivation from each patch
	std::vector<std::array<std::array<long long int, constants::num_gen>, constants::num_gen>> aes_F;
};

#endif //GENERALMETAPOP_AESTIVATION_H