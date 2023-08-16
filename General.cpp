#include "headGen.h"

// random number seed
std::random_device rd;
std::mt19937 twister(rd());

// global structs
Pars pa; 
Initials in;
Times ti; 
Totals to; 
std::vector<Patch> sites; 

// output files
std::ostringstream os1, os2, os3;
std::ofstream local_data; // population values of males with each genotype in each patch for each simulated day
std::ofstream global_data; // total population values (in all patches) of males with each genotype for each simulated day
std::ofstream coord_list; // coordinate list (x, y) of the patches

int main()
{	
	// input parameters
	std::cin >> pa.set_label; 
	std::cin >> pa.run_label;
	std::cin >> pa.num_pat; 
	std::cin >> in.rec_sites_freq; 
	std::cin >> ti.rec_interval_global;
	std::cin >> ti.max_t; 
	std::cin >> ti.rec_interval_local; 
	std::cin >> ti.rec_start; 
	std::cin >> ti.rec_end; 
	std::cin >> ti.num_reps; 
	std::cin >> in.num_driver_M;
	std::cin >> in.num_driver_sites; 
	std::cin >> in.driver_start;
	std::cin >> pa.mu_j;
	std::cin >> pa.mu_a;
	std::cin >> pa.disp_rate; 
	std::cin >> pa.gamma;
	std::cin >> pa.beta;
	std::cin >> pa.theta;
	std::cin >> pa.xi;
	std::cin >> pa.e;
	std::cin >> pa.max_disp; 

	std::cin >> pa.psi;
	std::cin >> pa.mu_aes;
	std::cin >> pa.t_hide1;
	std::cin >> pa.t_hide2;
	std::cin >> pa.t_wake1; 
	std::cin >> pa.t_wake2;
	std::cin >> pa.alpha0;

	std::cin >> pa.mean_dev;
	std::cin >> pa.min_dev;

	std::cin >> pa.side;
	std::cin >> pa.central_radius;

	// initial populations
	in.initial_WV = 10000;
	in.initial_WM = 50000;
	in.initial_WF = 40000;

	for (int a=0; a < max_dev + 1; ++a) {
		in.initial_WJ[a] = 10000;
	}

	//auto start = std::chrono::steady_clock::now();
	//auto finish = std::chrono::steady_clock::now();
	//double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
	//std::cout << "Fertility run time: " << elapsed_seconds << std::endl;

	// set inheritance architecture
	set_inheritance();

	// run model NumRuns times
	run_reps(ti.num_reps);
	
	return 0;
}

// Creates the output files and runs the simulation N times
void run_reps(int n) 
{
	create_files(pa.set_label, pa.run_label);

	for (int j=1; j<=n; ++j) {
		initiate();
		record_coords();
		run_model(ti.max_t);
	}

	close_files();
}

// Creates output files
void create_files(int set_label, int run_label) 
{
	os1 << "LocalData" << set_label << "run" << run_label << ".txt"; 
	local_data.open(os1.str().c_str());
	os2 << "Totals" << set_label << "run" << run_label << ".txt";
	global_data.open(os2.str().c_str());
	os3 << "CoordinateList" << set_label << "run" << run_label << ".txt";
	coord_list.open(os3.str().c_str());

	local_data << "Male populations of each genotype at each site\n";
	local_data << "Day" << "\t" << "Site" << "\t" << "WW" << "\t" << "WD" << "\t" << "DD" << "\t" << "WR" << "\t" << "RR" << "\t" << "DR" << std::endl;

	global_data << "Total males of each genotype\n";
	global_data << "Day" << "\t" << "WW" << "\t" << "WD" << "\t" << "DD" << "\t" << "WR" << "\t" << "RR" << "\t" << "DR" << std::endl;

	coord_list << "Coordinate list of the sites\n";
	coord_list << "Site" << "\t" << "x" << "\t" << "y" << std::endl;
}

// Closes output files
void close_files() 
{
	local_data.close();
	global_data.close();
	coord_list.close();
}

// Resets the sites and populations and sets all the initial site parameters
void initiate() 
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
	to.central_sites = 0;
	sites.clear();

	for (int ii=0; ii < pa.num_pat; ++ii) {   
		Patch pp;

		// set patch parameters  
		double x = random_real() * pa.side;
		double y = random_real() * pa.side;
		pp.coords = {x, y};

		if (pp.is_central()) {
			to.central_sites++;
		}

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
	set_connec();
	set_eclosion_probs(pa.min_dev, max_dev);
//	CheckCounts(0,'i');
}

bool Patch::is_central() 
{
	std::array<double, 2> centre = {pa.side / 2.0, pa.side / 2.0};
	bool is_inside = (distance(pa.side, coords, centre) < pa.central_radius);
	return is_inside;
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

	update_comp();
	update_mate();
}

// Computes the inter-patch connectivities
void set_connec() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat].connec_indices.clear();
		sites[pat].connec_weights.clear();
		for (int new_pat=0; new_pat < sites.size(); ++new_pat) {
		// re-activate this if loop to exclude self-dispersal. need to check isolated sites catered for
		//	if (new_pat != pat) {
				double dd = distance(pa.side, sites[pat].coords, sites[new_pat].coords);
				if (dd < pa.max_disp) {
					sites[pat].connec_indices.push_back(new_pat); 
					double ww = pa.max_disp - dd;
					sites[pat].connec_weights.push_back(ww); 
				}
		//	}
		}
	}
}

// set probabilities of juvenile eclosion for different age groups
void set_eclosion_probs(int min_time, int max_time) {
	for (int a=0; a < max_time + 1; ++a) {
        if (a >= min_time) {
            pa.dev_duration_probs[a] = 1.0 / (max_time - min_time);
        }
        else {
            pa.dev_duration_probs[a] = 0;
        }
		std::cout << a << ": " << pa.dev_duration_probs[a] << std::endl;
    }

	// for (int a=0; a < max_time + 1; ++a) {
    //     if (a <= (max_time - min_time)) {
    //         pa.dev_duration_probs[a] = 1.0 / (max_time - min_time);
    //     }
    //     else {
    //         pa.dev_duration_probs[a] = 0;
    //     }
	// 	std::cout << a << ": " << pa.dev_duration_probs[a] << std::endl;
    // }

}

// Runs the simulation once for simulated time max_time
void run_model(int max_time) 
{
	for(int tt=0; tt <= max_time; ++tt) { // current day of the simulation 
		// gene drive release
		int rel_time = in.driver_start;
		if (tt == rel_time) {
			int num_rel_sites = std::min(to.central_sites, in.num_driver_sites);
			std::vector<int> rel_patches = select_driver_sites(num_rel_sites);
			put_driver_sites(rel_patches);
		}

		if (tt > 0) run_step(tt);

		if (tt % ti.rec_interval_global == 0) {
			// outputs and records total number of each type of mosquito
			std::cout << tt << "\t" << to.tot_J << "\t" << to.tot_M << "\t" << to.tot_V << "\t" << to.tot_F << std::endl;
			record_global(tt);
		}

		if (tt > ti.rec_start && tt <= ti.rec_end && tt % ti.rec_interval_local == 0) {
			record_local(tt);
		}
	}
}

// Selects random sites for release of the gene drive
std::vector<int> select_driver_sites(int num_driver_sites) 
{
	std::vector<int> rel_patches; // patches in which to release the gene drive (contains indices to the patches in Site vector)
	while (rel_patches.size() < num_driver_sites) {
		int rel_pat = random_discrete(0, sites.size() - 1);

		// only pick unique sites within the central area to release the gene drive
		auto is_unique = (rel_patches.end() == std::find(rel_patches.begin(), rel_patches.end(), rel_pat));
		if ((sites[rel_pat].is_central()) && is_unique) { 
			rel_patches.push_back(rel_pat);
		}
	}

	return rel_patches;
}

// Adds drive heterozygous (WD) male mosquitoes to the selected sites
void put_driver_sites(const std::vector<int>& patches) 
{
	for (const auto& pat : patches) {
		if (pat >= 0 && pat < sites.size()) {
			sites[pat].M[1] += in.num_driver_M;
			sites[pat].tot_M += in.num_driver_M;

			// updating totals
			to.M[1] += in.num_driver_M;
			to.tot_M += in.num_driver_M;
		}
	}

	update_mate();
}

// Records the x and y coordinates of each site
void record_coords() 
{
	for (int pat=0; pat < sites.size(); pat += in.rec_sites_freq) {
		coord_list << pat+1 << "\t" << sites[pat].coords[0] << "\t" << sites[pat].coords[1] << std::endl;
	}
}

// Records the total number (over all sites) of males of each genotype
void record_global(int day) 
{
	global_data << day;
	for (const auto& M_gen : to.M) {
		global_data << "\t" << M_gen;
	}
	global_data << std::endl;
}

// Records the number of males of each genotype at each site in a file (localinfo file)
void record_local(int day) 
{	
	for (int pat=0; pat < sites.size(); pat += in.rec_sites_freq) {
		local_data << day << "\t" << pat+1;
		for (const auto& m_gen : sites[pat].M) {
			local_data << "\t" << m_gen;
		}
		local_data << std::endl;
	}
}

// Runs daily mosquito life-processes, including aging, death, egg laying, eclosion, mating, dispersal and aestivation 
void run_step(int day) 
{
	juv_get_older();
	adults_die();
	virgins_mate();
	adults_disperse();
	lay_eggs();
	juv_eclose();
	if (day%365 > pa.t_hide1 && day%365 <= pa.t_hide2 && pa.psi > 0.00001) hide();
	if (day%365 > pa.t_wake1 && day%365 <= pa.t_wake2 && pa.psi > 0.00001) wake(day);
	update_comp();
	update_mate();
}

// Ages the juvenile population in different age groups by a day
void juv_get_older() 
{
	std::array<long long int, num_gen> j_tot_gens;
	for (int i=0; i < num_gen; ++i) {
		j_tot_gens[i] = 0; 
	}

	long long int j_tot = 0;
	for (int pat=0; pat < sites.size(); ++pat) {
		long long int j_pat = 0;
		for (int i=0; i < num_gen; ++i) {
			for (int a=0; a < max_dev; ++a) {
				// number of juveniles that survive aging by a day are placed into the new older age group	
				sites[pat].J[i][a] = random_binomial(sites[pat].J[i][a+1], sites[pat].comp);
				j_pat += sites[pat].J[i][a];
				j_tot_gens[i] += sites[pat].J[i][a];
			}
		}
		
		sites[pat].tot_J = j_pat;
		j_tot += j_pat;

		// youngest ones have all aged by one day so none left in this age group
		for (int i=0; i < num_gen; ++i) {
			sites[pat].J[i][max_dev] = 0;
		} 
	}

	// updating totals
	for (int i=0; i < num_gen; ++i) {
		to.J[i] = j_tot_gens[i];
	}
	to.tot_J = j_tot;
}

// Selects the number of adults that die in the given day and updates population numbers
void adults_die() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			long long int m = random_binomial(sites[pat].M[i], pa.mu_a); // number of males that die
			sites[pat].M[i] -= m;
			sites[pat].tot_M -= m;
			to.M[i] -= m;
			to.tot_M -= m;	

			long long int v = random_binomial(sites[pat].V[i], pa.mu_a);
			sites[pat].V[i] -= v;
			to.V[i] -= v;
			to.tot_V -= v;	

			for (int j=0; j < num_gen; ++j) {
				long long int f = random_binomial(sites[pat].F[i][j], pa.mu_a);
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
	std::vector<double> m(num_gen);
	std::vector<long long int> v_c;
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			m[i] = sites[pat].M[i];
		}

		for (int i=0; i < num_gen; ++i) {
			v[i] = random_binomial(sites[pat].V[i], sites[pat].mate_rate); // how many V will mate

			if (v[i] > 0) {
				v_c = random_multinomial(v[i], m); // how many V with given genotype will carry each of the male genotypes
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
void adults_disperse() 
{
	// const int num_pats = sites.size();
	if (sites.size() > 1) {
		// number of adults dispersing from each patch
		// std::array<std::array<long long int, num_gen>, num_pats> m_from;
		// std::array<std::array<std::array<long long int, num_gen>, num_gen>, num_pats> f_from;
		for (int pat=0; pat < sites.size(); ++pat) {
			for (int i=0; i < num_gen; ++i) {
				// if (sites[pat].connec_indices > 0) // include this if de-activate self-dispersal
				sites[pat].move_M[i] = random_binomial(sites[pat].M[i], pa.disp_rate); // how many males will disperse from patch
				sites[pat].M[i] -= sites[pat].move_M[i];
				sites[pat].tot_M -= sites[pat].move_M[i];

				for (int j=0; j < num_gen; ++j) {
					sites[pat].move_F[i][j] = random_binomial(sites[pat].F[i][j], pa.disp_rate);
					sites[pat].F[i][j] -= sites[pat].move_F[i][j]; 
				}
			}
		}

		// number of adults dispersing to each patch
		std::vector<long long int> m_disp;
		std::vector<long long int> f_disp;
		for (int pat=0; pat < sites.size(); ++pat) {
			for (int i=0; i < num_gen; ++i) {
				// how many males will disperse to each of the connected patches for the given patch
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

		// reset values
		for (int pat=0; pat < sites.size(); ++pat) {
			for (int i=0; i < num_gen; ++i) {
				sites[pat].move_M[i] = 0;

				for (int j=0; j < num_gen; ++j) {
					sites[pat].move_F[i][j] = 0;
				}
			}
		}

	}
}

// Calculates the number of eggs laid on the given day and updates the number of juveniles, depending on egg survival rates
void lay_eggs() 
{
	std::vector<double> j_probs(max_dev + 1);
	for (int i=0; i < max_dev + 1; ++i) {
		j_probs[i] = pa.dev_duration_probs[i];
	}

	std::vector<long long int> j_new;
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			for (int j=0; j < num_gen; ++j) {
				for (int k=0; k < num_gen; ++k) {
					double num = pa.theta * sites[pat].F[i][j] * pa.f[i][j][k]; // expected number of eggs laid with k genotype
					long long int eggs = random_poisson(num); // actual number of eggs laid sampled from random distribution

					j_new = random_multinomial(eggs, j_probs); // number of eggs that start in each different age class (according to different juvenile development times)
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
void hide() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			for (int j=0; j < num_gen; ++j) {
				long long int f = random_binomial(sites[pat].F[i][j], pa.psi); // number of females that attempt to go into aestivation
				sites[pat].F[i][j] -= f;
				to.F[i] -= f;
				to.tot_F -= f;
				sites[pat].aes_F[i][j] += random_binomial(f, 1 - pa.mu_aes);	// number that survive going into aestivation
			}
		}
	}
}

// Calculates the number of mated females coming out of aestivation on the given day and updates the population numbers
void wake(int day) 
{
	double prob = 1.0 / (1.0 + pa.t_wake2 - (day%365)); // probability of a female waking on a given day
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
void update_comp() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat].comp = (1 - pa.mu_j) * std::pow(pa.alpha0 / (pa.alpha0 + sites[pat].tot_J), 1.0 / pa.mean_dev);
	}
}

// Updates the mating rate parameter in each site
void update_mate() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat].mate_rate = sites[pat].tot_M / (pa.beta + sites[pat].tot_M);
	}
}

// Sets the values of the f_{ijk} fraction for the gene drive considering r2 resistant alleles
// f_{ijk} denotes the fraction of genotype k offspring from mother with genotype i mated to father with genotype j
void set_inheritance() 
{
	// fraction of genotypes with index 0: ww, 1: wd, 2: dd, 3: wr, 4: rr, 5: dr
	std::array<double, 6> f_ww_ww = {1, 0, 0, 0, 0, 0};
	std::array<double, 6> f_ww_wd = {(1 - pa.e - pa.gamma) * 0.5, (1 + pa.e) * 0.5, 0, pa.gamma * 0.5, 0, 0};
	std::array<double, 6> f_ww_dd = {0, 1, 0, 0, 0, 0};
	std::array<double, 6> f_ww_wr = {0.5, 0, 0, 0.5, 0, 0};
	std::array<double, 6> f_ww_rr = {0, 0, 0, 1, 0, 0};
	std::array<double, 6> f_ww_dr = {0, 0.5, 0, 0.5, 0, 0};

	std::array<double, 6> f_wd_ww = {(1 - pa.xi)*(1 - pa.e - pa.gamma)*0.5, (1 - pa.xi)*(1 + pa.e)*0.5, 0, (1 - pa.xi)*pa.gamma*0.5, 0, 0};
	std::array<double, 6> f_wd_wd = {(1 - pa.xi)*(1 - pa.e - pa.gamma)*(1 - pa.e - pa.gamma)* 0.25, (1 - pa.xi)*(1 - pa.e - pa.gamma)*(1 + pa.e)*0.5, (1 - pa.xi)*(1 + pa.e)*(1 + pa.e)*0.25, (1 - pa.xi)*(1 - pa.e - pa.gamma)*pa.gamma*0.5, (1 - pa.xi)*pa.gamma*pa.gamma*0.25, (1 - pa.xi)*(1 + pa.e)*pa.gamma*0.5};
	std::array<double, 6> f_wd_dd = {0, (1 - pa.xi)*(1 - pa.e - pa.gamma)*0.5, (1 - pa.xi)*(1 + pa.e)*0.5, 0, 0, (1-pa.xi)*pa.gamma*0.5};
	std::array<double, 6> f_wd_wr = {(1 - pa.xi)*(1 - pa.e - pa.gamma)*0.25, (1 - pa.xi)*(1 + pa.e)*0.25, 0, (1 - pa.xi)*((1 - pa.e - pa.gamma)*0.25 + (pa.gamma * 0.25)), (1 - pa.xi)*pa.gamma*0.25, (1 - pa.xi)*(1 + pa.e)*0.25};
	std::array<double, 6> f_wd_rr = {0, 0, 0, (1 - pa.xi)*(1 - pa.e - pa.gamma)*0.5, (1 - pa.xi)*pa.gamma*0.5, (1 - pa.xi)*(1 + pa.e)*0.5};
	std::array<double, 6> f_wd_dr = {0, (1 - pa.xi)*(1 - pa.e - pa.gamma)*0.25, (1 - pa.xi)*(1 + pa.e)*0.25, (1 - pa.xi)*(1 - pa.e - pa.gamma)*0.25, (1 - pa.xi)*pa.gamma*0.25, (1 - pa.xi)*((1 + pa.e)*0.25 + pa.gamma*0.25)};
	
	std::array<double, 6> f_dd_ww = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_wd = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_dd = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_wr = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_rr = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> f_dd_dr = {0, 0, 0, 0, 0, 0};

	std::array<double, 6> f_wr_ww = {0.5, 0, 0, 0.5, 0, 0};
	std::array<double, 6> f_wr_wd = {(1 - pa.e - pa.gamma)*0.25, (1 + pa.e)*0.25, 0, (pa.gamma * 0.25 + (1 - pa.e - pa.gamma) * 0.25), pa.gamma*0.25, (1 + pa.e)*0.25};
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
					if (j==0) pa.f[i][j][k] = f_ww_ww[k];
					else if (j==1) pa.f[i][j][k] = f_ww_wd[k];
					else if (j==2) pa.f[i][j][k] = f_ww_dd[k];
					else if (j==3) pa.f[i][j][k] = f_ww_wr[k];
					else if (j==4) pa.f[i][j][k] = f_ww_rr[k];
					else if (j==5) pa.f[i][j][k] = f_ww_dr[k];
				}
				else if (i==1) {
					if (j==0) pa.f[i][j][k] = f_wd_ww[k];
					else if (j==1) pa.f[i][j][k] = f_wd_wd[k];
					else if (j==2) pa.f[i][j][k] = f_wd_dd[k];
					else if (j==3) pa.f[i][j][k] = f_wd_wr[k];
					else if (j==4) pa.f[i][j][k] = f_wd_rr[k];
					else if (j==5) pa.f[i][j][k] = f_wd_dr[k];
				}
				else if (i==2) {
					if (j==0) pa.f[i][j][k] = f_dd_ww[k];
					else if (j==1) pa.f[i][j][k] = f_dd_wd[k];
					else if (j==2) pa.f[i][j][k] = f_dd_dd[k];
					else if (j==3) pa.f[i][j][k] = f_dd_wr[k];
					else if (j==4) pa.f[i][j][k] = f_dd_rr[k];
					else if (j==5) pa.f[i][j][k] = f_dd_dr[k];
				}
				else if (i==3) {
					if (j==0) pa.f[i][j][k] = f_wr_ww[k];
					else if (j==1) pa.f[i][j][k] = f_wr_wd[k];
					else if (j==2) pa.f[i][j][k] = f_wr_dd[k];
					else if (j==3) pa.f[i][j][k] = f_wr_wr[k];
					else if (j==4) pa.f[i][j][k] = f_wr_rr[k];
					else if (j==5) pa.f[i][j][k] = f_wr_dr[k];
				}
				else if (i==4) {
					if (j==0) pa.f[i][j][k] = f_rr_ww[k];
					else if (j==1) pa.f[i][j][k] = f_rr_wd[k];
					else if (j==2) pa.f[i][j][k] = f_rr_dd[k];
					else if (j==3) pa.f[i][j][k] = f_rr_wr[k];
					else if (j==4) pa.f[i][j][k] = f_rr_rr[k];
					else if (j==5) pa.f[i][j][k] = f_rr_dr[k];
				}
				else if (i==5) {
					if (j==0) pa.f[i][j][k] = f_dr_ww[k];
					else if (j==1) pa.f[i][j][k] = f_dr_wd[k];
					else if (j==2) pa.f[i][j][k] = f_dr_dd[k];
					else if (j==3) pa.f[i][j][k] = f_dr_wr[k];
					else if (j==4) pa.f[i][j][k] = f_dr_rr[k];
					else if (j==5) pa.f[i][j][k] = f_dr_dr[k];
				}
			}
		}
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
	for(int i=0; i < num_outcomes; ++i) {
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
void check_counts(int day, char ref) 
{
	long long int tot_F = 0;
	long long int tot_M = 0;
	long long int tot_J = 0;
	long long int tot_V = 0;
	long long int tot_F_B = 0;
	long long int tot_M_B = 0;
	long long int tot_J_B = 0;
	long long int tot_V_B = 0;
	
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i){
			for (int j=0; j < num_gen; ++j) {
				if (sites[pat].F[i][j] < 0) {
					std::cout << day << "\t" << ref << " F neg  " << i << "\t" << j << "\t" << sites[pat].F[i][j] << std::endl;
					std::exit(1);
				}
			}
		}

		for (int i=0; i < num_gen; ++i) {
			tot_F += std::accumulate(sites[pat].F[i].begin(), sites[pat].F[i].end(), 0);
		}

		tot_M += std::accumulate(sites[pat].M.begin(), sites[pat].M.end(), 0);
		tot_V += std::accumulate(sites[pat].V.begin(), sites[pat].V.end(), 0);
		for (int i=0; i < num_gen; ++i) {
			for (int a=0; a < max_dev + 1; ++a) {
				tot_J += sites[pat].J[i][a];
			}
		}
	}

	tot_F_B += std::accumulate(to.F.begin(), to.F.end(), 0);
	tot_M_B += std::accumulate(to.M.begin(), to.M.end(), 0);
	tot_V_B += std::accumulate(to.V.begin(), to.V.end(), 0);
	tot_J_B += std::accumulate(to.J.begin(), to.J.end(), 0);

	if (tot_F != to.tot_F) {
		std::cout << day << "\t" << ref << " F count error  " << tot_F << "\t" << tot_F_B << "\t" << to.tot_F << std::endl;
		std::exit(1);
	}
	if (tot_M != to.tot_M) {
		std::cout << day << "\t" << ref << " M count error  " << tot_M << "\t" << tot_M_B << "\t" << to.tot_M << std::endl;
		std::exit(1);
	}
	if (tot_V != to.tot_V) {
		std::cout << day << "\t" << ref << " V count error  " << tot_V << "\t" << tot_V_B << "\t" << to.tot_V << std::endl;
		std::exit(1);
	}
	if (tot_J != to.tot_J) {
		std::cout << day << "\t" << ref << " J count error  " << tot_J << "\t" << tot_J_B << "\t" << to.tot_J << std::endl;
		std::exit(1);
	}
	if (tot_F_B != to.tot_F) {
		std::cout << day << "\t" << ref << " F count errorB  " << tot_F_B << "\t" << to.tot_F << std::endl;
		std::exit(1);
	}
	if (tot_M_B != to.tot_M) {
		std::cout << day << "\t" << ref << " M count errorB  " << tot_M_B << "\t" << to.tot_M << std::endl;
		std::exit(1);
	}
	if (tot_V_B != to.tot_V) {
		std::cout << day << "\t" << ref << " V count errorB  " << tot_V_B << "\t" << to.tot_V << std::endl;
		std::exit(1);
	}
	if (tot_J_B != to.tot_J) {
		std::cout << day << "\t" << ref << " J count errorB  " << tot_J_B << "\t" << to.tot_J << std::endl;
		std::exit(1);
	}
}