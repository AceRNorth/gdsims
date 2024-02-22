#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <fstream>
#include <string>
#include <limits> 

// Checks if a value falls within bounds. 
template <typename T>
bool check_bounds(const std::string &par_name, T value, T lower_bound, bool inclusive_lower = true,
 T upper_bound = std::numeric_limits<T>::max(), bool inclusive_upper = true) 
{
    if (lower_bound >= upper_bound) {
        std::cerr << "Error: lower_bound must be less than upper_bound." << std::endl;
        return false;
    }

    bool lower_check = inclusive_lower ? value >= lower_bound : value > lower_bound;
    bool upper_check = inclusive_upper ? value <= upper_bound : value < upper_bound;

    std::string lower_bound_char = inclusive_lower ? "\u2264" : "<";
    std::string upper_bound_char = inclusive_upper ? "\u2264" : "<";
    if (!lower_check || !upper_check) {
        std::cout << "The parameter " << par_name << " is out of bounds. ";
        std::cout << par_name << " should be " << lower_bound << " " << lower_bound_char << " " << par_name << " ";
        std::cout <<  upper_bound_char << " " << upper_bound << ". " << std::endl;
    }

    return lower_check && upper_check;
}

// Checks if two values constitute a valid interval.
template <typename T>
bool check_interval(const std::string &start_name, const std::string &stop_name, T start, T stop) {
	if (stop <= start) {
		std::cout << "The parameters " << start_name << ", " << stop_name << " do not constitute a valid interval." << std::endl;
		return false;
	}
	return true;
}

// Reads a value from filestream and assigns it to the parameter variable if the types match.
template <typename T>
bool read_and_validate_type(std::ifstream &file, T &par, const std::string &par_name, const std::string &par_type) 
{
	if (!(file >> par)) {
        std::cerr << "Error: invalid type for " << par_name << ". Expected " << par_type << "." << std::endl;
        return false;
    }
    return true;
}

struct InputParams {
	int num_runs; // number of simulation replicates to run
	int max_t; // maximum simulated time (in days)
	int num_pat; // number of population sites chosen for the simulation
	double side; // size of the square simulation area (side x side) (km)
	double mu_j; // juvenile density independent mortality rate per day
	double mu_a; // adult mortality rate per day
	double beta; // parameter that controls mating rate
	double theta; // average egg laying rate of wildtype females (eggs per day)
	double alpha0; // baseline contribution to carrying capacity
	double mean_dev; // mean juvenile development time (in days)
	int min_dev; // minimum development time for a juvenile (in days)
	double gamma; // rate of r2 allele formation from W/D meiosis
	double xi; // somatic Cas9 expression fitness cost
	double e; // homing rate in females
	int driver_start; // time to start releasing drive alleles into the mosquito population
	int num_driver_M; // number of drive heterozygous (WD) male mosquitoes per release
	int num_driver_sites; // number of gene drive release sites per year
	double disp_rate; // adult dispersal rate
	double max_disp; // maximum distance at which two sites are connected (km)
	double psi; // aestivation rate
	double mu_aes; // aestivation mortality
	int t_hide1; // start day of aestivation-entering period (day number of the year), not included
	int t_hide2; // end day of aestivation-entering period (day number of the year)
	int t_wake1; // start day of aestivation-waking period (day number of the year), not included
	int t_wake2; // end day of aestivation-waking period (day number of the year)
	int initial_WJ; // array of number of initial juvenile mosquitoes with wild homozygous (WW) genotype for each age group
	int initial_WM; // number of initial adult male mosquitoes with wild homozygous (WW) genotype
	int initial_WV; // number of initial adult unmated female (virgin) mosquitoes with wild homozygous (WW) genotype
	int initial_WF; // number of initial adult mated female mosquitoes with wild homozygous (WW) genotype
	int rec_start; // start time for the data recording window (in days) (non-inclusive)
	int rec_end; // end time for the data recording window (in days) (inclusive)
	int rec_interval_global; // time interval for global data recording/output
	int rec_interval_local; // time interval at which to collect/record local data (in days)
	int rec_sites_freq; // fraction of sites to collect local data for (1 is all sites, 10 is 1 in 10 etc)
	int set_label; // 'set of runs' index label for output files
};

InputParams set1 = {
    .num_runs = 2, 
    .max_t = 1000,
    .num_pat = 50,
    .side = 1.0,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 1000, 
    .num_driver_sites = 5,
    .disp_rate = 0.01,
    .max_disp = 0.2, 
    .psi = 0.0,
    .mu_aes = 0.0, 
    .t_hide1 = 0,
    .t_hide2 = 0, 
    .t_wake1 = 0, 
    .t_wake2 = 0,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 200,
    .rec_sites_freq = 1,
    .set_label = 1
};

InputParams set2 = {
    .num_runs = 2, 
    .max_t = 1000,
    .num_pat = 1,
    .side = 1.0,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 1000, 
    .num_driver_sites = 5,
    .disp_rate = 0.01,
    .max_disp = 0.2, 
    .psi = 0.0,
    .mu_aes = 0.0, 
    .t_hide1 = 0,
    .t_hide2 = 0, 
    .t_wake1 = 0, 
    .t_wake2 = 0,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 200,
    .rec_sites_freq = 1,
    .set_label = 2
};

InputParams set3 = {
    .num_runs = 2, 
    .max_t = 1000,
    .num_pat = 50,
    .side = 1.0,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 1000, 
    .num_driver_sites = 5,
    .disp_rate = 0,
    .max_disp = 0.2, 
    .psi = 0.0,
    .mu_aes = 0.0, 
    .t_hide1 = 0,
    .t_hide2 = 0, 
    .t_wake1 = 0, 
    .t_wake2 = 0,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 200,
    .rec_sites_freq = 1,
    .set_label = 3
};

InputParams set4 = {
    .num_runs = 2, 
    .max_t = 1000,
    .num_pat = 50,
    .side = 1.0,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 1000, 
    .num_driver_sites = 5,
    .disp_rate = 1,
    .max_disp = 0.2, 
    .psi = 0.0,
    .mu_aes = 0.0, 
    .t_hide1 = 0,
    .t_hide2 = 0, 
    .t_wake1 = 0, 
    .t_wake2 = 0,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 200,
    .rec_sites_freq = 1,
    .set_label = 4
};

InputParams set5 = {
    .num_runs = 2, 
    .max_t = 1,
    .num_pat = 50,
    .side = 1.0,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 1000, 
    .num_driver_sites = 5,
    .disp_rate = 0.01,
    .max_disp = 0.2, 
    .psi = 0.0,
    .mu_aes = 0.0, 
    .t_hide1 = 0,
    .t_hide2 = 0, 
    .t_wake1 = 0, 
    .t_wake2 = 0,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 1,
    .rec_sites_freq = 1,
    .set_label = 5
};

InputParams set6 = {
    .num_runs = 2, 
    .max_t = 1000,
    .num_pat = 50,
    .side = 0.1,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 1000, 
    .num_driver_sites = 5,
    .disp_rate = 0.01,
    .max_disp = 0.06, 
    .psi = 0.0,
    .mu_aes = 0.0, 
    .t_hide1 = 0,
    .t_hide2 = 0, 
    .t_wake1 = 0, 
    .t_wake2 = 0,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 200,
    .rec_sites_freq = 1,
    .set_label = 6
};

InputParams set7 = {
    .num_runs = 2, 
    .max_t = 1000,
    .num_pat = 50,
    .side = 1.0,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 1000, 
    .num_driver_sites = 5,
    .disp_rate = 0.01,
    .max_disp = 0.2, 
    .psi = 0.01,
    .mu_aes = 0.95, 
    .t_hide1 = 280,
    .t_hide2 = 310, 
    .t_wake1 = 100, 
    .t_wake2 = 130,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 200,
    .rec_sites_freq = 1,
    .set_label = 7
};

InputParams set8 = {
    .num_runs = 2, 
    .max_t = 1000,
    .num_pat = 50,
    .side = 1.0,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 1000, 
    .num_driver_sites = 5,
    .disp_rate = 0.01,
    .max_disp = 0.2, 
    .psi = 0.5,
    .mu_aes = 0.8, 
    .t_hide1 = 280,
    .t_hide2 = 310, 
    .t_wake1 = 100, 
    .t_wake2 = 130,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 200,
    .rec_sites_freq = 1,
    .set_label = 8
};

InputParams set9 = {
    .num_runs = 2, 
    .max_t = 1000,
    .num_pat = 50,
    .side = 1.0,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 0, 
    .num_driver_sites = 0,
    .disp_rate = 0.01,
    .max_disp = 0.2, 
    .psi = 0.0,
    .mu_aes = 0.0, 
    .t_hide1 = 0,
    .t_hide2 = 0, 
    .t_wake1 = 0, 
    .t_wake2 = 0,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 200,
    .rec_sites_freq = 1,
    .set_label = 9
};

InputParams set10 = {
    .num_runs = 2, 
    .max_t = 1000,
    .num_pat = 50,
    .side = 1.0,
    .mu_j = 0.05, 
    .mu_a = 0.125,
    .beta = 100.0,
    .theta = 9.0,
    .alpha0 = 100000.0,
    .mean_dev = 15.0,
    .min_dev = 10,
    .gamma = 0.025,
    .xi = 0.2, 
    .e = 0.95,
    .driver_start = 200,
    .num_driver_M = 1000, 
    .num_driver_sites = 50,
    .disp_rate = 0.01,
    .max_disp = 0.2, 
    .psi = 0.0,
    .mu_aes = 0.0, 
    .t_hide1 = 0,
    .t_hide2 = 0, 
    .t_wake1 = 0, 
    .t_wake2 = 0,
    .rec_start = 0,
    .rec_end = 1000,
    .rec_interval_global = 1,
    .rec_interval_local = 200,
    .rec_sites_freq = 1,
    .set_label = 10
};

#endif //INPUT_H
