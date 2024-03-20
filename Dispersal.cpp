#include <vector>
#include <array>
#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>
#include "Dispersal.h"
#include "random.h"
#include "constants.h"

#include <iostream>

using namespace constants;

const double PI = 3.14159265;
const double TWOPI = 2 * PI;

Dispersal::Dispersal(DispersalParams* params, BoundaryType boundary, double side) 
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
	
	// if (connec == Simple) {
	// 	connec_strategy = new DistanceKernelDispersal(params, boundary_type, side);
	// }
	// else if (connec == Wedge) {
	// 	connec_strategy = new WedgeDispersal(params, boundary_type, side);
	// }
	// else {
	// 	connec_strategy = new DistanceKernelDispersal(params, boundary_type, side);
	// }
}

Dispersal::~Dispersal()
{
	delete boundary_strategy;
}

// Sets the inter-patch connectivities
// void Dispersal::set_connecs(std::vector<Patch*> &sites)
// {
// 	connec_indices.clear();
// 	connec_weights.clear();
// 	auto connecs = connec_strategy->compute_connecs(sites);
// 	connec_indices = connecs.first;
// 	connec_weights = connecs.second;
// }

// Carries out dispersal by adults from and to each patch, depending on the patch connectivities
// void Dispersal::adults_disperse(std::vector<Patch*> &sites)
// {
// 	// adults dispersing out from each patch 
// 	std::vector<std::array<long long int, num_gen>> m_move = M_dispersing_out(sites); // males dispersing from each patch
// 	for (int pat = 0; pat < sites.size(); ++pat) { // update population numbers
// 		sites[pat]->M_disperse_out(m_move[pat]);
// 	}

// 	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> f_move = F_dispersing_out(sites);
// 	for (int pat = 0; pat < sites.size(); ++pat) { 
// 		sites[pat]->F_disperse_out(f_move[pat]);
// 	}
		
// 	// adults dispersing into each patch
// 	std::vector<long long int> m_disp_by_new_pat;
// 	for (int pat=0; pat < sites.size(); ++pat) {
// 		for (int i=0; i < num_gen; ++i) {
// 			// how many males of the given patch and given genotype will disperse to each of its connected patches
// 			m_disp_by_new_pat = random_multinomial(m_move[pat][i], connec_weights[pat]);
// 			for (int new_pat=0; new_pat < m_disp_by_new_pat.size(); ++new_pat) { // update population numbers
// 				sites[connec_indices[pat][new_pat]]->M_disperse_in(i, m_disp_by_new_pat[new_pat]);
// 			}
// 		}
// 	}

// 	std::vector<long long int> f_disp_by_new_pat;
// 	for (int pat = 0; pat < sites.size(); ++pat) {
// 		for (int i = 0; i < num_gen; ++i) {
// 			for (int j=0; j < num_gen; ++j) {
// 				f_disp_by_new_pat = random_multinomial(f_move[pat][i][j], connec_weights[pat]);
// 				for (int new_pat=0; new_pat < f_disp_by_new_pat.size(); ++new_pat) {
// 					sites[connec_indices[pat][new_pat]]->F_disperse_in(i, j, f_disp_by_new_pat[new_pat]);
// 				}
// 			}
// 		}
// 	}
// }

// Returns the number of males (of each genotype) dispersing out from each patch.
std::vector<std::array<long long int, num_gen>> Dispersal::M_dispersing_out(const std::vector<Patch*> &sites) 
{
	std::vector<std::array<long long int, num_gen>> m_move;	
	std::array<long long int, num_gen> m;
	std::array<long long int, num_gen> m_out;
	// if (sites.size() == 1) {
	// 	for (int pat=0; pat < sites.size(); ++pat) {
	// 		// m_out.fill(0);
	// 		m = sites[pat]->get_M();
	// 		for (int i=0; i < num_gen; ++i) {
	// 			m_out[i] = random_binomial(m[i], disp_rate); // how many males will disperse from the given patch
	// 		}
	// 		m_out.fill(0);
	// 		m_move.push_back(m_out);
	// 	}
	// }
	//else {
		for (int pat=0; pat < sites.size(); ++pat) {
			m = sites[pat]->get_M();
			for (int i=0; i < num_gen; ++i) {
				m_out[i] = random_binomial(m[i], disp_rate); // how many males will disperse from the given patch
			}
			m_move.push_back(m_out);
		}
	//}
	return m_move;
}

// Returns the number of females (of each genotype combination) dispersing out from each patch.
std::vector<std::array<std::array<long long int, num_gen>, num_gen>> Dispersal::F_dispersing_out(const std::vector<Patch*> &sites)
{
	std::vector<std::array<std::array<long long int, num_gen>, num_gen>> f_move; 
	std::array<std::array<long long int, num_gen>, num_gen> f;
	std::array<std::array<long long int, num_gen>, num_gen> f_out;
	// if (sites.size() == 1) {
	// 	for (int pat=0; pat < sites.size(); ++pat) {
	// 		// for (int i=0; i < num_gen; ++i) {
	// 		// 	for (int j=0; j < num_gen; ++j) {
	// 		// 		f_out[i][j] = 0; 
	// 		// 	}
	// 		// }

	// 		f = sites[pat]->get_F();
	// 		for (int i=0; i < num_gen; ++i) {
	// 			for (int j=0; j < num_gen; ++j) {
	// 				f_out[i][j] = random_binomial(f[i][j], disp_rate); // how many females will disperse from the given patch
	// 			}
	// 		}

	// 		for (int i=0; i < num_gen; ++i) {
	// 			for (int j=0; j < num_gen; ++j) {
	// 				f_out[i][j] = 0; 
	// 			}
	// 		}
	// 		f_move.push_back(f_out);
	// 	}
	// }
	//else {
		for (int pat=0; pat < sites.size(); ++pat) {
			f = sites[pat]->get_F();
			for (int i=0; i < num_gen; ++i) {
				for (int j=0; j < num_gen; ++j) {
					f_out[i][j] = random_binomial(f[i][j], disp_rate); // how many females will disperse from the given patch
				}
			}
			f_move.push_back(f_out);
		}
	//}
	return f_move;
}

// Sets the inter-patch connectivities
void DistanceKernelDispersal::set_connecs(std::vector<Patch*> &sites) {
	connec_indices.clear();
	connec_weights.clear();
	auto connecs = compute_connecs(sites);
	connec_indices = connecs.first;
	connec_weights = connecs.second;
}

// Carries out dispersal by adults from and to each patch, depending on the patch connectivities
void DistanceKernelDispersal::adults_disperse(std::vector<Patch*> &sites) {
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

// Computes the set of connection indices and weights for a group of patches based on the simple connection assumptions
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> DistanceKernelDispersal::compute_connecs(std::vector<Patch*>
 &sites) 
{
    std::vector<std::vector<int>> connec_indices;
    std::vector<std::vector<double>> connec_weights;
    std::vector<int> connec_indices_pat;
	std::vector<double> connec_weights_pat;
	for (int pat=0; pat < sites.size(); ++pat) {
		connec_indices_pat.clear();
		connec_weights_pat.clear();
		for (int new_pat=0; new_pat < sites.size(); ++new_pat) {
			double dd = boundary_strategy->distance(sites[pat]->get_coords(), sites[new_pat]->get_coords());
			if (dd < max_disp) {
				connec_indices_pat.push_back(new_pat); 
				double weight = max_disp - dd;
				connec_weights_pat.push_back(weight); 
			}
		}
		connec_indices.push_back(connec_indices_pat);
		connec_weights.push_back(connec_weights_pat);
	}

    return {connec_indices, connec_weights};
}

// Sets the inter-patch connectivities
void WedgeDispersal::set_connecs(std::vector<Patch*> &sites) {
	connec_indices.clear();
	connec_weights.clear();
	auto connecs = compute_connecs(sites);
	connec_indices = connecs.first;
	connec_weights = connecs.second;
}

// Carries out dispersal by adults from and to each patch, depending on the patch connectivities
void WedgeDispersal::adults_disperse(std::vector<Patch*> &sites) {
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

// Computes the set of connection indices and weights for a group of patches based on the wedge connection assumptions
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> WedgeDispersal::compute_connecs(std::vector<Patch*> &sites) {
	 int num_sites = sites.size();
	std::vector<std::vector<double>> connec_weights(num_sites);
	std::vector<std::vector<int>> connec_indices(num_sites);
	// Compute inter-point distances
	std::vector<std::vector<double>> distances = compute_distances(sites);
	// Compute the smallest inter-point distance for each point; radius for each pt is half this
	std::vector<double> radii;
	for (const auto& row : distances) {
		double smallest_dist = std::numeric_limits<double>::infinity();
		for (double dist : row) {
			if (dist > 0 && dist < smallest_dist) {
				smallest_dist = dist;
			}
		}
		radii.push_back(0.5*smallest_dist);
	}

	std::vector<std::pair<double, double>> intervals; // Vector to store intervals
	std::pair<double, double> qq; // temporary interval
	double alpha, theta;
	Point loc1, loc2;
	for (int i=0; i < num_sites; i++) {
		intervals.clear();
		std::vector<double> row = distances[i];
		// Get the vector of positions in order of distance (need to computes connectivies from closest to farthest)
		std::vector<int> order = get_sorted_positions(row);
		loc1 = sites[i]->get_coords();
		for (int j=1; j < order.size(); j++) {
			double length = 0;
			int jj = order[j];
			loc2 = sites[jj]->get_coords();
			if (distances[i][jj] < max_disp) {
				alpha = std::atan(radii[jj] / distances[i][jj]); 
				if (loc2.y - loc1.y >= 0 && loc2.x - loc1.x >= 0) {
					theta = std::atan((loc2.y - loc1.y) / (loc2.x - loc1.x)); 
				}
				if (loc2.y - loc1.y >= 0 && loc2.x - loc1.x <= 0) { 
					theta = PI + std::atan((loc2.y - loc1.y) / (loc2.x - loc1.x)); 
				}
				if (loc2.y - loc1.y <= 0 && loc2.x - loc1.x <= 0) {
					theta = PI + std::atan((loc2.y - loc1.y) / (loc2.x - loc1.x)); 
				}
				if (loc2.y - loc1.y <= 0 && loc2.x - loc1.x >= 0) {
					theta = 2*PI + std::atan((loc2.y - loc1.y) / (loc2.x - loc1.x)); 
				}
				double t_min = wrap_around((theta - alpha) / TWOPI, 1);
				double t_plus = wrap_around((theta + alpha) / TWOPI, 1);
				if (t_min > t_plus) {
					qq = {t_min, 1};
					auto result = compute_interval_union(qq, intervals);
					intervals = result.first;
					length += result.second;
					qq = {0, t_plus};
					result = compute_interval_union(qq, intervals);
					intervals = result.first;
					length += result.second;
				}
				else { 
					qq = {t_min, t_plus};
					auto result = compute_interval_union(qq, intervals);
					intervals = result.first;
					length = result.second;
				}
				if (length > 0) {
					connec_weights[i].push_back(length);
					connec_indices[i].push_back(jj);
				}
			}
		}
	}
	return {connec_indices, connec_weights};
}

double WedgeDispersal::wrap_around(double value, double range)
{
	return std::fmod(std::fmod(value, range) + range, range);
}

std::pair<std::vector<std::pair<double, double>>, double> WedgeDispersal::compute_interval_union(const std::pair<double, double>& qq,
 const std::vector<std::pair<double, double>>& input)
{
	// Create a vector to store the union of intervals
	std::vector<std::pair<double, double>> output;

	// Merge overlapping intervals in the output vector
	std::pair<double, double> merged_interval = qq;
	for (const auto& interval : input) {
		if (interval.second < merged_interval.first || interval.first > merged_interval.second) {
			output.push_back(interval);
		}
		else {
			merged_interval.first = std::min(merged_interval.first, interval.first);
			merged_interval.second = std::max(merged_interval.second, interval.second);
		}
	}

	// Add the merged interval
	output.push_back(merged_interval);

	// Calculate the difference in the sum of lengths
	double sum_lengths = 0.0;
	for (const auto& interval : output) {
		sum_lengths += interval.second - interval.first;
	}
	double input_sum_lengths = 0.0;
	for (const auto& interval : input) {
		input_sum_lengths += interval.second - interval.first;
	}
	double diff = sum_lengths - input_sum_lengths;

	std::sort(output.begin(), output.end());
	return {output, diff};
}

// Retuns the indices of the elements in the vector, sorted by numeric value in ascending order
std::vector<int> WedgeDispersal::get_sorted_positions(const std::vector<double>& numbers) 
{
	// Create a vector of indices (0 to N-1)
	std::vector<int> indices(numbers.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Sort the indices based on the corresponding values in the vector
	std::sort(indices.begin(), indices.end(), [&numbers](int a, int b) {return numbers[a] < numbers[b];});

	return indices;
}

// Computes the inter-point distances for a list of points
std::vector<std::vector<double>> WedgeDispersal::compute_distances(const std::vector<Patch*>& sites) 
{
	std::vector<std::vector<double>> distances;
	for (int i=0; i < sites.size(); ++i) {
		std::vector<double> row;
		for (int j=0; j < sites.size(); ++j) {
			double dist = boundary_strategy->distance(sites[i]->get_coords(), sites[j]->get_coords());
			row.push_back(dist);
		}
		distances.push_back(row);
	}

	return distances;
}