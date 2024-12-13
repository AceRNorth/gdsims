
.. _program_listing_file_includes_Patch.h:

Program Listing for File Patch.h
================================

|exhale_lsh| :ref:`Return to documentation for file <file_includes_Patch.h>` (``includes\Patch.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_PATCH_H
   #define GENERALMETAPOP_PATCH_H
   
   #include <array>
   #include "Patch.h"
   #include "constants.h"
   #include "Model.h"
   #include "Params.h"
   #include "Point.h"
   
   class Model;
   
   class Patch {
   public:
       Patch(Model* mod, LifeParams* par, double a0, double side_x, double side_y); // *
       Patch(Model* mod, LifeParams* par, double a0, Point point);
       void populate(int initial_WJ, int initial_WM, int initial_WV, int initial_WF);
   
       Point get_coords() const;
       std::array<long long int, constants::num_gen> get_M() const;
       std::array<std::array<long long int, constants::num_gen>, constants::num_gen> get_F() const;
   
       long long int calculate_tot_J();
       long long int calculate_tot_M();
       long long int calculate_tot_V();
       long long int calculate_tot_F();
   
       // life-processes for the local site
   
       void juv_get_older();
       void adults_die();
       void virgins_mate();
       void lay_eggs(const std::array<std::array<std::array <double, constants::num_gen>, constants::num_gen>, constants::num_gen> &f,
        const std::array<double, constants::max_dev+1> &dev_duration_probs);
       void juv_eclose();
       void update_comp();
       void update_mate();
       
       // interface to Dispersal
   
       void M_disperse_out(const std::array<long long int, constants::num_gen> &m_out);
       void F_disperse_out(const std::array<std::array<long long int, constants::num_gen>, constants::num_gen> &f_out);
       void M_disperse_in(int gen, long long int m_in);
       void F_disperse_in(int f_gen, int m_gen, long long int f_disp);
   
       // interface to Aestivation
   
       void F_hide(const std::array<std::array<long long int, constants::num_gen>, constants::num_gen> &f_try);
       void F_wake(const std::array<std::array<long long int, constants::num_gen>, constants::num_gen> &f_wake);
   
       // interface to GDRelease
   
       void add_driver_M(int num_driver_M);
   
   private:
       Model* model; 
       LifeParams* params;  
       double alpha0; 
       Point coords; 
       std::array<std::array<long long int, constants::max_dev+1>, constants::num_gen> J; 
       std::array<long long int, constants::num_gen> M; 
       std::array<long long int, constants::num_gen> V; 
       std::array<std::array<long long int, constants::num_gen>, constants::num_gen> F; 
       long double comp; 
       long double mate_rate; 
   };
   
   #endif //GENERALMETAPOP_PATCH_H
