#include <vector>
#include <cmath>
#include "Model.h"
#include "random.h"
#include "constants.h"
#include "Patch.h"
#include "Dispersal.h"
#include "GDRelease.h"
#include "Aestivation.h"

using namespace constants;

Model::Model(AreaParams *area, InitialPopsParams *initial, LifeParams *life, AestivationParams *aes, DispersalParams *disp, 
	ReleaseParams *rel) 
{
	num_pat = area->num_pat;
	side = area->side;
	initial_WJ = initial->initial_WJ;
	initial_WM = initial->initial_WM;
	initial_WV = initial->initial_WV;
	initial_WF = initial->initial_WF;
	mu_j = life->mu_j;
	mu_a = life->mu_a;
	beta = life->beta;
	theta = life->theta;
	alpha0 = life->alpha0;
	mean_dev = life->mean_dev;
	min_dev = life->min_dev;

	dev_duration_probs.fill(0);

	sites.clear();
	for (int ii=0; ii < num_pat; ++ii) {
		Patch* pp = new Patch(side, life);
		sites.push_back(pp);
	
	}
	Aestivation* new_aestivation = new Aestivation(aes, sites.size());
	aestivation = new_aestivation;
	Dispersal* new_dispersal = new Dispersal(disp);
	dispersal = new_dispersal;
	GDRelease* new_gd_release = new GDRelease(rel);
	gd_release = new_gd_release;
}

Model::~Model() 
{
	for (auto pat : sites) {
		delete pat;
	}
	delete aestivation;
	delete dispersal;
	delete gd_release;
}

// Sets up the model architecture 
void Model::initiate()
{
	populate_sites();
	set_dev_duration_probs(min_dev, max_dev);
	dispersal->set_connecs(side, sites); 
}

// Populates all sites with a (wild) mosquito population of different types (age and sex)
void Model::populate_sites() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat]->populate(initial_WJ, initial_WM, initial_WV, initial_WF);
	}
}

// Sets probabilities of juvenile eclosion for different age groups
void Model::set_dev_duration_probs(int min_time, int max_time) 
{
	for (int a=0; a < max_time + 1; ++a) {
        if (a >= min_time) {
            dev_duration_probs[a] = 1.0 / (max_time - min_time);
        }
        else {
            dev_duration_probs[a] = 0;
        }
    }
}

// Updates the juvenile survival probability in all sites
// void Model::update_comp() 
// {
// 	for (int pat=0; pat < sites.size(); ++pat) {
// 		sites[pat]->update_comp();
// 	}
// }

// Updates the mating rate parameter in all sites
// void Model::update_mate() 
// {
// 	for (int pat=0; pat < sites.size(); ++pat) {
// 		sites[pat]->update_mate();
// 	}
// }

// Handles which model event to run depending on the day of the simulation.
void Model::run(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f)
{
	if (gd_release->is_release_time(day)) {
		gd_release->release_gene_drive(sites);
	} 
	if (day > 0) {
		run_step(day, f);
	}
}

// Runs the daily mosquito life-processes for all sites, including dispersal and aestivation.
void Model::run_step(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f) 
{
	juv_get_older();
	adults_die();
	virgins_mate();
	dispersal->adults_disperse(sites);
	lay_eggs(f);
	juv_eclose();
	if (aestivation->is_hide_time(day)) aestivation->hide(sites);
	if (aestivation->is_wake_time(day)) aestivation->wake(day, sites);
}

// Returns the total number of juveniles across all ages and genotypes and across the simulation area
long long int Model::calculate_tot_J()
{
	long long int tot_J = 0;
	for (int pat = 0; pat < sites.size(); ++pat) {
		tot_J += sites[pat]->calculate_tot_J();
	}
	return tot_J;
}

// Returns the total number of males across all genotypes and across the simulation area
long long int Model::calculate_tot_M()
{
	long long int tot_M = 0;
	for (int pat = 0; pat < sites.size(); ++pat) {
		tot_M += sites[pat]->calculate_tot_M();
	}
	return tot_M;
}

// Returns the total number of unmated females (virgins) across all genotypes and across the simulation area
long long int Model::calculate_tot_V()
{
	long long int tot_V = 0;
	for (int pat = 0; pat < sites.size(); ++pat) {
		tot_V += sites[pat]->calculate_tot_V();
	}
	return tot_V;
}

// Returns the total number of mated females across all female and carrying-male genotypes and across the simulation area
long long int Model::calculate_tot_F()
{
	long long int tot_F = 0;
	for (int pat = 0; pat < sites.size(); ++pat) {
		tot_F += sites[pat]->calculate_tot_F();
	}
	return tot_F;
}

// Returns the total number of males of each genotype across the simulation area
std::array<long long int, num_gen> Model::calculate_tot_M_gen() 
{
	std::array<long long int, num_gen> tot_M_gen;
	tot_M_gen.fill(0);
	for (int pat = 0; pat < sites.size(); ++pat) {
		std::array<long long int, num_gen> m_pat = sites[pat]->get_M();
		for (int i = 0; i < num_gen; ++i) {
			tot_M_gen[i] += m_pat[i];
		}
	}
	return tot_M_gen;
}

std::vector<Patch*> Model::get_sites() const
{
	return sites;
}

// Ages the juvenile population in different age groups by a day across the simulation area
void Model::juv_get_older() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat]->juv_get_older(max_dev);
	}
}

// Selects the number of adults that die in the given day and updates population numbers across the simulation area
void Model::adults_die()
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat]->adults_die(mu_a);
	}
}

// Selects the number of virgins that mate in the given day with a male of genotype j, and tranforms them into mated females carrying
// male genotype j. Mating is carried out in each site across the simulation area.
void Model::virgins_mate() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat]->virgins_mate();
	}
}

// Calculates the number of eggs laid on the given day and updates the number of juveniles, depending on egg survival rates.
// Egg-laying is carried out in all sites across the simulation area.
void Model::lay_eggs(const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f)
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat]->lay_eggs(f, theta, dev_duration_probs);
	}
}

// Turns juveniles into adults, depending on eclosion survival rate, across all sites in the simulation area.
void Model::juv_eclose()
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat]->juv_eclose();
	}
}