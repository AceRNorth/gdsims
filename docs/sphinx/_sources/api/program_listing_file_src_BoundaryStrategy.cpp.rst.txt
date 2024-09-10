
.. _program_listing_file_src_BoundaryStrategy.cpp:

Program Listing for File BoundaryStrategy.cpp
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file_src_BoundaryStrategy.cpp>` (``src\BoundaryStrategy.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <cmath>
   #include "BoundaryStrategy.h"
   
   double ToroidalBoundaryStrategy::distance(const Point &p1, const Point &p2)
   {
       double x_dist = 0;
       double y_dist = 0;
   
       if (std::abs(p1.x - p2.x) > side - std::abs(p1.x - p2.x)) {
           x_dist = side - std::abs(p1.x - p2.x);
       } 
       else {
           x_dist = std::abs(p1.x - p2.x);
       }
   
       if (std::abs(p1.y - p2.y) > side - std::abs(p1.y - p2.y)) {
           y_dist = side - std::abs(p1.y - p2.y);
       }
       else {
           y_dist = std::abs(p1.y - p2.y);
       }
   
       return std::sqrt((x_dist * x_dist) + (y_dist * y_dist));
   }
   
   Point ToroidalBoundaryStrategy::relative_pos(const Point &p1, const Point &p2) 
   {
       Point t2=p2;
       if (2*(p2.x - p1.x) > side) {
           t2.x = p2.x - side;
       }
       if (2*(p1.x - p2.x) > side) {
           t2.x = p2.x + side;
       }
       if (2*(p2.y - p1.y) > side) {
           t2.y = p2.y - side;
       }
       if (2*(p1.y - p2.y) > side) {
           t2.y = p2.y + side;
       }
       return t2;
   }
   
   double EdgeBoundaryStrategy::distance(const Point &p1, const Point &p2)
   {
       return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
   }
   
   Point EdgeBoundaryStrategy::relative_pos(const Point &p1, const Point &p2) 
   {
       return p2;
   }
