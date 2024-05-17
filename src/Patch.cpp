#include <cmath>
#include <array>
#include <vector>
#include "Patch.h"
#include "random.h"
#include "constants.h"

// Sets coordinates randomly on the square space of side side.
Patch::Patch(Model* mod, LifeParams* par, double a0, double side) 
{
	model = mod;
	params = par;
	alpha0 = a0;
	
	double x = random_real() * side;
	double y = random_real() * side;
	coords = {x, y};

	for (int i=0; i < constants::num_gen; ++i) {
		for (int a=0; a < constants::max_dev + 1; ++a) {
			J[i][a] = 0; 
		}
		M[i] = 0;
		V[i] = 0;
		for (int j=0; j < constants::num_gen; ++j) {
			F[i][j] = 0;
		}
	}
	comp = 0;
	mate_rate = 0;
}

// Sets chosen coordinates.
Patch::Patch(Model* mod, LifeParams* par, double a0, Point point) 
{
	model = mod;
	params = par;
	alpha0 = a0;
	coords = point;

	// include to be able to compare data to oracle when testing
	double x = random_real();
	double y = random_real();

	for (int i=0; i < constants::num_gen; ++i) {
		for (int a=0; a < constants::max_dev + 1; ++a) {
			J[i][a] = 0; 
		}
		M[i] = 0;
		V[i] = 0;
		for (int j=0; j < constants::num_gen; ++j) {
			F[i][j] = 0;
		}
	}
	comp = 0;
	mate_rate = 0;
}

// Populates the local site with a (wild) mosquito population of different types (age and sex)
void Patch::populate(int initial_WJ, int initial_WM, int initial_WV, int initial_WF) 
{
	for (int a=0; a < constants::max_dev + 1; ++a) {
		J[0][a] += initial_WJ;
	}	
	M[0] = initial_WM;
	V[0] = initial_WV;
	F[0][0] = initial_WF;

	update_comp();
	update_mate();
}

// Returns the coordinates of the site on the simulation area
Point Patch::get_coords() const
{
	return coords;
}

// Returns an array of number of males with each genotype
std::array<long long int, constants::num_gen> Patch::get_M() const
{
	return M;
}

// Returns a 2D array of number of females with each female and carrying mated male genotype
std::array<std::array<long long int, constants::num_gen>, constants::num_gen> Patch::get_F() const
{
	return F;
}

// Returns the total number of juveniles across all ages and genotypes
long long int Patch::calculate_tot_J() 
{
	long long int tot_J = 0;
	for (int i = 0; i < constants::num_gen; ++i) {
		for (int a = 0; a < constants::max_dev+1; ++a) {
			tot_J += J[i][a];
		}
	}
	return tot_J;
}

// Returns the total number of males across all genotypes
long long int Patch::calculate_tot_M() 
{
	long long int tot_M = 0;
	for (const auto& m : M) {
		tot_M += m;
	}
	return tot_M;
}

// Returns the total number of unmated females (virgins) across all genotypes
long long int Patch::calculate_tot_V()
{
	long long int tot_V = 0;
	for (const auto& v : V) {
		tot_V += v;
	}
	return tot_V;
}

// Returns the total number of mated females across all female and carrying-male genotypes
long long int Patch::calculate_tot_F()
{
	long long int tot_F = 0;
	for (int i = 0; i < constants::num_gen; ++i) {
		for (int j = 0; j < constants::num_gen; ++j) {
			tot_F += F[i][j];
		}
	}
	return tot_F;
}

// Removes males from the population as they disperse out.
void Patch::M_disperse_out(const std::array<long long int, constants::num_gen> &m_out) 
{
	for (std::size_t i = 0; i < m_out.size(); ++i) {
		M[i] -= m_out[i];
	}
	update_mate();
}

// Removes females from the population as they disperse out.
void Patch::F_disperse_out(const std::array<std::array<long long int, constants::num_gen>, constants::num_gen> &f_out) 
{
	for (std::size_t i = 0; i < f_out.size(); ++i) {
		for (std::size_t j = 0; j < f_out[0].size(); ++j) {
			F[i][j] -= f_out[i][j];
		}
	}
}

// Introduces new males into the population as they disperse in.
void Patch::M_disperse_in(int gen, long long int m_in) 
{
	M[gen] += m_in;
	update_mate();
}

// Introduces new females into the population as they disperse in.
void Patch::F_disperse_in(int f_gen, int m_gen, long long int f_disp) 
{
	F[f_gen][m_gen] += f_disp;
}

// Updates active female population numbers after they attempt to go into aestivation.
void Patch::F_hide(const std::array<std::array<long long int, constants::num_gen>, constants::num_gen> &f_try)
{
	for (std::size_t i = 0; i < F.size(); ++i) {
		for (std::size_t j = 0; j < F[0].size(); ++j) {
			F[i][j] -= f_try[i][j];
		}
	}
}

// Updates active female population numbers after they wake from aestivation.
void Patch::F_wake(const std::array<std::array<long long int, constants::num_gen>, constants::num_gen> &f_wake)
{
	for (std::size_t i = 0; i < F.size(); ++i) {
		for (std::size_t j = 0; j < F[0].size(); ++j) {
			F[i][j] += f_wake[i][j];
		}
	}
}

// Introduces driver heterozygous males into the population.
void Patch::add_driver_M(int num_driver_M) 
{
	M[1] += num_driver_M;
	update_mate();
}

// Ages the juvenile population in different age groups by a day within the local site
void Patch::juv_get_older() 
{
	for (int i=0; i < constants::num_gen; ++i) {
		for (int a=0; a < constants::max_dev; ++a) {
			// number of juveniles that survive aging by a day are placed into the new older age group	
			J[i][a] = random_binomial(J[i][a+1], comp);
		}
		J[i][constants::max_dev] = 0; // youngest ones have all aged by one day so none left in this age group
	}
}

// Selects the number of adults that die in the given day and updates population numbers within the local site
void Patch::adults_die()
{
	double mu_a = params->mu_a;
	for (int i=0; i < constants::num_gen; ++i) {
		long long int m = random_binomial(M[i], mu_a); // number of males that die
		M[i] -= m;

		long long int v = random_binomial(V[i], mu_a);
		V[i] -= v;	

		for (int j=0; j < constants::num_gen; ++j) {
			long long int f = random_binomial(F[i][j], mu_a);
			F[i][j] -= f;
		}
	}

	update_mate();
}

// Selects the number of virgins that mate in the given day with a male of genotype j, and tranforms them into mated females carrying
// male genotype j. Mating is carried out within the local site.
void Patch::virgins_mate() 
{
	std::array<long long int, constants::num_gen> v;
	std::vector<long long int> v_c;
	for (int i=0; i < constants::num_gen; ++i) {
		v[i] = random_binomial(V[i], mate_rate); // how many V will mate
		if (v[i] > 0) {
			v_c = random_multinomial(v[i], M); // how many V with given genotype will carry each of the male genotypes
			for (int j=0; j < constants::num_gen; j++) {
				F[i][j] += v_c[j];
			}
			V[i] -= v[i];
		}
	}
	
}

// Calculates the number of eggs laid on the given day and updates the number of juveniles within the local site, depending on egg
// survival rates.
void Patch::lay_eggs(const std::array<std::array<std::array <double, constants::num_gen>, constants::num_gen>, constants::num_gen> &inher_fraction,
 const std::array<double, constants::max_dev+1> &dev_duration_probs)
{
	std::vector<long long int> j_new;
	for (int i=0; i < constants::num_gen; ++i) {
		for (int j=0; j < constants::num_gen; ++j) {
			for (int k=0; k < constants::num_gen; ++k) {
				double num = (params->theta) * F[i][j] * inher_fraction[i][j][k]; // expected number of eggs laid with k genotype
				long long int eggs = random_poisson(num); // actual number of eggs laid sampled from random distribution

				j_new = random_multinomial(eggs, dev_duration_probs); // number of eggs that start in each different age class (according to different juvenile development times)
				for (int t=0; t < constants::max_dev + 1; ++t) { // juveniles created with assigned remaining time to develop
					J[k][t] += j_new[t];
				}
			}
		}
	}

	update_comp();
}

// Turns juveniles into adults, depending on eclosion survival rate, within the local site.
void Patch::juv_eclose() 
{
	for (int i=0; i < constants::num_gen; ++i) {
		long long int surv = random_binomial(J[i][0], comp); // number of juveniles that survive eclosion
		J[i][0] = 0; // all the oldest juveniles either successfully eclose or die
		if (surv > 0) {	
			// roughly half of the juveniles become male and half female following a distribution
			long long int surv_m = random_binomial(surv, 0.5); 
			M[i] += surv_m; 
			V[i] += surv - surv_m;
		}
	}
	update_comp();
	update_mate();
}

// Updates the juvenile survival probability in the local site
void Patch::update_comp()
{
	int d = model->get_day();
	double alpha = model->get_alpha(alpha0);
	long long int tot_J = calculate_tot_J();
	comp = (1 - (params->mu_j)) * std::pow(alpha / (alpha + tot_J), 1.0 / (params->mean_dev));
}

// Updates the mating rate parameter in the local site
void Patch::update_mate()
{
	long long int tot_M = calculate_tot_M();
	mate_rate = tot_M / ((params->beta) + tot_M);
}