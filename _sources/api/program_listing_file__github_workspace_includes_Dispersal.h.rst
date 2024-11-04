
.. _program_listing_file__github_workspace_includes_Dispersal.h:

Program Listing for File Dispersal.h
====================================

|exhale_lsh| :ref:`Return to documentation for file <file__github_workspace_includes_Dispersal.h>` (``/github/workspace/includes/Dispersal.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_DISPERSAL_H
   #define GENERALMETAPOP_DISPERSAL_H
   
   #include <vector>
   #include <array>
   #include <utility>
   #include "constants.h"
   #include "Params.h"
   #include "Patch.h"
   #include "Point.h"
   #include "BoundaryStrategy.h"
   
   class Patch;
   
   class Dispersal {
   public:
       Dispersal(DispersalParams* params, BoundaryType boundary, double side);
       ~Dispersal();
       virtual void set_connecs(std::vector<Patch*> &sites) = 0;
       virtual void adults_disperse(std::vector<Patch*> &sites) = 0;
   
   protected:
       double disp_rate; 
       double max_disp; 
       std::vector<std::vector<int>> connec_indices; 
       // 
       std::vector<std::vector<double>> connec_weights; 
       BoundaryStrategy* boundary_strategy;
   
       std::vector<std::array<long long int, constants::num_gen>> M_dispersing_out(const std::vector<Patch*> &sites);
       std::vector<std::array<std::array<long long int, constants::num_gen>, constants::num_gen>> F_dispersing_out(const std::vector<Patch*> &sites);
   };
   
   class DistanceKernelDispersal: public Dispersal {
   public:
       DistanceKernelDispersal(DispersalParams* params, BoundaryType boundary, double side): Dispersal(params, boundary, side) {};
       void set_connecs(std::vector<Patch*> &sites) override;
       void adults_disperse(std::vector<Patch*> &sites) override;
   
   private:
       std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> compute_connecs(std::vector<Patch*> &sites);
   };
   
   class RadialDispersal: public Dispersal {
   public:
       RadialDispersal(DispersalParams* params, BoundaryType boundary, double side);
       void set_connecs(std::vector<Patch*> &sites) override;
       void adults_disperse(std::vector<Patch*> &sites) override;
   
   private:
       std::vector<double> connec_weights_sum; 
       std::pair<std::vector<std::vector<int>>, std::vector<std::vector<double>>> compute_connecs(std::vector<Patch*> &sites);
       std::pair<std::vector<std::pair<double, double>>, double> compute_interval_union(const std::pair<double, double>& qq,
           const std::vector<std::pair<double, double>>& input);
       double wrap_around(double value, double range); 
       std::vector<int> get_sorted_positions(const std::vector<double>& numbers);
       std::pair<std::vector<double>,std::vector<int>> compute_distances_site(int,std::vector<Patch*> &sites);
   };
   
   #endif //GENERALMETAPOP_DISPERSAL_H
