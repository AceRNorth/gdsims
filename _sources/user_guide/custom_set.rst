
Custom sets
===========

In addition to the pre-defined sets provided, GDSiMS features a custom parameter set section within the command-line interface (CLI) for more customised modelling work. The CLI will guide you through entering a file with parameters to setting more advanced options. You can follow our tutorial on this - Tutorial :doc:`../tutorials/tutorial2`.

Custom set runs are also available through direct interaction with the source code classes - more information in the :doc:`code` section.

.. _custom_params_file:

Parameter input file
--------------------

The CLI will first ask for a parameter file, where the parameters must be listed in the order given.

.. image:: ../images/tut2_custom.png

An example of this parameter file is given below, with added labels.

.. image:: ../images/tut2_params_set100.png
    :scale: 70 %

To input the file, the CLI will ask for a filename. This may be one of three:

- An absolute filepath, e.g. ``C:\Users\MyUser\gdsims\params_set100.txt`` for Windows, ``home/user/gdsims/Params/params_set100.txt`` for Linux
- A relative filepath, relative to the ``build`` directory, e.g. ``../params_set100.txt``
- A filename, if the file is within the ``build`` directory, e.g. ``params_set100.txt``

The expected directory structure of the repository is discussed in the :doc:`../install` section.

Upon entering a parameter file the CLI will carry out bound and interval checks on the parameters, among other checks. If there are any errors, another file (or the same one but corrected) will be required to proceed. 

The expected bounds on parameters can be found below.

+-------------------------+-------------+----------------------+
|  Parameter              | Number type | Range                | 
|                         |             |                      |
|                         |             |                      |
|                         |             |                      |
+=========================+=============+======================+
| ``num_runs``	          |   Integer   |   (0, inf)           |
+-------------------------+-------------+----------------------+
| ``max_t``               |   Integer   |   (0, inf)           |
+-------------------------+-------------+----------------------+
| ``num_pat``             |   Integer   |   (0, inf)           |
+-------------------------+-------------+----------------------+
| ``mu_j``                |  Continuous |    [0, 1)            | 
+-------------------------+-------------+----------------------+
| ``mu_a``                |  Continuous |    (0, 1)            | 
+-------------------------+-------------+----------------------+
| ``beta``                |  Continuous |   (0, inf)           |
+-------------------------+-------------+----------------------+
| ``theta``               |  Continuous |   (0, inf)           | 
+-------------------------+-------------+----------------------+
| ``comp_power``          |  Continuous |   (0, inf)           | 
+-------------------------+-------------+----------------------+
| ``min_dev``             |   Integer   |   (0, inf)           |  
+-------------------------+-------------+----------------------+
| ``gamma``               |  Continuous |    [0, 1]            |  
+-------------------------+-------------+----------------------+
| ``xi``                  |  Continuous |    [0, 1]            |
+-------------------------+-------------+----------------------+
| ``e``                   |  Continuous |       [0, 1]         |
+-------------------------+-------------+----------------------+
| ``driver_start``        |   Integer   |      [1, inf)        |   
+-------------------------+-------------+----------------------+
| ``num_driver_M``        |   Integer   |      [0, inf)        | 
+-------------------------+-------------+----------------------+
| ``num_driver_sites``    |   Integer   |      [0, inf)        |
+-------------------------+-------------+----------------------+
| ``disp_rate``           |  Continuous |       [0, 1]         | 
+-------------------------+-------------+----------------------+
| ``max_disp``            |  Continuous |    (0, ``side``]     |    
+-------------------------+-------------+----------------------+
| ``psi``                 |  Continuous |       [0, 1]         |
+-------------------------+-------------+----------------------+
| ``mu_aes``              |  Continuous |       [0, 1]         | 
+-------------------------+-------------+----------------------+
| ``t_hide1``             |   Integer   |      [1, 365]        |
+-------------------------+-------------+----------------------+
| ``t_hide2``             |   Integer   | [``t_hide1``, 365]   |
+-------------------------+-------------+----------------------+
| ``t_wake1``             |   Integer   |      [1, 365]        |
+-------------------------+-------------+----------------------+
| ``t_wake2``             |   Integer   | [``t_wake2``, 365]   |  
+-------------------------+-------------+----------------------+
| ``alpha0_mean``         |  Continuous |      (0, inf)        | 
+-------------------------+-------------+----------------------+
| ``alpha0_variance``     |  Continuous |      [0, inf)        |     
+-------------------------+-------------+----------------------+
| ``alpha1``              |  Continuous |      [0, inf)        | 
+-------------------------+-------------+----------------------+
| ``amp``                 |  Continuous |      [0, 1]          |
+-------------------------+-------------+----------------------+
| ``resp``                |  Continuous |      [0, inf)        |  
+-------------------------+-------------+----------------------+
| ``rec_start``           |   Integer   |      [0, inf)        | 
+-------------------------+-------------+----------------------+
| ``rec_end``             |   Integer   | [``rec_start``, inf) |  
+-------------------------+-------------+----------------------+
| ``rec_interval_global`` |   Integer   |      [1, inf)        |
+-------------------------+-------------+----------------------+
| ``rec_interval_local``  |   Integer   |      [1, inf)        |
+-------------------------+-------------+----------------------+
| ``rec_sites_freq``      |   Integer   |      [1, inf)        |   
+-------------------------+-------------+----------------------+
| ``set_label``           |   Integer   |      [0, inf)        |
+-------------------------+-------------+----------------------+

() and [] denote excluding and including the bound, respectively.

Advanced options
----------------

Custom set runs on the CLI provide a submenu to set advanced options.

.. image:: ../images/tut3_adv_options.png
    :scale: 80%

Multiple options can be set one after the other. If setting an option is unsuccessful (i.e. there are error messages), the option can be re-set by selecting it again.

.. caution:: 
    If you exit the advanced options menu and run the program with an unsuccessful setting option, this option will maintain its default behaviour. More information in the :doc:`defaults` section.

Equally, the advanced options can be re-set at any point for any other reasons, as long as this is prior to exiting the advanced options menu.  

.. note::
    For options requiring input files, once the advanced behaviour has been successfully set the default behaviour cannot be reset. Only new files can be provided for resets. If one of these resets fails, the program will default to the last successful attempt - if none were successful, the original default behaviour will be maintained. More information on this in the :doc:`adv_options` section.
