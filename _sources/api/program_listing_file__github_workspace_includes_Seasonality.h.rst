
.. _program_listing_file__github_workspace_includes_Seasonality.h:

Program Listing for File Seasonality.h
======================================

|exhale_lsh| :ref:`Return to documentation for file <file__github_workspace_includes_Seasonality.h>` (``/github/workspace/includes/Seasonality.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_SEASONALITY_H
   #define GENERALMETAPOP_SEASONALITY_H
   
   #include <vector>
   #include "Params.h"
   
   class Seasonality {
   public:
       Seasonality(double alpha1): alpha1(alpha1) {}
       ~Seasonality() {}
       virtual double alpha(int day, double alpha0) = 0;
   
   protected:
       double alpha1; 
   };
   
   class SineRainfall: public Seasonality {
   public:
       SineRainfall(SineRainfallParams *params);
       double alpha(int day, double alpha0) override;
   
   private:
       double amp; 
   };
   
   class InputRainfall: public Seasonality {
   public:
       InputRainfall(InputRainfallParams *params);
       double alpha(int day, double alpha0) override;
   
   private:
       double resp; 
       std::vector<double> rainfall; 
   };
   
   #endif //GENERALMETAPOP_SEASONALITY_H
