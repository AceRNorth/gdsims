#include <vector>
#include <array>
#include <cmath>
#include "Dispersal.h"
#include "random.h"
#include "constants.h"

using namespace constants;

Dispersal::Dispersal(DispersalParams *params) 
{
	disp_rate = params->disp_rate;
	max_disp = params->max_disp;
}

// Computes the inter-patch connectivities (connec_indices and connec_weights)
void Dispersal::set_connecs(double side, std::vector<Patch*> &sites)
{
	std::vector<int> connec_indices_pat;
	std::vector<double> connec_weights_pat;
	for (int pat=0; pat < sites.size(); ++pat) {
		connec_indices_pat.clear();
		connec_weights_pat.clear();
		for (int new_pat=0; new_pat < sites.size(); ++new_pat) {
			double dd = distance(side, sites[pat]->get_coords(), sites[new_pat]->get_coords());
			if (dd < max_disp) {
				connec_indices_pat.push_back(new_pat); 
				double weight = max_disp - dd;
				connec_weights_pat.push_back(weight); 
			}
		}
		connec_indices.push_back(connec_indices_pat);
		connec_weights.push_back(connec_weights_pat);
	}
}

// Carries out dispersal by adults from and to each patch, depending on the patch connectivities
void Dispersal::adults_disperse(std::vector<Patch*> &sites)
{
	// adults dispersing out from each patch 
	std::vector<std::array<long long int, num_gen>> m_move = M_dispersing_out(sites); // males dispersing from each patch
	for (int pat = 0; pat < sites.size(); ++pat) { // update population numbers
		sites[pat]->M_disperse_out(m_move[pat]);
	}

	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> f_move = F_dispersing_out(sites);
	for (int pat = 0; pat < sites.size(); ++pat) { 
		sites[pat]->F_disperse_out(f_move[pat]);
	}
		
	// adults dispersing into each patch
	std::vector<long long int> m_disp_by_new_pat;
	for (int pat=0; pat < sites.size(); ++pat) {
		for (int i=0; i < num_gen; ++i) {
			// how many males of the given patch and given genotype will disperse to each of its connected patches
			m_disp_by_new_pat = random_multinomial(m_move[pat][i], connec_weights[pat]);
			for (int new_pat=0; new_pat < m_disp_by_new_pat.size(); ++new_pat) { // update population numbers
				sites[connec_indices[pat][new_pat]]->M_disperse_in(i, m_disp_by_new_pat[new_pat]);
			}
		}
	}

	std::vector<long long int> f_disp_by_new_pat;
	for (int pat = 0; pat < sites.size(); ++pat) {
		for (int i = 0; i < num_gen; ++i) {
			for (int j=0; j < num_gen; ++j) {
				f_disp_by_new_pat = random_multinomial(f_move[pat][i][j], connec_weights[pat]);
				for (int new_pat=0; new_pat < f_disp_by_new_pat.size(); ++new_pat) {
					sites[connec_indices[pat][new_pat]]->F_disperse_in(i, j, f_disp_by_new_pat[new_pat]);
				}
			}
		}
	}
}

// Returns the periodic distance between two points in the simulation area with boundaries x = side, y = side
double Dispersal::distance(double side, std::array<double, 2> point1, std::array<double, 2> point2) 
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

// Returns the number of males (of each genotype) dispersing out from each patch.
std::vector<std::array<long long int, num_gen>> Dispersal::M_dispersing_out(const std::vector<Patch*> &sites) 
{
	std::vector<std::array<long long int, num_gen>> m_move;	
	std::array<long long int, num_gen> m;
	std::array<long long int, num_gen> m_out;
	for (int pat=0; pat < sites.size(); ++pat) {
		m = sites[pat]->get_M();
		for (int i=0; i < num_gen; ++i) {
			m_out[i] = random_binomial(m[i], disp_rate); // how many males will disperse from the given patch
		}
		m_move.push_back(m_out);
	}
	return m_move;
}

// Returns the number of females (of each genotype combination) dispersing out from each patch.
std::vector<std::array<std::array<long long int, num_gen>, num_gen>> Dispersal::F_dispersing_out(const std::vector<Patch*> &sites)
{
	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> f_move; 
	std::array<std::array<long long int, num_gen>, num_gen> f;
	std::array<std::array<long long int, num_gen>, num_gen> f_out;
	for (int pat=0; pat < sites.size(); ++pat) {
		f = sites[pat]->get_F();
		for (int i=0; i < num_gen; ++i) {
			for (int j=0; j < num_gen; ++j) {
				f_out[i][j] = random_binomial(f[i][j], disp_rate); // how many females will disperse from the given patch
			}
		}
		f_move.push_back(f_out);
	}
	return f_move;
}