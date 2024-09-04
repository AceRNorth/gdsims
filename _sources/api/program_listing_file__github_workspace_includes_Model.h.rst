
.. _program_listing_file__github_workspace_includes_Model.h:

Program Listing for File Model.h
================================

|exhale_lsh| :ref:`Return to documentation for file <file__github_workspace_includes_Model.h>` (``/github/workspace/includes/Model.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_MODEL_H
   #define GENERALMETAPOP_MODEL_H
   
   #include <vector>
   #include <array>
   #include "constants.h"
   #include "Params.h"
   #include "Patch.h"
   #include "Dispersal.h"
   #include "Aestivation.h"
   #include "GDRelease.h"
   #include "Seasonality.h"
   #include "Point.h"
   
   class Patch;
   class Aestivation;
   class Dispersal;
   class GDRelease;
   class Seasonality;
   
   class Model {
   public:
       Model(ModelParams *params, const std::array<std::array<std::array <double, constants::num_gen>, constants::num_gen>, constants::num_gen> &inher_frac, SineRainfallParams *season, double a0_mean, double a0_var, std::vector<int> rel_sites = {}, BoundaryType boundary = BoundaryType::Toroid, DispersalType disp_type = DispersalType::DistanceKernel, std::vector<Point> coords = {});
       Model(ModelParams *params, const std::array<std::array<std::array <double, constants::num_gen>, constants::num_gen>, constants::num_gen> &inher_frac, InputRainfallParams *season, double a0_mean, double a0_var, std::vector<int> rel_sites = {}, BoundaryType boundary = BoundaryType::Toroid, DispersalType disp_type = DispersalType::DistanceKernel, std::vector<Point> coords = {});
       
       ~Model();
       void initiate();
       void run(int day);
   
       long long int calculate_tot_J(); 
       long long int calculate_tot_M();
       long long int calculate_tot_V();
       long long int calculate_tot_F();
       std::array<long long int, constants::num_gen> calculate_tot_M_gen();
       std::vector<Patch*> get_sites() const;
       int get_day() const;
       double get_alpha(double alpha0);
   
   private:
       std::vector<Patch*> sites; 
       Dispersal* dispersal;
       Aestivation* aestivation;
       GDRelease* gd_release;
       Seasonality* seasonality;
   
       int day_sim; 
       // simulation area parameters
       int num_pat; 
       double side; 
       InitialPopsParams *initial_pops; 
       // juvenile development parameters - common for all Patches
       int min_dev; 
       std::array<double, constants::max_dev+1> dev_duration_probs; 
       // inheritance
       std::array<std::array<std::array <double, constants::num_gen>, constants::num_gen>, constants::num_gen> inher_fraction; 
       // Patch construction parameters
       double alpha0_mean; 
       double alpha0_variance; 
       double alpha0();
   
       // initiation methods
       void populate_sites();
       void set_dev_duration_probs(int min_time, int max_time);
   
       // life-processes - interface with Patch
       void run_step(int day);
       void juv_get_older();
       void adults_die();
       void virgins_mate();
       void lay_eggs();
       void juv_eclose();
   };
   
   #endif //GENERALMETAPOP_MODEL_H
