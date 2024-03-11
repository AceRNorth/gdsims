#include <limits>
#include <cmath>
#include <algorithm>
#include <numeric>
#include "ConnecStrategy.h"

const double PI = 3.14159265;
const double TWOPI = 2 * PI;

ConnecStrategy::ConnecStrategy(double md, BoundaryStrategy* boundary)
{
    max_disp = md;
    boundary_strategy = boundary;
}

// Computes the set of connection indices and weights for a group of patches based on the simple connection assumptions
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> SimpleConnecStrategy::compute_connecs(std::vector<Patch*>
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

// Computes the set of connection indices and weights for a group of patches based on the wedge connection assumptions
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> WedgeConnecStrategy::compute_connecs(std::vector<Patch*>
 &sites)
{
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

double WedgeConnecStrategy::wrap_around(double value, double range)
{
	return fmod(fmod(value, range) + range, range);
}

std::pair<std::vector<std::pair<double, double>>, double> WedgeConnecStrategy::compute_interval_union(const std::pair<double, double>& qq,
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
std::vector<int> WedgeConnecStrategy::get_sorted_positions(const std::vector<double>& numbers) 
{
	// Create a vector of indices (0 to N-1)
	std::vector<int> indices(numbers.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Sort the indices based on the corresponding values in the vector
	std::sort(indices.begin(), indices.end(), [&numbers](int a, int b) {return numbers[a] < numbers[b];});

	return indices;
}

// Computes the inter-point distances for a list of points
std::vector<std::vector<double>> WedgeConnecStrategy::compute_distances(const std::vector<Patch*>& sites) 
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