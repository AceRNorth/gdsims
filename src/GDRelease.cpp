#include <vector>
#include <algorithm>
#include "GDRelease.h"
#include "Patch.h"
#include "random.h"

GDRelease::GDRelease(ReleaseParams* params)
{
	num_driver_M = params->num_driver_M;
	num_driver_sites = params->num_driver_sites;
	release_times = params->release_times;
	release_sites.clear();
}

// Releases the gene drive mosquitoes into the simulation area
void GDRelease::release_gene_drive(std::vector<Patch*> &sites)
{
	int num_rel_sites = std::min(int(sites.size()), num_driver_sites);
	std::vector<Patch*> rel_sites = select_driver_sites(num_rel_sites, sites);
	put_driver_sites(rel_sites, sites);
	release_sites.push_back(rel_sites); // store release sites for recording purposes
}

bool GDRelease::is_release_time(int day) 
{
	return (std::find(release_times.begin(), release_times.end(), day) == release_times.end()) ? false : true;
}

// Selects randomly and returns the release sites for the gene drive
std::vector<Patch*> GDRelease::select_driver_sites(int num_rel_sites, const std::vector<Patch*> &sites) 
{
	std::vector<Patch*> rel_patches; // patches in which to release the gene drive
	while (rel_patches.size() < num_rel_sites) {
		int rel_pat = random_discrete(0, sites.size() - 1);

		// only pick unique sites within the central area to release the gene drive
		auto is_unique = (rel_patches.end() == std::find(rel_patches.begin(), rel_patches.end(), sites[rel_pat]));
		if (is_unique) { 
			rel_patches.push_back(sites[rel_pat]);
		}
	}
	return rel_patches;
}

// Adds drive heterozygous (WD) male mosquitoes to the release sites
void GDRelease::put_driver_sites(std::vector<Patch*>& rel_sites, std::vector<Patch*> &sites)
{
	for (const auto& rel_pat : rel_sites) {
		rel_pat->add_driver_M(num_driver_M);
	}
}