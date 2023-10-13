#include <filesystem>
#include "GeneralMetapop.h"

// random number seed
std::random_device rd;
//std::mt19937 twister(rd());
std::mt19937 twister(1);

// global structs
Pars pa; 
Initials in;
Times ti; 
Totals to; 
std::vector<Patch> sites; 

int main()
{	
	// input parameters
	// progression parameters
	int num_runs;
	int max_t; 

	// area parameters
	int num_pat;  
	double side;
	
	// model parameters
	double mu_j;
	double mu_a;
	double beta;
	double theta;
	double alpha0;
	double mean_dev;
	int min_dev;

	// gene drive inheritance parameters
	double gamma;
	double xi;
	double e;

	// gene drive release parameters
	int driver_start;
	int num_driver_M;
	int num_driver_sites; 

	// dispersal parameters 
	double disp_rate; 
	double max_disp; 

	// aestivation parameters
	double psi;
	double mu_aes;
	int t_hide1;
	int t_hide2;
	int t_wake1; 
	int t_wake2;

	// data-recording parameters
	int rec_start; 
	int rec_end;
	int rec_interval_global;
	int rec_interval_local;
	int rec_sites_freq; 

	int set_label; 
	int run_label;

	bool invalid_input = true;
	std::cout << "Please enter the input parameters for the model:" << std::endl;
	while (invalid_input){
		try {
			std::cin >> num_runs; 
			std::cin >> max_t;
			std::cin >> num_pat; 
			std::cin >> side; 
			std::cin >> mu_j;
			std::cin >> mu_a; 
			std::cin >> beta; 
			std::cin >> theta; 
			std::cin >> alpha0; 
			std::cin >> mean_dev; 
			std::cin >> min_dev;
			std::cin >> gamma; 
			std::cin >> xi;
			std::cin >> e;
			std::cin >> driver_start;
			std::cin >> num_driver_M; 
			std::cin >> num_driver_sites;
			std::cin >> disp_rate;
			std::cin >> max_disp;
			std::cin >> psi;
			std::cin >> mu_aes;
			std::cin >> t_hide1;
			std::cin >> t_hide2;
			std::cin >> t_wake1; 
			std::cin >> t_wake2;
			std::cin >> rec_start;
			std::cin >> rec_end;
			std::cin >> rec_interval_global;
			std::cin >> rec_interval_local;
			std::cin >> rec_sites_freq;
			std::cin >> set_label;
			std::cin >> run_label;

			// input validation
			if (num_runs <= 0) throw OutOfBoundsException("num_runs");
			if (max_t <= 0) throw OutOfBoundsException("max_t");
			if (num_pat <= 0) throw OutOfBoundsException("num_pat");
			if (side <= 0) throw OutOfBoundsException("side");
			if (mu_j < 0 || mu_j >= 1) throw OutOfBoundsException("mu_j");
			if (mu_a <= 0 || mu_a >= 1) throw OutOfBoundsException("mu_a");
			if (beta <= 0) throw OutOfBoundsException("beta");
			if (theta <= 0) throw OutOfBoundsException("theta");
			if (alpha0 <= 0) throw OutOfBoundsException("alpha0");
			if (mean_dev <= 0) throw OutOfBoundsException("mean_dev");
			if (min_dev <= 0) throw OutOfBoundsException("min_dev");
			if (gamma < 0 || gamma > 1) throw OutOfBoundsException("gamma");
			if (xi < 0 || xi > 1) throw OutOfBoundsException("xi");
			if (e < 0 || e > 1) throw OutOfBoundsException("e");
			if (driver_start < 1) throw OutOfBoundsException("driver_start");
			if (num_driver_M < 0) throw OutOfBoundsException("num_driver_M");
			if (num_driver_sites < 0) throw OutOfBoundsException("num_driver_sites");
			if (disp_rate < 0 || disp_rate > 1) throw OutOfBoundsException("disp_rate");
			if (max_disp <= 0 || max_disp > side) throw OutOfBoundsException("max_disp");
			if (psi < 0 || psi > 1) throw OutOfBoundsException("psi");
			if (mu_aes < 0 || mu_aes > 1) throw OutOfBoundsException("mu_aes");
			if (psi > 0) {
				if (t_hide1 < 1 || t_hide1 > 365) throw OutOfBoundsException("t_hide1");
				if (t_hide2 < 1 || t_hide2 > 365) throw OutOfBoundsException("t_hide2");
				if (t_wake1 < 1 || t_wake1 > 365) throw OutOfBoundsException("t_wake1");
				if (t_wake2 < 1 || t_wake2 > 365) throw OutOfBoundsException("t_wake2");
			}
			if (rec_start < 0) throw OutOfBoundsException("rec_start");
			if (rec_end < 0) throw OutOfBoundsException("rec_end");
			if (rec_interval_global < 1) throw OutOfBoundsException("rec_interval_global");
			if (rec_interval_local < 1) throw OutOfBoundsException("rec_interval_local");
			if (rec_sites_freq < 1) throw OutOfBoundsException("rec_sites_freq");
			if (set_label < 0) throw OutOfBoundsException("set_label");
			if (run_label < 0) throw OutOfBoundsException("run_label");

			if (rec_start >= rec_end) throw InvalidIntervalException("rec_start", "rec_end"); // invalid interval exception
			if (psi > 0) {
				if (t_hide1 >= t_hide2) throw InvalidIntervalException("t_hide1", "t_hide2");
				if (t_wake1 >= t_wake2) throw InvalidIntervalException("t_wake1", "t_wake2");
			}

			if (num_driver_sites > num_pat) {
				std::cout << "Warning: num_driver_sites > num_pat. ";
				std::cout << "This simulation will have reduced gene drive release sites num_driver_sites = num_pat." << std::endl;
			}
			if (driver_start > max_t || num_driver_sites == 0 || num_driver_M == 0) {
				std::cout << "Warning: num_driver_sites or num_driver_M = 0, or driver_start > max_t. ";
				std::cout << "This simulation will not include gene drive." << std::endl;
				std::cout << "Inheritance parameters xi, e and gamma will have no effect." << std::endl;
			} 
			if (disp_rate == 0 || max_disp == 0) {
				std::cout << "Warning: disp_rate or max_disp = 0. This simulation will not include dispersal." << std::endl;
			} 
			if (max_disp > side/2) {
				std::cout << "Warning: max_disp > side/2." << std::endl;
			}
			if (t_hide1 > max_t || t_hide2 > max_t || t_wake1 > max_t || t_wake2 > max_t) {
				std::cout << "Warning: the aestivation interval times are larger than max_t. ";
				std::cout << "This simulation will only run partly through the aestivation period." << std::endl;
			}
			if (psi == 0) {
				std::cout << "Warning: psi = 0. This simulation will not include aestivation." << std::endl;
			} 

			if (rec_start > max_t) {
				std::cout << "Warning: rec_start > max_t. This simulation will not include local recording." << std::endl;
			}
			if (max_t - rec_interval_local - rec_start < 0) {
				std::cout << "Warning: the interval between rec_start and max_t is larger than rec_interval_local. ";
				std::cout << "This simulation will only record local data for day 0." << std::endl;
			}

			invalid_input = false;
		}
		catch (Exception &exception) {
			exception.message();
		}
	}

	// create data structures from validated parameters
	ProgressionParams prog{num_runs, max_t};
	AreaParams area{num_pat, side};
	LifeParams life{mu_j, mu_a, beta, theta, alpha0, mean_dev, min_dev};
	InheritanceParams inher{gamma, xi, e};
	ReleaseParams rel{driver_start, num_driver_M, num_driver_sites};
	DispersalParams disp{disp_rate, max_disp};
	AestivationParams aes{psi, mu_aes, t_hide1, t_hide2, t_wake1, t_wake2};
	InitialPopsParams initial;
	RecordParams rec{rec_start, rec_end, rec_interval_global, rec_interval_local, rec_sites_freq, set_label, run_label};

	// temporary initialisation of old param struct variables needed
	pa.num_pat = area.num_pat;
	pa.side = area.side;
	pa.mu_j = life.mu_j;
	pa.mu_a = life.mu_a;
	pa.beta = life.beta;
	pa.theta = life.theta;
	pa.alpha0 = life.alpha0;
	pa.mean_dev = life.mean_dev;
	pa.min_dev = life.min_dev;
	in.driver_start = rel.driver_start;
	in.num_driver_M = rel.num_driver_M;
	in.num_driver_sites = rel.num_driver_sites;
	pa.disp_rate = disp.disp_rate;
	pa.max_disp = disp.max_disp;
	pa.psi = aes.psi;
	pa.mu_aes = aes.mu_aes;
	pa.t_hide1 = aes.t_hide1;
	pa.t_hide2 = aes.t_hide2;
	pa.t_wake1 = aes.t_wake1;
	pa.t_wake2 = aes.t_wake2;
	in.initial_WJ.fill(initial.initial_WJ);
	in.initial_WM = initial.initial_WM;
	in.initial_WV = initial.initial_WV;
	in.initial_WF = initial.initial_WF;

	auto start = std::chrono::steady_clock::now();

	// run simulation
	SimController simulation(prog, area, life, inher, rel, disp, aes, initial, rec);
	simulation.run_sim();

	auto finish = std::chrono::steady_clock::now();
	double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
	std::cout << "Program run time: " << std::endl;
	printf("%.10f\n", elapsed_seconds);
	
	return 0;
}

// Resets the sites and populations and sets all the initial site parameters
void initiate(int pats, double side) 
{
	for (int i=0; i < num_gen; ++i) {
		to.J[i] = 0;
		to.M[i] = 0;
		to.V[i] = 0;
		to.F[i] = 0;
	}
	to.tot_J = 0;
	to.tot_M = 0;
	to.tot_V = 0;
	to.tot_F = 0;
	sites.clear();

	for (int ii=0; ii < pats; ++ii) {   
		Patch pp;

		// set patch parameters
		double x = random_real() * side;
		double y = random_real() * side;
		pp.coords = {x, y};

		// clear all other patch parameter values
		pp.connec_indices.clear();
		pp.connec_weights.clear();

		for (int i=0; i < num_gen; ++i) {
			for (int a=0; a < max_dev + 1; ++a) {
				pp.J[i][a] = 0; 
			}
			pp.M[i] = 0;
			pp.V[i] = 0;
			for (int j=0; j < num_gen; ++j) {
				pp.F[i][j] = 0;
				pp.aes_F[i][j] = 0;
			}
		}
		pp.comp = 0;
		pp.mate_rate = 0;
		pp.tot_J = 0;
		pp.tot_M = 0;

		sites.push_back(pp);
	}
	
	populate_sites();
	set_connec(pa.side, pa.max_disp);
	set_dev_duration_probs(pa.min_dev, max_dev);
//	CheckCounts(0,'i');
}

// Populates the local site with a (wild) mosquito population of different types (age and sex), according to the initial values provided
// Sets the competition and mating rate entomological model parameters at the site
void populate_sites() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int a=0; a < max_dev + 1; ++a) {
			sites[pat].J[0][a] += in.initial_WJ[a];
			to.J[0] += in.initial_WJ[a];
			to.tot_J += in.initial_WJ[a];
			sites[pat].tot_J += in.initial_WJ[a];
		}
			
		sites[pat].M[0] = in.initial_WM;
		to.M[0] += in.initial_WM;
		to.tot_M += in.initial_WM;
		sites[pat].tot_M += in.initial_WM;

		sites[pat].V[0] = in.initial_WV;
		to.V[0] += in.initial_WV;
		to.tot_V += in.initial_WV;

		sites[pat].F[0][0] = in.initial_WF;
		to.F[0] += in.initial_WF;
		to.tot_F += in.initial_WF;
	}

	update_comp(pa.mu_j, pa.alpha0, pa.mean_dev);
	update_mate(pa.beta);
}

// Computes the inter-patch connectivities
void set_connec(double side, double max_disp) 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat].connec_indices.clear();
		sites[pat].connec_weights.clear();
		for (int new_pat=0; new_pat < sites.size(); ++new_pat) {
		// re-activate this if loop to exclude self-dispersal. need to check isolated sites catered for
		//	if (new_pat != pat) {
				double dd = distance(side, sites[pat].coords, sites[new_pat].coords);
				if (dd < max_disp) {
					sites[pat].connec_indices.push_back(new_pat); 
					double ww = max_disp - dd;
					sites[pat].connec_weights.push_back(ww); 
				}
		//	}
		}
	}
	
}

// Sets probabilities of juvenile eclosion for different age groups
void set_dev_duration_probs(int min_time, int max_time) {
	for (int a=0; a < max_time + 1; ++a) {
        if (a >= min_time) {
            pa.dev_duration_probs[a] = 1.0 / (max_time - min_time);
        }
        else {
            pa.dev_duration_probs[a] = 0;
        }
    }
}

// Releases the gene drive mosquitoes into the simulation area
void release_gene_drive(int num_driver_M, int num_driver_sites)
{
	int num_rel_sites = std::min(pa.num_pat, num_driver_sites);
	std::vector<int> rel_patches = select_driver_sites(num_rel_sites);
	put_driver_sites(rel_patches, num_driver_M);
}

// Selects random sites for release of the gene drive
std::vector<int> select_driver_sites(int num_driver_sites) 
{
	std::vector<int> rel_patches; // patches in which to release the gene drive (contains indices to the patches in Site vector)
	while (rel_patches.size() < num_driver_sites) {
		int rel_pat = random_discrete(0, sites.size() - 1);

		// only pick unique sites within the central area to release the gene drive
		auto is_unique = (rel_patches.end() == std::find(rel_patches.begin(), rel_patches.end(), rel_pat));
		if (is_unique) { 
			rel_patches.push_back(rel_pat);
		}
	}

	return rel_patches;
}

// Adds drive heterozygous (WD) male mosquitoes to the selected sites
void put_driver_sites(const std::vector<int>& patches, int num_driver_M) 
{
	for (const auto& pat : patches) {
		if (pat >= 0 && pat < sites.size()) {
			sites[pat].M[1] += num_driver_M;
			sites[pat].tot_M += num_driver_M;

			// updating totals
			to.M[1] += num_driver_M;
			to.tot_M += num_driver_M;
		}
	}

	update_mate(pa.beta);
}

// Runs daily mosquito life-processes, including aging, death, egg laying, eclosion, mating, dispersal and aestivation 
void run_step(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f, int t_hide1, int t_hide2, int t_wake1, int t_wake2, double psi) 
{
	juv_get_older();
	adults_die(pa.mu_a);
	virgins_mate();
	adults_disperse(pa.disp_rate);
	lay_eggs(pa.theta, pa.dev_duration_probs, f);
	juv_eclose();
	if (day%365 > t_hide1 && day%365 <= t_hide2 && psi > 0.00001) hide(pa.psi, pa.mu_aes);
	if (day%365 > t_wake1 && day%365 <= t_wake2 && psi > 0.00001) wake(day, pa.t_wake2);
	update_comp(pa.mu_j, pa.alpha0, pa.mean_dev);
	update_mate(pa.beta);
}

// Ages the juvenile population in different age groups by a day
void juv_get_older() 
{
	// reset totals
	to.tot_J = 0;
	for (int i=0; i < num_gen; ++i) {
		to.J[i] = 0;
	}
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat].tot_J = 0;
	}

	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			for (int a=0; a < max_dev; ++a) {
				// number of juveniles that survive aging by a day are placed into the new older age group	
				sites[pat].J[i][a] = random_binomial(sites[pat].J[i][a+1], sites[pat].comp);
				sites[pat].tot_J += sites[pat].J[i][a];
				to.J[i] += sites[pat].J[i][a];
			}
		}
		// youngest ones have all aged by one day so none left in this age group
		for (int i=0; i < num_gen; ++i) {
			sites[pat].J[i][max_dev] = 0;
			sites[pat].tot_J += sites[pat].J[i][max_dev];
			to.J[i] += sites[pat].J[i][max_dev];
		} 
		
		to.tot_J += sites[pat].tot_J;
	}
}

// Selects the number of adults that die in the given day and updates population numbers
void adults_die(double mu_a) 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			long long int m = random_binomial(sites[pat].M[i], mu_a); // number of males that die
			sites[pat].M[i] -= m;
			sites[pat].tot_M -= m;
			to.M[i] -= m;
			to.tot_M -= m;	

			long long int v = random_binomial(sites[pat].V[i], mu_a);
			sites[pat].V[i] -= v;
			to.V[i] -= v;
			to.tot_V -= v;	

			for (int j=0; j < num_gen; ++j) {
				long long int f = random_binomial(sites[pat].F[i][j], mu_a);
				sites[pat].F[i][j] -= f;
				to.F[i] -= f;
				to.tot_F -= f;	
			}
		}
	}
}

// Selects the number of virgins that mate in the given day with a male of genotype j, and tranforms them into mated females carrying male genotype j
void virgins_mate() 
{
	std::array<long long int, num_gen> v;
	std::vector<long long int> v_c;
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			v[i] = random_binomial(sites[pat].V[i], sites[pat].mate_rate); // how many V will mate
			if (v[i] > 0) {
				v_c = random_multinomial(v[i], sites[pat].M); // how many V with given genotype will carry each of the male genotypes
				for (int j=0; j < num_gen; j++) {
					sites[pat].F[i][j] += v_c[j];
				}

				sites[pat].V[i] -= v[i];
				to.F[i] += v[i];
				to.V[i] -= v[i];
				to.tot_V -= v[i];
				to.tot_F += v[i];
			}
		}
	}
}

// Selects and updates the number of adults that disperse from and to each patch, depending on the patch connectivities
void adults_disperse(double disp_rate) 
{
	// reset values
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			sites[pat].move_M[i] = 0;

			for (int j=0; j < num_gen; ++j) {
				sites[pat].move_F[i][j] = 0;
			}
		}
	}

	if (sites.size() > 1) {
		// number of adults dispersing from each patch
		for (int pat=0; pat < sites.size(); ++pat) {
			for (int i=0; i < num_gen; ++i) {
				// if (sites[pat].connec_indices > 0) // include this if de-activate self-dispersal
				sites[pat].move_M[i] = random_binomial(sites[pat].M[i], disp_rate); // how many males will disperse from patch
				sites[pat].M[i] -= sites[pat].move_M[i];
				sites[pat].tot_M -= sites[pat].move_M[i];

				for (int j=0; j < num_gen; ++j) {
					sites[pat].move_F[i][j] = random_binomial(sites[pat].F[i][j], disp_rate);
					sites[pat].F[i][j] -= sites[pat].move_F[i][j]; 
				}
			}
		}

		// number of adults dispersing to each patch
		std::vector<long long int> m_disp;
		std::vector<long long int> f_disp;
		for (int pat=0; pat < sites.size(); ++pat) {
			for (int i=0; i < num_gen; ++i) {
				// how many males (for each genotype) will disperse to each of the connected patches for the given patch
				if (sites[pat].move_M[i] > 0) {
					m_disp = random_multinomial(sites[pat].move_M[i], sites[pat].connec_weights);
					for (int new_pat=0; new_pat < m_disp.size(); ++new_pat) {
						sites[sites[pat].connec_indices[new_pat]].M[i] += m_disp[new_pat];
					}
				}

				for (int j=0; j < num_gen; ++j) {
					if (sites[pat].move_F[i][j] > 0) {
						f_disp = random_multinomial(sites[pat].move_F[i][j], sites[pat].connec_weights);
						for (int new_pat=0; new_pat < f_disp.size(); ++new_pat) {
							sites[sites[pat].connec_indices[new_pat]].F[i][j] += f_disp[new_pat];
						}
					}
				}
			}
		}
	}
}

// Calculates the number of eggs laid on the given day and updates the number of juveniles, depending on egg survival rates
void lay_eggs(double theta, const std::array<double, max_dev+1>& dev_duration_probs, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f) 
{
	std::vector<long long int> j_new;
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			for (int j=0; j < num_gen; ++j) {
				for (int k=0; k < num_gen; ++k) {
					double num = theta * sites[pat].F[i][j] * f[i][j][k]; // expected number of eggs laid with k genotype
					long long int eggs = random_poisson(num); // actual number of eggs laid sampled from random distribution

					j_new = random_multinomial(eggs, dev_duration_probs); // number of eggs that start in each different age class (according to different juvenile development times)
					for (int t=0; t < max_dev + 1; ++t) { // juveniles created with assigned remaining time to develop
						sites[pat].J[k][t] += j_new[t];
					}
					
					to.J[k] += eggs;
					to.tot_J += eggs;
				}
			}
		}
	}
}

// Transforms juveniles into adults, depending on eclosion survival rate
void juv_eclose() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			long long int surv = random_binomial(sites[pat].J[i][0], sites[pat].comp); // number of juveniles that survive eclosion
			if (surv > 0) {		
				long long int surv_m = random_binomial(surv, 0.5); // roughly half of the juveniles become male and half female following a distribution
				sites[pat].tot_M += surv_m;
				sites[pat].M[i] += surv_m; 
				to.M[i] += surv_m;
				to.tot_M += surv_m;
				sites[pat].V[i] += surv - surv_m;
				to.V[i] += surv - surv_m;
				to.tot_V += surv - surv_m;
			}
		}
	}
}

// Calculates the number of mated females going into aestivation on the given day and updates the population numbers, depending on the survival rate of going into aestivation
void hide(double psi, double mu_aes) 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			for (int j=0; j < num_gen; ++j) {
				long long int f = random_binomial(sites[pat].F[i][j], psi); // number of females that attempt to go into aestivation
				sites[pat].F[i][j] -= f;
				to.F[i] -= f;
				to.tot_F -= f;
				sites[pat].aes_F[i][j] += random_binomial(f, 1 - mu_aes);	// number that survive going into aestivation
			}
		}
	}
}

// Calculates the number of mated females coming out of aestivation on the given day and updates the population numbers
void wake(int day, int t_wake2) 
{
	double prob = 1.0 / (1.0 + t_wake2 - (day%365)); // probability of a female waking on a given day
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			for(int j=0; j < num_gen; ++j) {
				long long int f = random_binomial(sites[pat].aes_F[i][j], prob); // number of females that wake up from aestivation on the given day
				sites[pat].F[i][j] += f;
				to.F[i] += f;
				to.tot_F += f;
				sites[pat].aes_F[i][j] -= f;	
			}
		}
	}
}

// Updates the juvenile survival probability on the given day in each site
void update_comp(double mu_j, double alpha0, double mean_dev) 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat].comp = (1 - mu_j) * std::pow(alpha0 / (alpha0 + sites[pat].tot_J), 1.0 / mean_dev);
	}
}

// Updates the mating rate parameter in each site
void update_mate(double beta) 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat].mate_rate = sites[pat].tot_M / (beta + sites[pat].tot_M);
	}
}

// Returns the periodic distance between two points in the simulation area with boundaries x = side, y = side
double distance(double side, std::array<double, 2> point1, std::array<double, 2> point2) 
{
	double x_dist = 0;
	double y_dist = 0;
	double x1 = point1[0];
	double y1 = point1[1];
	double x2 = point2[0];
	double y2 = point2[1];

	if (std::abs(x1 - x2) > side - std::abs(x1 - x2)) {
		x_dist = side - std::abs(x1 - x2);
	} 
	else if (std::abs(x1 - x2) <= side - std::abs(x1 - x2)) {
		x_dist = std::abs(x1 - x2);
	}

	if (std::abs(y1 - y2) > side - std::abs(y1 - y2)) {
		y_dist = side - std::abs(y1 - y2);
	}
	else if (std::abs(y1 - y2) <= side - std::abs(y1 - y2)) {
		y_dist = std::abs(y1 - y2);
	}

	return std::sqrt((x_dist * x_dist) + (y_dist * y_dist));
}

// Returns a random floating-point number from a uniform real distribution of 0.0 to 1.0
double random_real() 
{
	std::uniform_real_distribution<> dist(0.0, 1.0);
	return dist(twister);
}

// Returns a random integer number from a uniform discrete distribution of a to b
int random_discrete(int a, int b) 
{
	std::uniform_int_distribution<> dist(a, b);
	return dist(twister);
}

// Returns a random draw (non-negative integer) from the Poisson distribution with mean lambda (using normal distribution approximation when lambda > 30)
long long int random_poisson(double lambda) 
{
	long long int result;
	if (lambda < 1e-5) { 
		result = 0;
	}
	else if (lambda > 30) {
		// use normal approximation	
		std::normal_distribution<> dist(lambda, std::sqrt(lambda)); // distribution(mean, standard deviation)
		int x = std::round(dist(twister));
		result = std::max(0, x);
	}
	else {
		// sample poisson directly
		std::poisson_distribution<> dist(lambda); // distribution(mean)
		result = dist(twister);
	}

	return result;
}

// Returns a random draw (non-negative integer) from the Binomial distribution B(N,p)
// Uses Normal and Poisson distribution approximations for large N
long long int random_binomial(long long int n, double p) 
{
	long long int result;
	if (n*p > 10 && n*(1 - p) > 10) {
		// use normal approximation
		std::normal_distribution<> dist(n*p, std::sqrt(n*p*(1 - p))); // distribution(mean, standard deviation)
		long long int x = std::round(dist(twister));
		if (x<0) x=0;
		if (x>n) x=n;
		result = x;
	}
	else if ((n > 20 && p < 0.05) || (n > 100 && n*p < 10)) {
		// use Poisson approximation
		result = random_poisson(n*p);
	}
	else if ((n > 20 && p > 0.95) || (n > 100 && n*(1-p) < 10)) {
		// use Poisson approximation
		result = n - random_poisson(n*(1 - p));
	}
	else {
		// use binomial distribution directly
		std::binomial_distribution<> dist(n, p);
		result = dist(twister);
	}

	return result;
}

// Returns a vector of outcomes from a random draw of the Multinomial distribution with N trials where each trial has a vector of probabilities <probs>
std::vector<long long int> random_multinomial(long long int n, const std::vector<double>& probs) 
{
	int num_outcomes = probs.size();
	double sum_p = 0.0;
	for (int i = 0; i < num_outcomes; ++i) {
		sum_p += probs[i];
	}

	long long int n_used = n;
	std::vector<long long int> result(num_outcomes, 0);
	for (int i=0; i < num_outcomes; ++i) {
		if (n_used > 0) {
			result[i] = random_binomial(n_used, probs[i] / sum_p);
			sum_p -= probs[i];
			n_used -= result[i];
		}
		else {
			result[i] = 0;
		}
	}

	return result;
}

// Returns a vector of outcomes from a random draw of the Multinomial distribution with N trials where each trial has a vector of probabilities <probs>
std::vector<long long int> random_multinomial(long long int n, const std::array<long long int, num_gen>& probs) 
{
	int num_outcomes = probs.size();
	double sum_p = 0.0;
	for (int i = 0; i < num_outcomes; ++i) {
		sum_p += probs[i];
	}

	long long int n_used = n;
	std::vector<long long int> result(num_outcomes, 0);
	for (int i=0; i < num_outcomes; ++i) {
		if (n_used > 0) {
			result[i] = random_binomial(n_used, probs[i] / sum_p);
			sum_p -= probs[i];
			n_used -= result[i];
		}
		else {
			result[i] = 0;
		}
	}

	return result;
}

// Returns a vector of outcomes from a random draw of the Multinomial distribution with N trials where each trial has a vector of probabilities <probs>
std::vector<long long int> random_multinomial(long long int n, const std::array<double, max_dev+1>& probs) 
{
	int num_outcomes = probs.size();
	double sum_p = 0.0;
	for (int i = 0; i < num_outcomes; ++i) {
		sum_p += probs[i];
	}

	long long int n_used = n;
	std::vector<long long int> result(num_outcomes, 0);
	for (int i=0; i < num_outcomes; ++i) {
		if (n_used > 0) {
			result[i] = random_binomial(n_used, probs[i] / sum_p);
			sum_p -= probs[i];
			n_used -= result[i];
		}
		else {
			result[i] = 0;
		}
	}

	return result;
}

// For debugging, manually calculates total populations counts for F, M, V, J mosquito types and compares with total parameters
// void check_counts(int day, char ref) 
// {
// 	long long int tot_F = 0;
// 	long long int tot_M = 0;
// 	long long int tot_J = 0;
// 	long long int tot_V = 0;
// 	long long int tot_F_B = 0;
// 	long long int tot_M_B = 0;
// 	long long int tot_J_B = 0;
// 	long long int tot_V_B = 0;
	
// 	for (int pat=0; pat < sites.size(); ++pat) {
// 		for (int i=0; i < num_gen; ++i){
// 			for (int j=0; j < num_gen; ++j) {
// 				if (sites[pat].F[i][j] < 0) {
// 					std::cout << day << "\t" << ref << " F neg  " << i << "\t" << j << "\t" << sites[pat].F[i][j] << std::endl;
// 					std::exit(1);
// 				}
// 			}
// 		}

// 		for (int i=0; i < num_gen; ++i) {
// 			tot_F += std::accumulate(sites[pat].F[i].begin(), sites[pat].F[i].end(), 0);
// 		}

// 		tot_M += std::accumulate(sites[pat].M.begin(), sites[pat].M.end(), 0);
// 		tot_V += std::accumulate(sites[pat].V.begin(), sites[pat].V.end(), 0);
// 		for (int i=0; i < num_gen; ++i) {
// 			for (int a=0; a < max_dev + 1; ++a) {
// 				tot_J += sites[pat].J[i][a];
// 			}
// 		}
// 	}

// 	tot_F_B += std::accumulate(to.F.begin(), to.F.end(), 0);
// 	tot_M_B += std::accumulate(to.M.begin(), to.M.end(), 0);
// 	tot_V_B += std::accumulate(to.V.begin(), to.V.end(), 0);
// 	tot_J_B += std::accumulate(to.J.begin(), to.J.end(), 0);

// 	if (tot_F != to.tot_F) {
// 		std::cout << day << "\t" << ref << " F count error  " << tot_F << "\t" << tot_F_B << "\t" << to.tot_F << std::endl;
// 		std::exit(1);
// 	}
// 	if (tot_M != to.tot_M) {
// 		std::cout << day << "\t" << ref << " M count error  " << tot_M << "\t" << tot_M_B << "\t" << to.tot_M << std::endl;
// 		std::exit(1);
// 	}
// 	if (tot_V != to.tot_V) {
// 		std::cout << day << "\t" << ref << " V count error  " << tot_V << "\t" << tot_V_B << "\t" << to.tot_V << std::endl;
// 		std::exit(1);
// 	}
// 	if (tot_J != to.tot_J) {
// 		std::cout << day << "\t" << ref << " J count error  " << tot_J << "\t" << tot_J_B << "\t" << to.tot_J << std::endl;
// 		std::exit(1);
// 	}
// 	if (tot_F_B != to.tot_F) {
// 		std::cout << day << "\t" << ref << " F count errorB  " << tot_F_B << "\t" << to.tot_F << std::endl;
// 		std::exit(1);
// 	}
// 	if (tot_M_B != to.tot_M) {
// 		std::cout << day << "\t" << ref << " M count errorB  " << tot_M_B << "\t" << to.tot_M << std::endl;
// 		std::exit(1);
// 	}
// 	if (tot_V_B != to.tot_V) {
// 		std::cout << day << "\t" << ref << " V count errorB  " << tot_V_B << "\t" << to.tot_V << std::endl;
// 		std::exit(1);
// 	}
// 	if (tot_J_B != to.tot_J) {
// 		std::cout << day << "\t" << ref << " J count errorB  " << tot_J_B << "\t" << to.tot_J << std::endl;
// 		std::exit(1);
// 	}
// }

// New classes

SimController::SimController(ProgressionParams &prog, AreaParams &area, LifeParams &life, InheritanceParams &inher,
 ReleaseParams &rel, DispersalParams &disp, AestivationParams &aes, InitialPopsParams &initial, RecordParams &rec)
{ // also initiate f, maybe to mendelian values, or all zeros???
	my_num_runs = prog.num_runs;
	my_max_t = prog.max_t;
	area_params = &area;
	life_params = &life;
	inher_params = &inher;
	rel_params = &rel;
	disp_params = &disp;
	aes_params = &aes;
	initial_params = &initial;
	rec_params = &rec;
}

void SimController::run_sim()
{
	initiate_sim();
	my_run_reps(my_num_runs);
}

void SimController::initiate_sim()
{
	my_set_inheritance();
}

// Sets the values of the f_{ijk} fraction for the gene drive considering r2 resistant alleles
// f_{ijk} denotes the fraction of genotype k offspring from mother with genotype i mated to father with genotype j
void SimController::my_set_inheritance()
{
	double gamma = inher_params->gamma;
	double xi = inher_params->xi;
	double e = inher_params->e;

	// fraction of genotypes with index 0: ww, 1: wd, 2: dd, 3: wr, 4: rr, 5: dr
	std::array<double, 6> f_ww_ww = {1, 0, 0, 0, 0, 0};
	std::array<double, 6> f_ww_wd = {(1 - e - gamma) * 0.5, (1 + e) * 0.5, 0, gamma * 0.5, 0, 0};
	std::array<double, 6> f_ww_dd = {0, 1, 0, 0, 0, 0};
	std::array<double, 6> f_ww_wr = {0.5, 0, 0, 0.5, 0, 0};
	std::array<double, 6> f_ww_rr = {0, 0, 0, 1, 0, 0};
	std::array<double, 6> f_ww_dr = {0, 0.5, 0, 0.5, 0, 0};

	std::array<double, 6> f_wd_ww = {(1 - xi)*(1 - e - gamma)*0.5, (1 - xi)*(1 + e)*0.5, 0, (1 - xi)*gamma*0.5, 0, 0};
	std::array<double, 6> f_wd_wd = {(1 - xi)*(1 - e - gamma)*(1 - e - gamma)* 0.25, (1 - xi)*(1 - e - gamma)*(1 + e)*0.5, (1 - xi)*(1 + e)*(1 + e)*0.25, (1 - xi)*(1 - e - gamma)*gamma*0.5, (1 - xi)*gamma*gamma*0.25, (1 - xi)*(1 + e)*gamma*0.5};
	std::array<double, 6> f_wd_dd = {0, (1 - xi)*(1 - e - gamma)*0.5, (1 - xi)*(1 + e)*0.5, 0, 0, (1-xi)*gamma*0.5};
	std::array<double, 6> f_wd_wr = {(1 - xi)*(1 - e - gamma)*0.25, (1 - xi)*(1 + e)*0.25, 0, (1 - xi)*((1 - e - gamma)*0.25 + (gamma * 0.25)), (1 - xi)*gamma*0.25, (1 - xi)*(1 + e)*0.25};
	std::array<double, 6> f_wd_rr = {0, 0, 0, (1 - xi)*(1 - e - gamma)*0.5, (1 - xi)*gamma*0.5, (1 - xi)*(1 + e)*0.5};
	std::array<double, 6> f_wd_dr = {0, (1 - xi)*(1 - e - gamma)*0.25, (1 - xi)*(1 + e)*0.25, (1 - xi)*(1 - e - gamma)*0.25, (1 - xi)*gamma*0.25, (1 - xi)*((1 + e)*0.25 + gamma*0.25)};
	
	std::array<double, 6> f_dd_ww = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_wd = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_dd = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_wr = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_rr = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_dr = {0, 0, 0, 0, 0, 0};

	std::array<double, 6> f_wr_ww = {0.5, 0, 0, 0.5, 0, 0};
	std::array<double, 6> f_wr_wd = {(1 - e - gamma)*0.25, (1 + e)*0.25, 0, (gamma * 0.25 + (1 - e - gamma) * 0.25), gamma*0.25, (1 + e)*0.25};
	std::array<double, 6> f_wr_dd = {0, 0.5, 0, 0, 0, 0.5};
	std::array<double, 6> f_wr_wr = {0.25, 0, 0, 0.5, 0.25, 0};
	std::array<double, 6> f_wr_rr = {0, 0, 0, 0.5, 0.5, 0};
	std::array<double, 6> f_wr_dr = {0, 0.25, 0, 0.25, 0.25, 0.25};

	std::array<double, 6> f_rr_ww = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_rr_wd = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_rr_dd = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_rr_wr = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_rr_rr = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_rr_dr = {0, 0, 0, 0, 0, 0};

	std::array<double, 6> f_dr_ww = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dr_wd = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dr_dd = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dr_wr = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dr_rr = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dr_dr = {0, 0, 0, 0, 0, 0};

	for (int k=0; k<6; ++k) {
		for (int i=0; i<6; ++i) {
			for (int j=0; j<6; ++j) {
				if (i==0) {
					if (j==0) f[i][j][k] = f_ww_ww[k];
					else if (j==1) f[i][j][k] = f_ww_wd[k];
					else if (j==2) f[i][j][k] = f_ww_dd[k];
					else if (j==3) f[i][j][k] = f_ww_wr[k];
					else if (j==4) f[i][j][k] = f_ww_rr[k];
					else if (j==5) f[i][j][k] = f_ww_dr[k];
				}
				else if (i==1) {
					if (j==0) f[i][j][k] = f_wd_ww[k];
					else if (j==1) f[i][j][k] = f_wd_wd[k];
					else if (j==2) f[i][j][k] = f_wd_dd[k];
					else if (j==3) f[i][j][k] = f_wd_wr[k];
					else if (j==4) f[i][j][k] = f_wd_rr[k];
					else if (j==5) f[i][j][k] = f_wd_dr[k];
				}
				else if (i==2) {
					if (j==0) f[i][j][k] = f_dd_ww[k];
					else if (j==1) f[i][j][k] = f_dd_wd[k];
					else if (j==2) f[i][j][k] = f_dd_dd[k];
					else if (j==3) f[i][j][k] = f_dd_wr[k];
					else if (j==4) f[i][j][k] = f_dd_rr[k];
					else if (j==5) f[i][j][k] = f_dd_dr[k];
				}
				else if (i==3) {
					if (j==0) f[i][j][k] = f_wr_ww[k];
					else if (j==1) f[i][j][k] = f_wr_wd[k];
					else if (j==2) f[i][j][k] = f_wr_dd[k];
					else if (j==3) f[i][j][k] = f_wr_wr[k];
					else if (j==4) f[i][j][k] = f_wr_rr[k];
					else if (j==5) f[i][j][k] = f_wr_dr[k];
				}
				else if (i==4) {
					if (j==0) f[i][j][k] = f_rr_ww[k];
					else if (j==1) f[i][j][k] = f_rr_wd[k];
					else if (j==2) f[i][j][k] = f_rr_dd[k];
					else if (j==3) f[i][j][k] = f_rr_wr[k];
					else if (j==4) f[i][j][k] = f_rr_rr[k];
					else if (j==5) f[i][j][k] = f_rr_dr[k];
				}
				else if (i==5) {
					if (j==0) f[i][j][k] = f_dr_ww[k];
					else if (j==1) f[i][j][k] = f_dr_wd[k];
					else if (j==2) f[i][j][k] = f_dr_dd[k];
					else if (j==3) f[i][j][k] = f_dr_wr[k];
					else if (j==4) f[i][j][k] = f_dr_rr[k];
					else if (j==5) f[i][j][k] = f_dr_dr[k];
				}
			}
		}
	}	
}

// Runs the simulation n times, recording data in output files.
void SimController::my_run_reps(int n) 
{
	int run_label = rec_params->run_label;
	for (int j=1; j<=n; ++j) {
		Record data(*rec_params, run_label);

		initiate(area_params->num_pat, area_params->side); // set-up Model

		data.record_coords();
		
		// previous run_model() - Runs the simulation once for simulated time max_time
		for(int tt=0; tt <= my_max_t; ++tt) { // current day of the simulation 
			// gene drive release
			if (tt == rel_params->driver_start) release_gene_drive(rel_params->num_driver_M, rel_params->num_driver_sites);

			// model step run
			if (tt > 0) {
				run_step(tt, f, aes_params->t_hide1, aes_params->t_hide2, aes_params->t_wake1, aes_params->t_wake2, aes_params->psi);
			}

			// recording
			if (tt % rec_params->rec_interval_global == 0) {
				// outputs and records total number of each type of mosquito
				std::cout << tt << "\t" << to.tot_J << "\t" << to.tot_M << "\t" << to.tot_V << "\t" << to.tot_F << std::endl;
				data.record_global(tt);
			}
			if ((tt == 0) || (tt >= rec_params->rec_start && tt <= rec_params->rec_end && tt % rec_params->rec_interval_local == 0)) {
				data.record_local(tt, sites.size());
			}
		}

		run_label++;
	}

}

// Creates LocalData, Totals and CoordinateList output files
Record::Record(RecordParams &rec_params, int run_label) 
{
	my_rec_start = rec_params.rec_start;
	my_rec_end = rec_params.rec_end;
	my_rec_interval_global = rec_params.rec_interval_global;
	my_rec_interval_local = rec_params.rec_interval_local;
	my_rec_sites_freq = rec_params.rec_sites_freq;
	my_set_label = rec_params.set_label;
	my_run_label = run_label;

	std::filesystem::path output_path = "C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Output files";
	std::filesystem::current_path(output_path);
	
	my_os1 << "LocalData" << my_set_label << "run" << my_run_label << ".txt"; 
	my_local_data.open(my_os1.str());
	my_os2 << "Totals" << my_set_label << "run" << my_run_label << ".txt";
	my_global_data.open(my_os2.str());
	my_os3 << "CoordinateList" << my_set_label << "run" << my_run_label << ".txt";
	my_coord_list.open(my_os3.str());

	my_local_data << "Male populations of each genotype at each site\n";
	my_local_data << "Day" << "\t" << "Site" << "\t" << "WW" << "\t" << "WD" << "\t" << "DD" << "\t" << "WR" << "\t" << "RR" << "\t" << "DR" << std::endl;

	my_global_data << "Total males of each genotype\n";
	my_global_data << "Day" << "\t" << "WW" << "\t" << "WD" << "\t" << "DD" << "\t" << "WR" << "\t" << "RR" << "\t" << "DR" << std::endl;

	my_coord_list << "Coordinate list of the sites\n";
	my_coord_list << "Site" << "\t" << "x" << "\t" << "y" << std::endl;
}

// Records the number of males of each genotype at each site
void Record::record_local(int day, int sites_size) 
{
	for (int pat=0; pat < sites_size; pat += my_rec_sites_freq) {
		my_local_data << day << "\t" << pat+1;
		for (const auto& m_gen : sites[pat].M) {
			my_local_data << "\t" << m_gen;
		}
		my_local_data << std::endl;
	}
}

// Records the x and y coordinates of each site
void Record::record_coords() 
{
	for (int pat=0; pat < sites.size(); pat += my_rec_sites_freq) {
		my_coord_list << pat+1 << "\t" << sites[pat].coords[0] << "\t" << sites[pat].coords[1] << std::endl;
	}
}

// Records the total number of males (over all sites) of each genotype
void Record::record_global(int day) 
{
	my_global_data << day;
	for (const auto& m_gen : to.M) {
		my_global_data << "\t" << m_gen;
	}
	my_global_data << std::endl;
}

Exception::Exception() {}

OutOfBoundsException::OutOfBoundsException(const std::string& param) : par(param){}

void OutOfBoundsException::message() 
{
    std::cout << "The parameter " << par << " is out of bounds. Please enter all the input parameters again." << std::endl;
}

InvalidIntervalException::InvalidIntervalException(const std::string& param1, const std::string& param2)
{
    inter1 = param1;
    inter2 = param2;
}

void InvalidIntervalException::message()
{
    std::cout << "The parameters " << inter1 << ", " << inter2 << " do not constitute a valid interval. Please enter all the input parameters again." << std::endl;
}
