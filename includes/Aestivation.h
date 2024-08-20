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

	std::vector<std::array<std::array<long long int, constants::num_gen>, constants::num_gen>> aes_F; /**< Number of mated female mosquitoes that have gone into aestivation from each patch. @details They follow the same genotypic structure as the mated females in Patch.*/
};

#endif //GENERALMETAPOP_AESTIVATION_H