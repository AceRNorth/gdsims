
.. _program_listing_file__github_workspace_includes_GDRelease.h:

Program Listing for File GDRelease.h
====================================

|exhale_lsh| :ref:`Return to documentation for file <file__github_workspace_includes_GDRelease.h>` (``/github/workspace/includes/GDRelease.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_GDRELEASE_H
   #define GENERALMETAPOP_GDRELEASE_H
   
   #include <vector>
   #include "Params.h"
   #include "Patch.h"
   
   class Patch;
   
   class GDRelease {
   public:
       GDRelease(int num_driver_M, std::vector<int> rel_times): num_driver_M(num_driver_M), release_times(rel_times) {}
       virtual ~GDRelease() {};
       void release_gene_drive(int day, std::vector<Patch*> &sites);
   protected:
       int num_driver_M;
       std::vector<int> release_times; 
       bool is_release_time(int day);
       virtual std::vector<Patch*> select_driver_sites(int day, const std::vector<Patch*> &sites) = 0;
       void put_driver_sites(std::vector<Patch*>& rel_sites);
   };
   
   class RandomGDRelease: public GDRelease {
   public:
       RandomGDRelease(ReleaseParams* params);
       ~RandomGDRelease() {};
   private:
       int num_driver_sites; 
       std::vector<Patch*> select_driver_sites(int day, const std::vector<Patch*> &sites) override;
   };
   
   class SchedGDRelease: public GDRelease {
   public:
       SchedGDRelease(ReleaseParams* params, std::vector<int> rel_sites, std::vector<Patch*> &sites);
       ~SchedGDRelease() {};
   private:
       std::vector<Patch*> release_sites; 
       std::vector<Patch*> select_driver_sites(int day, const std::vector<Patch*> &sites) override;
   };
   
   #endif //GENERALMETAPOP_GDRELEASE_H
