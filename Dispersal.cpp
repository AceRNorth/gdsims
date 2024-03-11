#include <vector>
#include <array>
#include <cmath>
#include "Dispersal.h"
#include "random.h"
#include "constants.h"

#include <iostream>

using namespace constants;

Dispersal::Dispersal(DispersalParams* params, BoundaryType boundary, double side, ConnecType connec) 
{
	disp_rate = params->disp_rate;
	max_disp = params->max_disp;
	
	if (boundary == Toroid) {
		boundary_strategy = new ToroidalBoundaryStrategy(side);
	}
	else if (boundary == Edge) {
		boundary_strategy = new EdgeBoundaryStrategy(side);
	}
	else {
		boundary_strategy = new ToroidalBoundaryStrategy(side);
	}
	
	if (connec == Simple) {
		connec_strategy = new SimpleConnecStrategy(max_disp, boundary_strategy);
	}
	else if (connec == Wedge) {
		connec_strategy = new WedgeConnecStrategy(max_disp, boundary_strategy);
	}
	else {
		connec_strategy = new SimpleConnecStrategy(max_disp, boundary_strategy);
	}
}

Dispersal::~Dispersal()
{
	delete boundary_strategy;
	delete connec_strategy;
}

// Sets the inter-patch connectivities
void Dispersal::set_connecs(std::vector<Patch*> &sites)
{
	connec_indices.clear();
	connec_weights.clear();
	auto connecs = connec_strategy->compute_connecs(sites);
	connec_indices = connecs.first;
	connec_weights = connecs.second;
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