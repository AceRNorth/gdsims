
.. _program_listing_file__github_workspace_includes_Simulation.h:

Program Listing for File Simulation.h
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file__github_workspace_includes_Simulation.h>` (``/github/workspace/includes/Simulation.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_SIMULATION_H
   #define GENERALMETAPOP_SIMULATION_H
   
   #include <array>
   #include <vector>
   #include <string>
   #include "constants.h"
   #include "Params.h"
   #include "InputParams.h"
   #include "Point.h"
   
   class Simulation {
   public:
       Simulation(InputParams input);
       ~Simulation();
       void set_coords(const std::string& filename);
       void set_boundary_type(BoundaryType boundary);
       void set_dispersal_type(DispersalType disp);
       void set_rainfall(const std::string& filename);
       void set_release_times(const std::string& filename);
       void set_inheritance(InheritanceParams inher_params); 
       void run_reps();
   
   private:
       int num_runs; 
       int max_t; 
       ModelParams *model_params; 
       RecordParams *rec_params; 
       SineRainfallParams *sine_rainfall_params; 
       InputRainfallParams *input_rainfall_params; 
       double alpha0_mean; 
       double alpha0_variance; 
       // additional parameter options
       std::vector<Point> sites_coords; 
       std::vector<int> release_sites; 
       BoundaryType boundary_type;
       DispersalType disp_type;
   
       // inheritance
       std::array<std::array<std::array <double, constants::num_gen>, constants::num_gen>, constants::num_gen> inher_fraction; 
   };
   
   #endif //GENERALMETAPOP_SIMULATION_H
