#include <array>
#include <vector>
#include "Aestivation.h"
#include "random.h"
#include "constants.h"

/**
 * Aestivation constructor.
 * @param params 		aestivation parameters
 * @param sites_size	number of sites in the model, size of Model::sites
 * @see @ref Model::sites
 */
Aestivation::Aestivation(AestivationParams* params, int sites_size) 
{
	psi = params->psi;
	mu_aes = params->mu_aes;
	t_hide1 = params->t_hide1;
	t_hide2 = params->t_hide2;
	t_wake1 = params->t_wake1;
	t_wake2 = params->t_wake2;

	aes_F.clear();

	std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f;
	for (int i=0; i < constants::num_gen; ++i) {
		for (int j=0; j < constants::num_gen; ++j) {
			f[i][j] = 0;
		}
	}

	for (int pat=0; pat < sites_size; ++pat) {
		aes_F.push_back(f);
	}
}

/**
 * Hides aestivating females from their patches.
 * @note Not all females survive aestivation. Only mated females aestivate. 
 * @details The number of females that attempt to go into aestivation (of the given genotype combination) is determined by a binomial distribution of the aestivation rate. Of those, the number that survive is determined by a binomial distribution of 1 - mu_aes (the aestivation mortality). Aestivating females are temporarily separated from the rest of the female patch population.
 * @param[in, out] sites vector of all Patch objects
 * @see Aestivation::psi, Aestivation::mu_aes
 */
void Aestivation::hide(std::vector<Patch*> &sites) 
{
	std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f;
	std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f_try;
	std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f_aes;
	for (int pat=0; pat < sites.size(); ++pat) {
		f = sites[pat]->get_F();
		for (int i=0; i < constants::num_gen; ++i) {
			for (int j=0; j < constants::num_gen; ++j) {
				f_try[i][j] = random_binomial(f[i][j], psi); // number of females that attempt to go into aestivation
				f_aes[i][j] = random_binomial(f_try[i][j], 1 - mu_aes);	// number that survive going into aestivation
				aes_F[pat][i][j] += f_aes[i][j]; // move aestivating females into a separate box variable
			}
		}
		sites[pat]->F_hide(f_try); // update population numbers
	}
}

/**
 * Wakes a fraction of the aestivating females.
 * @details The number of females that wake up on the given day (for the given genotype combination and patch) is determined by a binomial distribution with probability 1.0 / (1.0 + t_wake2 - (day%365). Those that wake are returned to their patch's female population. 
 * @param[in] day 			simulation day
 * @param[in, out] sites	vector of all Patch objects
 * @see Aestivation::t_wake2
 */ 
void Aestivation::wake(int day, std::vector<Patch*> &sites) 
{
	double prob = 1.0 / (1.0 + t_wake2 - (day%365)); // probability of a female waking on a given day
	
	std::array<std::array<long long int, constants::num_gen>, constants::num_gen> f_w;
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < constants::num_gen; ++i) {
			for(int j=0; j < constants::num_gen; ++j) {
				// number of females that wake up from aestivation on the given day
				f_w[i][j] = random_binomial(aes_F[pat][i][j], prob);
				aes_F[pat][i][j] -= f_w[i][j];
			}
		}
		sites[pat]->F_wake(f_w);
	}
}

/**
 * Determines if the time is within the aestivation hiding window. 
 * @note The hiding window is exclusive of the start time but inclusive of the end time. 
 * @param[in] day 			simulation day
 * @return As you would expect.
 * @see Aestivation::t_hide1, Aestivation::t_hide2, Aestivation::psi
 */ 
bool Aestivation::is_hide_time(int day) 
{
	return day%365 > t_hide1 && day%365 <= t_hide2 && psi > 0.00001;
}

/**
 * Determines if the time is within the aestivation waking window. 
 * @note The wakinf window is exclusive of the start time but inclusive of the end time. 
 * @param[in] day 			simulation day
 * @return As you would expect.
 * @see Aestivation::t_wake1, Aestivation::t_wake2, Aestivation::psi
 */ 
bool Aestivation::is_wake_time(int day) 
{
	return day%365 > t_wake1 && day%365 <= t_wake2 && psi > 0.00001;
}