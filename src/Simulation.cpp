#include <array>
#include <vector>
#include <iostream> // for error messages
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include "Simulation.h"
#include "constants.h"
#include "Model.h"
#include "Record.h"
#include "inputval.h"

using namespace constants;

Simulation::Simulation(ProgressionParams &prog, AreaParams &area, LifeParams &life, ReleaseParams &rel, DispersalParams &disp,
 AestivationParams &aes, InitialPopsParams &initial, RecordParams &rec, double a0_mean, double a0_var, double a1, double ampl)
{ 
	num_runs = prog.num_runs;
	max_t = prog.max_t;
	area_params = &area;
	life_params = &life;
	rel_params = &rel;
	disp_params = &disp;
	aes_params = &aes;
	initial_params = &initial;
	rec_params = &rec;
	alpha0_mean = a0_mean;
	alpha0_variance = a0_var;
	alpha1 = a1;
	amp = ampl;
	
	for (int i=0; i < num_gen; ++i) {
		for (int j=0; j < num_gen; ++j) {
			for (int k=0; k < num_gen; ++k) {
				inher_fraction[i][j][k] = 0;
			}
		}
	}

	sites_coords.clear();
	rainfall.clear();
	boundary_type = BoundaryType::Toroid;
	disp_type = DispersalType::DistanceKernel;
}

// Sets the sites' coordinates from a .txt file, unless any errors are thrown.
void Simulation::set_coords(const std::string& filename) 
{
	sites_coords.clear();

	auto filepath = std::filesystem::path(std::string("./")+filename);
	if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath)) {
		std::cerr << "Invalid filename. Make sure the file is in the program directory." << std::endl;
	}
	else {
		std::ifstream file(filename);
		std::string line;
		std::vector<Point> temp;
		if (file.is_open()) {
			for(int i=0; std::getline(file, line); ++i) {
				std::stringstream linestream(line);
				if (line.size() == 0) break;

				double x, y;
				int err = 0;
				if (!read_and_validate_type(linestream, x, "x" + std::to_string(i+1), "double")) err++;
				if (!read_and_validate_type(linestream, y, "y" + std::to_string(i+1), "double")) err++;
				
				if (boundary_type == Toroid) {
					if (!check_bounds("x" + std::to_string(i+1), x, 0.0, true, area_params->side, true)) err++;
					if (!check_bounds("y" + std::to_string(i+1), y, 0.0, true, area_params->side, true)) err++;
				}
				if (err == 0) {
					temp.push_back({x, y});
				}
			}
		}
		file.close();

		if (temp.size() != area_params->num_pat) {
			std::cerr << "Error: the number of valid coordinates in the file does not match num_pat." << std::endl;
		}
		else {
			sites_coords = temp;
		}
	}		
}

void Simulation::set_boundary_type(BoundaryType boundary) 
{
	boundary_type = boundary;
}

void Simulation::set_dispersal_type(DispersalType disp)
{
	disp_type = disp;
}


// Sets the daily rainfall values from a .txt file, unless any errors are thrown. 
// These can be daily values for a year cycle, or daily values for all the simulated days.
// A year is assumed to be 365 days.
void Simulation::set_rainfall(double res, const std::string& filename)
{
	resp = res;
	rainfall.clear();

	auto filepath = std::filesystem::path(std::string("./")+filename);
	if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath)) {
		std::cerr << "Invalid filename. Make sure the file is in the program directory." << std::endl;
	}
	else {
		std::ifstream file(filename);
		std::string line;
		std::vector<double> temp;
		if (file.is_open()) {
			for(int i=0; std::getline(file, line); ++i) {
				std::stringstream linestream(line);
				if (line.size() == 0) break;

				double r_d;
				int err = 0;
				if (!read_and_validate_type(linestream, r_d, "rainfall_day" + std::to_string(i+1), "double")) err++;
				if (!check_bounds("rainfall_day" + std::to_string(i+1), r_d, 0.0, true)) err++;

				if (err == 0) {
					temp.push_back(r_d);
				}
			}
		}
		file.close();

		if (temp.size() == 365 || temp.size() == max_t) {
			rainfall = temp;
		}
		else {
			std::cout << "temp size: " << temp.size() << std::endl;
			std::cerr << "Error: the number of valid daily rainfall values in the file is not 365 or max_t." << std::endl;
		}
	}		
}

// Sets the values of the f_{ijk} fraction for the gene drive considering r2 resistant alleles
// f_{ijk} denotes the fraction of genotype k offspring from mother with genotype i mated to father with genotype j
void Simulation::set_inheritance(InheritanceParams inher_params)
{
	double gamma = inher_params.gamma;
	double xi = inher_params.xi;
	double e = inher_params.e;

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
					if (j==0) inher_fraction[i][j][k] = f_ww_ww[k];
					else if (j==1) inher_fraction[i][j][k] = f_ww_wd[k];
					else if (j==2) inher_fraction[i][j][k] = f_ww_dd[k];
					else if (j==3) inher_fraction[i][j][k] = f_ww_wr[k];
					else if (j==4) inher_fraction[i][j][k] = f_ww_rr[k];
					else if (j==5) inher_fraction[i][j][k] = f_ww_dr[k];
				}
				else if (i==1) {
					if (j==0) inher_fraction[i][j][k] = f_wd_ww[k];
					else if (j==1) inher_fraction[i][j][k] = f_wd_wd[k];
					else if (j==2) inher_fraction[i][j][k] = f_wd_dd[k];
					else if (j==3) inher_fraction[i][j][k] = f_wd_wr[k];
					else if (j==4) inher_fraction[i][j][k] = f_wd_rr[k];
					else if (j==5) inher_fraction[i][j][k] = f_wd_dr[k];
				}
				else if (i==2) {
					if (j==0) inher_fraction[i][j][k] = f_dd_ww[k];
					else if (j==1) inher_fraction[i][j][k] = f_dd_wd[k];
					else if (j==2) inher_fraction[i][j][k] = f_dd_dd[k];
					else if (j==3) inher_fraction[i][j][k] = f_dd_wr[k];
					else if (j==4) inher_fraction[i][j][k] = f_dd_rr[k];
					else if (j==5) inher_fraction[i][j][k] = f_dd_dr[k];
				}
				else if (i==3) {
					if (j==0) inher_fraction[i][j][k] = f_wr_ww[k];
					else if (j==1) inher_fraction[i][j][k] = f_wr_wd[k];
					else if (j==2) inher_fraction[i][j][k] = f_wr_dd[k];
					else if (j==3) inher_fraction[i][j][k] = f_wr_wr[k];
					else if (j==4) inher_fraction[i][j][k] = f_wr_rr[k];
					else if (j==5) inher_fraction[i][j][k] = f_wr_dr[k];
				}
				else if (i==4) {
					if (j==0) inher_fraction[i][j][k] = f_rr_ww[k];
					else if (j==1) inher_fraction[i][j][k] = f_rr_wd[k];
					else if (j==2) inher_fraction[i][j][k] = f_rr_dd[k];
					else if (j==3) inher_fraction[i][j][k] = f_rr_wr[k];
					else if (j==4) inher_fraction[i][j][k] = f_rr_rr[k];
					else if (j==5) inher_fraction[i][j][k] = f_rr_dr[k];
				}
				else if (i==5) {
					if (j==0) inher_fraction[i][j][k] = f_dr_ww[k];
					else if (j==1) inher_fraction[i][j][k] = f_dr_wd[k];
					else if (j==2) inher_fraction[i][j][k] = f_dr_dd[k];
					else if (j==3) inher_fraction[i][j][k] = f_dr_wr[k];
					else if (j==4) inher_fraction[i][j][k] = f_dr_rr[k];
					else if (j==5) inher_fraction[i][j][k] = f_dr_dr[k];
				}
			}
		}
	}	
}

// Runs the simulation n times, recording data in output files.
void Simulation::run_reps() 
{
	for (int rep=1; rep <= num_runs; ++rep) {
		Model* model;
		if (rainfall.empty()) {
			model = new Model(area_params, initial_params, life_params, aes_params, disp_params, rel_params, alpha0_mean,
			 alpha0_variance, alpha1, amp, boundary_type, disp_type, sites_coords);
		}
		else {
			model = new Model(area_params, initial_params, life_params, aes_params, disp_params, rel_params, alpha0_mean,
			 alpha0_variance, alpha1, resp, rainfall, boundary_type, disp_type, sites_coords);
		}
		Record data(rec_params, rep);
		model->initiate();
		data.record_coords(model->get_sites());

		for (int tt=0; tt <= max_t; ++tt) { // current day of the simulation 
			model->run(tt, inher_fraction);

			if (data.is_rec_global_time(tt)) {
				data.output_totals(tt, model->calculate_tot_J(), model->calculate_tot_M(), model->calculate_tot_V(),
				 model->calculate_tot_F());
				data.record_global(tt, model->calculate_tot_M_gen());
			}
			if (data.is_rec_local_time(tt)) {
				data.record_local(tt, model->get_sites());
			}
		}

		delete model;
	}
}