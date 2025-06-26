#ifndef GENERALMETAPOP_AESTIVATION_H
#define GENERALMETAPOP_AESTIVATION_H

#include <vector>
#include <array>
#include "Params.h"
#include "Patch.h"
#include "constants.h"

class Patch;

/**
 * Implements aestivative behaviour for the model. 
 * @details Only adult mated females aestivate, going into aestivation within a hiding time window determined by ``t_hide1`` and ``t_hide2`` and emerging from aestivation within a waking time window determined by ``t_wake1`` and ``t_wake2``. The females are isolated from their original patches during aestivation and returned back at the end, dependent on mortality. Other parameter rates controlling aestivation are ``psi`` and ``mu_aes``, the aestivation rate and aestivation mortality respectively. 
 * @see AestivationParams
 */
class Aestivation {
public:
	Aestivation(AestivationParams *params, int sites_size);
	void hide(std::vector<Patch*> &sites);
	void wake(int day, std::vector<Patch*> &sites);
	bool is_hide_time(int day);
	bool is_wake_time(int day);

private:
	double psi; /**< Aestivation rate. */ 
	double mu_aes; /**< Aestivation mortality. */ 
	int t_hide1; /**< Start day of aestivation-hiding period (exclusive). */ 
	int t_hide2; /**< End day of aestivation-hiding period (inclusive). */ 
	int t_wake1; /**< Start day of aestivation-waking period (exclusive). */ 
	int t_wake2; /**< End day of aestivation-waking period (inclusive). */ 

	std::vector<std::array<std::array<long long int, constants::num_gen>, constants::num_gen>> aes_F; /**< Number of adult mated female mosquitoes that have gone into aestivation from each patch. @details They follow the same genotypic structure as the mated females in Patch.*/
};

#endif //GENERALMETAPOP_AESTIVATION_H