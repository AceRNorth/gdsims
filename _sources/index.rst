.. GDSiMS documentation master file, created by
   sphinx-quickstart on Fri Aug  9 11:12:29 2024.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. toctree::
   :maxdepth: 2
   :hidden:

   install
   usage
   user_guide/user_guide_root
   tutorials/tutorials_root
   api/library_root

GDSiMS documentation
====================

Welcome to the documentation of GDSiMS: the Gene Drive Simulator of Mosquito Spread.

GDSiMS is a metapopulation simulation program modelling mosquito populations and their response to gene drive release. 

Features
--------
- Spatial modelling
- Specific to mosquito behaviour
- Complex behaviour simulations
- Simple, easy-to-use interface 
- Apply to real-life locations (in progress)

Usage
-----
To run the program simply run the executable file ``gdsimsapp`` from the command line. 
For Windows:

.. code-block:: bash

	gdsimsapp.exe

This will display a command-line interface with instructions to follow.
Output files will be generated in an ``output_files`` subdirectory. 

For those comfortable with C++ programming, a code-based option also exists.

More details on both options can be found in the :doc:`tutorials/tutorials_root` and :doc:`user_guide/user_guide_root`.

.. warning::

   The version of the program on GitHub is made to run tests, and therefore not fully randomised. If you'd like to fully randomise results, the following line of code at the top of ``random.cpp``, 

   .. code-block:: cpp
  
      twister(1)

   must be set to

   .. code-block:: cpp

      twister(rd())

   The CMake project must then be re-built and re-compiled (as in the :doc:`install` instructions).

   When doing this, pre-defined set results will not match saved test data. 


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
