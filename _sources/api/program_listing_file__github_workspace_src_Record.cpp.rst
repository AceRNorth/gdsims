
.. _program_listing_file__github_workspace_src_Record.cpp:

Program Listing for File Record.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file__github_workspace_src_Record.cpp>` (``/github/workspace/src/Record.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <filesystem>
   #include <iostream>
   #include <sstream>
   #include <fstream>
   #include <vector>
   #include <iomanip>
   #include "Record.h"
   
   Record::Record(RecordParams* rec_params, int rep) 
   {
       rec_start = rec_params->rec_start;
       rec_end = rec_params->rec_end;
       rec_interval_global = rec_params->rec_interval_global;
       rec_interval_local = rec_params->rec_interval_local;
       rec_sites_freq = rec_params->rec_sites_freq;
       set_label = rec_params->set_label;
       rep_label = rep;
   
       next_local_day = rec_params->rec_start;
       next_global_day = 0;
   
       // create folder for destination of output files 
       if (!std::filesystem::exists("./output_files")) {
           std::filesystem::create_directory("output_files");
       }
       std::filesystem::current_path("./output_files");
       
       os1 << "LocalData" << set_label << "run" << rep_label << ".txt"; 
       local_data.open(os1.str());
       os2 << "Totals" << set_label << "run" << rep_label << ".txt";
       global_data.open(os2.str());
       os3 << "CoordinateList" << set_label << "run" << rep_label << ".txt";
       coord_list.open(os3.str());
   
       local_data << "Adult female populations of each genotype at each site\n";
       local_data << "Day" << "\t" << "Site" << "\t" << "WW" << "\t" << "WD" << "\t" << "DD" << "\t" << "WR" << "\t" << "RR" << "\t" << "DR" << std::endl;
   
       global_data << "Total adult females of each genotype\n";
       global_data << "Day" << "\t" << "WW" << "\t" << "WD" << "\t" << "DD" << "\t" << "WR" << "\t" << "RR" << "\t" << "DR" << std::endl;
   
       coord_list << "Coordinate list of the sites\n";
       coord_list << "Site" << "\t" << "x" << "\t" << "y" << std::endl;
   }
   
   Record::~Record()
   {
       std::filesystem::current_path("..");
   }
   
   void Record::record_coords(const std::vector<Patch*> &sites) 
   {
       const auto default_precision{std::cout.precision()};
       constexpr auto max_precision{std::numeric_limits<double>::digits10 + 1};
       for (int pat=0; pat < sites.size(); pat += rec_sites_freq) {
           auto coords = sites[pat]->get_coords();
           coord_list << pat+1 << "\t" << std::setprecision(max_precision) << coords.x << "\t" << coords.y << std::endl;
       }
       coord_list << std::setprecision(default_precision);
   }
   
   void Record::record_global(int day, const std::array<long long int, constants::num_gen> &tot_F_gen)
   {
       global_data << day;
       for (const auto& f_gen : tot_F_gen) {
           global_data << "\t" << f_gen;
       }
       global_data << std::endl;
       next_global_day += rec_interval_global;
   }
   
   void Record::output_totals(int day, long long int tot_J, long long int tot_M, long long int tot_V, long long int tot_F)
   {
       if (day == 0) {
           std::cout << "\n" << "rep " << rep_label << "\n";
           std::cout << "day" << "   " << "total J" << "   " << "total M" << "   " << "total V" << "   " << "total F" << "\n";
       }
       std::cout << day << "     " << tot_J << "   " << tot_M << "   " << tot_V << "   " << tot_F << std::endl;
   }
   
   void Record::record_local(int day, const std::vector<Patch*> &sites) 
   {
       for (int pat=0; pat < sites.size(); pat += rec_sites_freq) {
           local_data << day << "\t" << pat+1;
           for (const auto& fem_gen : sites[pat]->get_F_fem_gen()) {
               local_data << "\t" << fem_gen;
           }
           local_data << std::endl;
       }
       next_local_day += rec_interval_local;
   }
   
   bool Record::is_rec_global_time(int day)
   {
       return day == next_global_day;
   }
   
   bool Record::is_rec_local_time(int day) 
   {
       return day >= rec_start && day <= rec_end && day == next_local_day;
   }
