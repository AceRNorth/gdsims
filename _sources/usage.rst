
Usage
=====

The program can be used in two ways: through a command-line interface or interacting directly with the :class:`Simulation` class through C++ code. 

There also exists a more advanced option, where you can interact with the :class:`Model` and :class:`Record` classes directly, which may be added to the :doc:`user_guide/user_guide_root` in the future.

Command-line interface (CLI)
----------------------------

The CLI provides default sets of parameters to demonstrate specific behaviours, but also offers a custom parameter option. It provides instructions and prompts to set up the program run, making our program easy to use for non-programmers. 

.. image:: images/usage_CLI.png
    :scale: 80 %


Our :doc:`tutorials/tutorials_root` explain how to use the CLI in full detail. 

User-defined main file
----------------------

For those comfortable with C++ programming, a user-defined main file may be specified to interact with the :class:`Simulation` class directly.

The new main file can be substituted in ``CMakeLists.txt`` as the executable source file:

.. image:: images/usage_main3_cmakelists.PNG
    :scale: 60 %


To run the simulation with the default parameters of set 1, ``main3.cpp`` would look like this:

.. image:: images/usage_main3_file.png
    :scale: 80 %


The CMake project will need to be re-built and re-compiled after making these changes (as explained in the :doc:`install` section).

Step-by-step details and examples can be found in the :doc:`user_guide/code` section of the User Guide.

Output
======

Upon running a set, output files will be created in a new subdirectory called ``output_files``. This will contain:

- ``CoordinateList.txt``
- ``LocalData.txt``
- ``Totals.txt``

for each run of the set. Optional Python scripts are provided in the ``test`` directory for default visualisation options and running tests.

.. note::
    Before outputting any data, the :class:`Simulation` class will implicitly run a burn-in period of 1 year (365 days) at the start of each run. This is to make sure the simulated populations have reached a stable equilibrium before modelling any interventions. 
