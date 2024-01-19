#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Record.h"

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