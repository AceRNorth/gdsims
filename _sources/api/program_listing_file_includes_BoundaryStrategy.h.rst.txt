
.. _program_listing_file_includes_BoundaryStrategy.h:

Program Listing for File BoundaryStrategy.h
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_includes_BoundaryStrategy.h>` (``includes\BoundaryStrategy.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #ifndef GENERALMETAPOP_BOUNDARYSTRATEGY_H
   #define GENERALMETAPOP_BOUNDARYSTRATEGY_H
   
   #include "Point.h"
   
   class BoundaryStrategy {
   public:
       BoundaryStrategy(double side): side(side) {};
       virtual double distance(const Point& p1, const Point& p2) = 0;
   
   protected:
       double side; 
   };
   
   class ToroidalBoundaryStrategy: public BoundaryStrategy {
   public:
       ToroidalBoundaryStrategy(double side): BoundaryStrategy(side) {};
       double distance(const Point &p1, const Point &p2) override;
   };
   
   class EdgeBoundaryStrategy: public BoundaryStrategy {
   public:
       EdgeBoundaryStrategy(double side): BoundaryStrategy(side) {};
       double distance(const Point& p1, const Point& p2) override;
   };
   
   #endif //GENERALMETAPOP_BOUNDARYSTRATEGY_H
