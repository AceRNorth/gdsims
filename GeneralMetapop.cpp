#include <filesystem>
#include "GeneralMetapop.h"

// random number seed
std::random_device rd;
//std::mt19937 twister(rd());
std::mt19937 twister(1);

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
			if (rec_start >= rec_end) throw InvalidIntervalException("rec_start", "rec_end");
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
	RecordParams rec{rec_start, rec_end, rec_interval_global, rec_interval_local, rec_sites_freq, set_label};

	auto start = std::chrono::steady_clock::now();

	// run simulation
	SimController simulation(prog, area, life, inher, rel, disp, aes, initial, rec);
	simulation.set_inheritance();
	simulation.run_reps();

	auto finish = std::chrono::steady_clock::now();
	double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
	std::cout << "Program run time: " << std::endl;
	printf("%.10f\n", elapsed_seconds);
	
	return 0;
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

SimController::SimController(ProgressionParams &prog, AreaParams &area, LifeParams &life, InheritanceParams &inher,
 ReleaseParams &rel, DispersalParams &disp, AestivationParams &aes, InitialPopsParams &initial, RecordParams &rec)
{ 
	num_runs = prog.num_runs;
	max_t = prog.max_t;
	area_params = &area;
	life_params = &life;
	inher_params = &inher;
	rel_params = &rel;
	disp_params = &disp;
	aes_params = &aes;
	initial_params = &initial;
	rec_params = &rec;
	
	for (int i=0; i < num_gen; ++i) {
		for (int j=0; j < num_gen; ++j) {
			for (int k=0; k < num_gen; ++k) {
				f[i][j][k] = 0;
			}
		}
	}
}

// // Runs the simulation
// void SimController::run_sim()
// {
// 	initiate_sim();
// 	run_reps(num_runs);
// }

// // Sets up the model architecture for the simulation
// void SimController::initiate_sim()
// {
// 	set_inheritance();
// }

// Sets the values of the f_{ijk} fraction for the gene drive considering r2 resistant alleles
// f_{ijk} denotes the fraction of genotype k offspring from mother with genotype i mated to father with genotype j
void SimController::set_inheritance()
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
void SimController::run_reps() 
{
	for (int rep=1; rep <= num_runs; ++rep) {
		Model model(area_params, initial_params, life_params, aes_params, disp_params, rel_params);
		Record data(rec_params, rep);
		model.initiate();
		data.record_coords(model.get_sites());
		
		for (int tt=0; tt <= max_t; ++tt) { // current day of the simulation 
			if (model.is_release_time(tt)) {
				model.release_gene_drive(rel_params->num_driver_M, rel_params->num_driver_sites, area_params->num_pat);
			} 
			if (tt > 0) {
				model.run_step(tt, f);
			}

			if (data.is_rec_global_time(tt)) {
				data.output_totals(tt, model.calculate_tot_J(), model.calculate_tot_M(), model.calculate_tot_V(),
				 model.calculate_tot_F());
				data.record_global(tt, model.calculate_tot_M_gen());
			}
			if (data.is_rec_local_time(tt)) {
				data.record_local(tt, model.get_sites());
			}
		}
	}
}

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

	connec_indices.clear();
	connec_weights.clear();
	disp_params = disp;
	aes_params = aes;
	rel_params = rel;
}

Model::~Model() 
{
	for (auto pat : sites) {
		delete pat;
	}
}

// Sets up the model architecture 
void Model::initiate()
{
	sites.clear();
	for (int ii=0; ii < num_pat; ++ii) {
		Patch* pp = new Patch(side);
		sites.push_back(pp);
	
	}
	populate_sites();
	set_dev_duration_probs(min_dev, max_dev);

	set_connecs(side); // Dispersal set-up
}

// Populates all sites with a (wild) mosquito population of different types (age and sex)
void Model::populate_sites() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat]->populate(initial_WJ, initial_WM, initial_WV, initial_WF);
	}
	update_comp();
	update_mate();
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
void Model::update_comp() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat]->update_comp(mu_j, alpha0, mean_dev);
	}
}

// Updates the mating rate parameter in all sites
void Model::update_mate() 
{
	for (int pat=0; pat < sites.size(); ++pat) {
		sites[pat]->update_mate(beta);
	}
}

void Model::run_step(int day, const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f) 
{
	juv_get_older();
	adults_die();
	virgins_mate();
	adults_disperse();
	lay_eggs(f);
	juv_eclose();
	if (day%365 > aes_params->t_hide1 && day%365 <= aes_params->t_hide2 && aes_params->psi > 0.00001) hide();
	if (day%365 > aes_params->t_wake1 && day%365 <= aes_params->t_wake2 && aes_params->psi > 0.00001) wake(day);
	update_comp();
	update_mate();
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

std::size_t Model::get_sites_size() 
{
	return sites.size();
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

// Returns the periodic distance between two points in the simulation area with boundaries x = side, y = side
double Model::distance(double side, std::array<double, 2> point1, std::array<double, 2> point2) 
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

// Computes the inter-patch connectivities
void Model::set_connecs(double side) 
{
	std::vector<int> connec_indices_pat;
	std::vector<double> connec_weights_pat;
	for (int pat=0; pat < sites.size(); ++pat) {
		connec_indices_pat.clear();
		connec_weights_pat.clear();
		for (int new_pat=0; new_pat < sites.size(); ++new_pat) {
			double dd = distance(side, sites[pat]->get_coords(), sites[new_pat]->get_coords());
			if (dd < (disp_params->max_disp)) {
				connec_indices_pat.push_back(new_pat); 
				double weight = (disp_params->max_disp) - dd;
				connec_weights_pat.push_back(weight); 
			}
		}
		connec_indices.push_back(connec_indices_pat);
		connec_weights.push_back(connec_weights_pat);
	}
}

// Selects and updates the number of adults that disperse from and to each patch, depending on the patch connectivities
void Model::adults_disperse() 
{ 
	// adults dispersing out from each patch 
	std::vector<std::array<long long int, num_gen>> m_move = M_dispersing_out(); // males dispersing from each patch
	for (std::size_t pat = 0; pat < sites.size(); ++pat) { // update population numbers
		sites[pat]->M_disperse_out(m_move[pat]);
	}

	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> f_move = F_dispersing_out();
	for (std::size_t pat = 0; pat < sites.size(); ++pat) { 
		sites[pat]->F_disperse_out(f_move[pat]);
	}
		
	// adults dispersing to each patch
	std::vector<long long int> m_disp_by_new_pat;
	for (std::size_t pat=0; pat < sites.size(); ++pat) {
		for (std::size_t i=0; i < num_gen; ++i) {
			// how many males of the given patch and given genotype will disperse to each of its connected patches
			m_disp_by_new_pat = random_multinomial(m_move[pat][i], connec_weights[pat]);

			for (std::size_t new_pat=0; new_pat < m_disp_by_new_pat.size(); ++new_pat) {
				sites[connec_indices[pat][new_pat]]->M_disperse_in(i, m_disp_by_new_pat[new_pat]);
			}
		}
	}

	std::vector<long long int> f_disp_by_new_pat;
	for (std::size_t pat = 0; pat < sites.size(); ++pat) {
		for (std::size_t i = 0; i < num_gen; ++i) {
			for (std::size_t j=0; j < num_gen; ++j) {
				f_disp_by_new_pat = random_multinomial(f_move[pat][i][j], connec_weights[pat]);
				for (std::size_t new_pat=0; new_pat < f_disp_by_new_pat.size(); ++new_pat) {
					sites[connec_indices[pat][new_pat]]->F_disperse_in(i, j, f_disp_by_new_pat[new_pat]);
				}
			}
		}
	}
}

// Returns the number of males (of each genotype) dispersing out from each patch.
std::vector<std::array<long long int, num_gen>> Model::M_dispersing_out()
{
	std::vector<std::array<long long int, num_gen>> m_move;	
	std::array<long long int, num_gen> m;
	std::array<long long int, num_gen> m_out;
	for (int pat=0; pat < sites.size(); ++pat) {
		m = sites[pat]->get_M();
		for (int i=0; i < num_gen; ++i) {
			m_out[i] = random_binomial(m[i], disp_params->disp_rate); // how many males will disperse from the given patch
		}
		m_move.push_back(m_out);
	}
	return m_move;
}

std::vector<std::array<std::array<long long int, num_gen>, num_gen>> Model::F_dispersing_out()
{
	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> f_move; 
	std::array<std::array<long long int, num_gen>, num_gen> f;
	std::array<std::array<long long int, num_gen>, num_gen> f_out;
	for (int pat=0; pat < sites.size(); ++pat) {
		f = sites[pat]->get_F();
		for (int i=0; i < num_gen; ++i) {
			for (int j=0; j < num_gen; ++j) {
				f_out[i][j] = random_binomial(f[i][j], disp_params->disp_rate); // how many females will disperse from the given patch
			}
		}
		f_move.push_back(f_out);
	}
	return f_move;
}

// Calculates the number of mated females going into aestivation on the given day and updates the population numbers, depending on the survival rate of going into aestivation
void Model::hide() 
{
	std::array<std::array<long long int, num_gen>, num_gen> f;
	std::array<std::array<long long int, num_gen>, num_gen> f_try;
	std::array<std::array<long long int, num_gen>, num_gen> f_aes;
	for (int pat=0; pat < sites.size(); ++pat) {
		f = sites[pat]->get_F();
		for (int i=0; i < num_gen; ++i) {
			for (int j=0; j < num_gen; ++j) {
				f_try[i][j] = random_binomial(f[i][j], aes_params->psi); // number of females that attempt to go into aestivation
				f_aes[i][j] = random_binomial(f_try[i][j], 1 - (aes_params->mu_aes));	// number that survive going into aestivation
			}
		}
		sites[pat]->F_hide(f_try, f_aes);
	}
}

// Calculates the number of mated females coming out of aestivation on the given day and updates the population numbers
void Model::wake(int day) 
{
	double prob = 1.0 / (1.0 + (aes_params->t_wake2) - (day%365)); // probability of a female waking on a given day
	
	std::array<std::array<long long int, num_gen>, num_gen> f_w;
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			for(int j=0; j < num_gen; ++j) {
				// number of females that wake up from aestivation on the given day
				f_w[i][j] = random_binomial(sites[pat]->aes_F[i][j], prob);
			}
		}
		sites[pat]->F_wake(f_w);
	}
}

// Releases the gene drive mosquitoes into the simulation area
void Model::release_gene_drive(int num_driver_M, int num_driver_sites, int num_pat)
{
	int num_rel_sites = std::min(num_pat, num_driver_sites);
	std::vector<int> rel_patches = select_driver_sites(num_rel_sites);
	put_driver_sites(rel_patches, num_driver_M);
}

// Selects random sites for release of the gene drive
std::vector<int> Model::select_driver_sites(int num_driver_sites) 
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
void Model::put_driver_sites(const std::vector<int>& patches, int num_driver_M) 
{
	for (const auto& pat : patches) {
		if (pat >= 0 && pat < sites.size()) {
			sites[pat]->add_driver_M(num_driver_M);
		}
	}
	update_mate();
}

bool Model::is_release_time(int day) 
{
	return day == rel_params->driver_start;
}

Patch::Patch(double side) 
{
	double x = random_real() * side;
	double y = random_real() * side;
	coords = {x, y};

	for (int i=0; i < num_gen; ++i) {
		for (int a=0; a < max_dev + 1; ++a) {
			J[i][a] = 0; 
		}
		M[i] = 0;
		V[i] = 0;
		for (int j=0; j < num_gen; ++j) {
			F[i][j] = 0;
			aes_F[i][j] = 0;
		}
	}
	comp = 0;
	mate_rate = 0;
}

// Populates the local site with a (wild) mosquito population of different types (age and sex)
void Patch::populate(int initial_WJ, int initial_WM, int initial_WV, int initial_WF) 
{
	for (int a=0; a < max_dev + 1; ++a) {
		J[0][a] += initial_WJ;
	}	
	M[0] = initial_WM;
	V[0] = initial_WV;
	F[0][0] = initial_WF;
}

// Returns the coordinates of the site on the simulation area
std::array<double, 2> Patch::get_coords() const
{
	return coords;
}

// Returns an array of number of males with each genotype
std::array<long long int, num_gen> Patch::get_M() const
{
	return M;
}

// Returns a 2D array of number of females with each female and carrying mated male genotype
std::array<std::array<long long int, num_gen>, num_gen> Patch::get_F() const
{
	return F;
}

// Returns the total number of juveniles across all ages and genotypes
long long int Patch::calculate_tot_J() 
{
	long long int tot_J = 0;
	for (int i = 0; i < num_gen; ++i) {
		for (int a = 0; a < max_dev+1; ++a) {
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
	for (int i = 0; i < num_gen; ++i) {
		for (int j = 0; j < num_gen; ++j) {
			tot_F += F[i][j];
		}
	}
	return tot_F;
}

// Removes males from the population as they disperse out.
void Patch::M_disperse_out(const std::array<long long int, num_gen> &m_out) 
{
	for (std::size_t i = 0; i < m_out.size(); ++i) {
		M[i] -= m_out[i];
	}
}

// Removes females from the population as they disperse out.
void Patch::F_disperse_out(const std::array<std::array<long long int, num_gen>, num_gen> &f_out) 
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
}

// Introduces new females into the population as they disperse in.
void Patch::F_disperse_in(int f_gen, int m_gen, long long int f_disp) 
{
	F[f_gen][m_gen] += f_disp;
}

// Updates active female population numbers after they attempt to go into aestivation.
void Patch::F_hide(const std::array<std::array<long long int, num_gen>, num_gen> &f_try,
 const std::array<std::array<long long int, num_gen>, num_gen> &f_succeed)
{
	for (std::size_t i = 0; i < F.size(); ++i) {
		for (std::size_t j = 0; j < F[0].size(); ++j) {
			F[i][j] -= f_try[i][j];
			aes_F[i][j] += f_succeed[i][j];
		}
	}
}

// Updates active female population numbers after they wake from aestivation.
void Patch::F_wake(const std::array<std::array<long long int, num_gen>, num_gen> &f_wake)
{
	for (std::size_t i = 0; i < F.size(); ++i) {
		for (std::size_t j = 0; j < F[0].size(); ++j) {
			F[i][j] += f_wake[i][j];
			aes_F[i][j] -= f_wake[i][j];
		}
	}
}

// Introduces driver heterozygous males into the population.
void Patch::add_driver_M(int num_driver_M) 
{
	M[1] += num_driver_M;
}

// Ages the juvenile population in different age groups by a day within the local site
void Patch::juv_get_older(int max_dev) 
{
	for (int i=0; i < num_gen; ++i) {
		for (int a=0; a < max_dev; ++a) {
			// number of juveniles that survive aging by a day are placed into the new older age group	
			J[i][a] = random_binomial(J[i][a+1], comp);
		}
		J[i][max_dev] = 0; // youngest ones have all aged by one day so none left in this age group
	}
}

// Selects the number of adults that die in the given day and updates population numbers within the local site
void Patch::adults_die(double mu_a)
{
	for (int i=0; i < num_gen; ++i) {
		long long int m = random_binomial(M[i], mu_a); // number of males that die
		M[i] -= m;

		long long int v = random_binomial(V[i], mu_a);
		V[i] -= v;	

		for (int j=0; j < num_gen; ++j) {
			long long int f = random_binomial(F[i][j], mu_a);
			F[i][j] -= f;
		}
	}
}

// Selects the number of virgins that mate in the given day with a male of genotype j, and tranforms them into mated females carrying
// male genotype j. Mating is carried out within the local site.
void Patch::virgins_mate() 
{
	std::array<long long int, num_gen> v;
	std::vector<long long int> v_c;
	for (int i=0; i < num_gen; ++i) {
		v[i] = random_binomial(V[i], mate_rate); // how many V will mate
		if (v[i] > 0) {
			v_c = random_multinomial(v[i], M); // how many V with given genotype will carry each of the male genotypes
			for (int j=0; j < num_gen; j++) {
				F[i][j] += v_c[j];
			}
			V[i] -= v[i];
		}
	}
	
}

// Calculates the number of eggs laid on the given day and updates the number of juveniles within the local site, depending on egg
// survival rates.
void Patch::lay_eggs(const std::array<std::array<std::array <double, num_gen>, num_gen>, num_gen> &f, double theta, 
 const std::array<double, max_dev+1> &dev_duration_probs)
{
	std::vector<long long int> j_new;
	for (int i=0; i < num_gen; ++i) {
		for (int j=0; j < num_gen; ++j) {
			for (int k=0; k < num_gen; ++k) {
				double num = theta * F[i][j] * f[i][j][k]; // expected number of eggs laid with k genotype
				long long int eggs = random_poisson(num); // actual number of eggs laid sampled from random distribution

				j_new = random_multinomial(eggs, dev_duration_probs); // number of eggs that start in each different age class (according to different juvenile development times)
				for (int t=0; t < max_dev + 1; ++t) { // juveniles created with assigned remaining time to develop
					J[k][t] += j_new[t];
				}
			}
		}
	}
}

// Turns juveniles into adults, depending on eclosion survival rate, within the local site.
void Patch::juv_eclose() 
{
	for (int i=0; i < num_gen; ++i) {
		long long int surv = random_binomial(J[i][0], comp); // number of juveniles that survive eclosion
		if (surv > 0) {	
			// roughly half of the juveniles become male and half female following a distribution
			long long int surv_m = random_binomial(surv, 0.5); 
			M[i] += surv_m; 
			V[i] += surv - surv_m;
		}
	}
}

// Updates the juvenile survival probability in the local site
void Patch::update_comp(double mu_j, double alpha0, double mean_dev)
{
	long long int tot_J = calculate_tot_J();
	comp = (1 - mu_j) * std::pow(alpha0 / (alpha0 + tot_J), 1.0 / mean_dev);
}

// Updates the mating rate parameter in the local site
void Patch::update_mate(double beta)
{
	long long int tot_M = calculate_tot_M();
	mate_rate = tot_M / (beta + tot_M);
}

// Creates LocalData, Totals and CoordinateList output .txt files
Record::Record(RecordParams *rec_params, int rep) 
{
	rec_start = rec_params->rec_start;
	rec_end = rec_params->rec_end;
	rec_interval_global = rec_params->rec_interval_global;
	rec_interval_local = rec_params->rec_interval_local;
	rec_sites_freq = rec_params->rec_sites_freq;
	set_label = rec_params->set_label;
	rep_label = rep;

	std::filesystem::path output_path = "C:\\Users\\biol0117\\OneDrive - Nexus365\\Documents\\Programming projects\\C++ Model\\GeneralMetapop\\Output files";
	std::filesystem::current_path(output_path);
	
	os1 << "LocalData" << set_label << "run" << rep_label << ".txt"; 
	local_data.open(os1.str());
	os2 << "Totals" << set_label << "run" << rep_label << ".txt";
	global_data.open(os2.str());
	os3 << "CoordinateList" << set_label << "run" << rep_label << ".txt";
	coord_list.open(os3.str());

	local_data << "Male populations of each genotype at each site\n";
	local_data << "Day" << "\t" << "Site" << "\t" << "WW" << "\t" << "WD" << "\t" << "DD" << "\t" << "WR" << "\t" << "RR" << "\t" << "DR" << std::endl;

	global_data << "Total males of each genotype\n";
	global_data << "Day" << "\t" << "WW" << "\t" << "WD" << "\t" << "DD" << "\t" << "WR" << "\t" << "RR" << "\t" << "DR" << std::endl;

	coord_list << "Coordinate list of the sites\n";
	coord_list << "Site" << "\t" << "x" << "\t" << "y" << std::endl;
}

// Records the x and y coordinates of each site
void Record::record_coords(const std::vector<Patch*> &sites) 
{
	for (int pat=0; pat < sites.size(); pat += rec_sites_freq) {
		std::array<double, 2> pat_coords = sites[pat]->get_coords();
		coord_list << pat+1 << "\t" << pat_coords[0] << "\t" << pat_coords[1] << std::endl;
	}
}

// Records the total number of males (over all sites) of each genotype
void Record::record_global(int day, const std::array<long long int, num_gen> &tot_M_gen)
{
	global_data << day;
	for (const auto& m_gen : tot_M_gen) {
		global_data << "\t" << m_gen;
	}
	global_data << std::endl;
}

// Outputs to screen the J, M, V, F totals over the simulation area for the given day
void Record::output_totals(int day, long long int tot_J, long long int tot_M, long long int tot_V, long long int tot_F)
{
	std::cout << day << "\t" << tot_J << "\t" << tot_M << "\t" << tot_V << "\t" << tot_F << std::endl;
}


// Records the number of males of each genotype at each site
void Record::record_local(int day, const std::vector<Patch*> &sites) 
{
	for (int pat=0; pat < sites.size(); pat += rec_sites_freq) {
		local_data << day << "\t" << pat+1;
		for (const auto& m_gen : sites[pat]->get_M()) {
			local_data << "\t" << m_gen;
		}
		local_data << std::endl;
	}
}

bool Record::is_rec_global_time(int day)
{
	return day % rec_interval_global == 0;
}

bool Record::is_rec_local_time(int day) 
{
	return (day == 0) || (day >= rec_start && day <= rec_end && day % rec_interval_local == 0);
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
