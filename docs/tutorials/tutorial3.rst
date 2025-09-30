
3. Setting coordinates and release sites
========================================

In this tutorial we give bite-sized introductions to some of the essential components of the model: patches and gene drive. Feel free to skip these sections if you're familiar with these concepts. However, they do provide some of the assumptions specific to our model. We then cover how to set your own Patch coordinates and gene drive release sites, and how to visualise the release as an animation. 

3.1 Model introduction: Patch
-----------------------------

The :class:`Patch` component is the essential building block of the model. It represents a population of mosquitoes mating, aging, dying… carrying out all the essential behaviours of a living, breathing population. As we've seen, the mosquitoes are divided into four types of individuals: juveniles (J), adult males (M), adult unmated (virgin) females (V) and adult mated females (F). Juveniles do not have a specific sex in the model - they are turned into adult males and adult (unmated) females upon eclosion (more information in the ``juv_eclose()`` function documentation of the :class:`Patch` class).

.. figure:: ../images/tut3_patch_intro.png
    :align: left
    :scale: 90 %

These populations (or Patches) are assigned some coordinates and arranged on a 2D simulated area, where, following metapopulation ecology theory, they interact with each other through dispersal mechanisms.

.. _tutorial-3.2: 

3.2 Model introduction: Gene drive
----------------------------------

Gene drive is a real-life technology that enhances the inheritance rates of certain genetic traits. In the case of our model, gene drive is applied to the mosquitoes, rapidly selecting for traits that will eventually suppress the overall population numbers. 

To start the gene drive process, a number of modified mosquitoes are released into the wild populations. We would usually select a specific number of patches for it to be released into. The drive mosquitoes then mate with wild mosquitoes and carry out the same life processes as them. They will disperse too, which causes the natural spread of the drive throughout the simulation area. 

We need to expand on the Patch concept to track the gene drive and so, the four types of individuals in a Patch are subdivided into several genotypes - let's consider six genotypes as this is the current default. These are composed of three different alleles: wild-type (W), drive-type (D) and non-functional resistance-type (R). We thus have the following genotypes available: WW, WD, DD, DR, WR and RR, which we already plotted as part of our first run!

.. note:: 
    We count six genotypes and not nine because WD and DW genotypes are counted together, and likewise for the other heterozygous genotypes.


.. _tutorial-3.3:

3.3 Setting your own Patch coordinates
--------------------------------------

Now we can repeat our first run but, instead of randomly generating patch coordinates by default, we will set our own. To do this, we will need to select a custom run to be able to access the advanced options. 

Create a file that contains these parameters:

.. collapse:: Parameters

    .. code-block::
        :caption: docs/exercises/params_set101.txt

        1
        1000
        25
        0.05
        0.125
        100
        9
        0.0666666666666666
        10
        0.025
        0.2
        0.95
        50
        5000
        1
        0.01
        0.25
        0
        0
        0
        0
        0
        0
        100000
        0
        0
        0
        0
        0
        1000
        1
        1
        1
        101

 
And, as before, enter the filepath for the parameters file (this time ``params_set101.txt``) into the program. Remember these exercise files can be found in the ``docs/exercises`` directory and the filepath we use is relative to the ``build`` directory, so we would enter ``../docs/exercises/params_set101.txt`` for this tutorial. 

Now we can say yes to the advanced options and we'll be given a submenu of options. Let's choose ``4`` - the custom patch coordinates option. 

.. image:: ../images/tut3_adv_options.png
    :scale: 80 %

The interface will then prompt us to enter a coordinates filename. Let's have a quick look at what this file should look like before proceeding with the run.

The custom coordinates file should have three columns - the x and y coordinates plus a ``y`` / ``n`` character. The last character describes whether the coordinate is a gene drive release site or not (yes or no).

As in other parameter files, each row should be delimited by new lines and each value in a row by white space. 

The coordinate values may also be subject to certain bound checks - more information on this in the :doc:`../user_guide/adv_options` section of the User Guide. 

.. warning::
    Selecting release sites in this way will overwrite the number of release sites previously chosen with the ``num_driver_sites`` parameter. 


Let's use the coordinates file below to create a square grid of patches. 

.. collapse:: Coordinates
    
    .. code-block:: 
        :caption: docs/exercises/coords_square_25.txt

        0.0     0.0	    n
        0.25    0.0	    n
        0.5	    0.0	    n
        0.75	0.0	    n
        1.0	    0.0	    n
        0.0	    0.25	n
        0.25	0.25	n
        0.5	    0.25	n
        0.75	0.25	n
        1.0	    0.25	n
        0.0	    0.5	    n
        0.25	0.5	    n
        0.5	    0.5	    y
        0.75	0.5	    n
        1.0	    0.5	    n
        0.0	    0.75	n
        0.25	0.75	n
        0.5	    0.75	n
        0.75	0.75	n
        1.0	    0.75	n
        0.0	    1.0	    n
        0.25	1.0	    n
        0.5	    1.0	    n
        0.75	1.0	    n
        1.0	    1.0	    n

Now we can enter the filepath into the prompt. Like with the parameters file, we can use the relative filepath to the ``docs/exercises`` directory. If there are no error messages, this will set the coordinates (more information on errors in the :doc:`../user_guide/custom_set` and :doc:`../user_guide/adv_options` sections of the User Guide).

We can exit the advanced options and run the program by entering ``0``, which should start outputting day data as in previous runs. 

.. image:: ../images/tut3_set_coords.png

Finally, we could re-plot the coordinates and check the difference, but we will leave this as an exercise for the user. 


3.4 Creating a local data animation: drive allele frequency (optional - python)
-------------------------------------------------------------------------------

.. note:: 

   This tutorial will require the same installations as Tutorial :ref:`tutorial-1.2`.

We can also create an animation of the drive allele frequency to track the spread of the gene drive.

The drive allele (D) frequency is calculated relative to all available genotypes:

.. math:: 

    \text{drive allele freq.} = \frac{\mathrm{WD} + 2(\mathrm{DD}) + \mathrm{DR}}{2(\mathrm{TOT})}

with :math:`\textrm{TOT} = \textrm{WW} + \textrm{WD} + \textrm{DD} + \textrm{WR} + \textrm{RR} + \textrm{DR}`. Each genotype in the equation represents the total number of mated females of that genotype in the overall area.

To make this animation, you should run this script:

.. code-block:: python

    import gdsimsplotlib as gdp
    import os

    os.chdir(r"C:\Users\MyUser\Projects\gdsims\build\output_files")  # change the current directory to the output_files directory
    anim = gdp.animate_local_drive_allele_freq("LocalData101run1.txt", "CoordinateList101run1.txt", interval=20)  # assign return to a variable to ensure animation lives long enough to display

You'll likely want to change the frame interval on the animation to 20 ms (compared to the default of 500 ms) since we have a lot more recorded days to cycle through. It's also important to note that for animations the function return must be assigned to a variable for the animation object to live long enough to be displayed. Otherwise, you will get an error saying this. 

Our script should produce the following animation:

.. image:: ../images/tut3_drive_anim_coords.gif
    :scale: 90 %

The animation clearly shows the spread of the gene drive and the subsequent collapse of the populations - note how the drive mosquitoes are released into the left edge central patch and spread, not only towards the right, but also appear from the right edge of the plot! This is because of the toroidal boundary conditions the model uses by default - we'll get into more detail in the next tutorial.

.. tip::

    If you want to look at specific snapshots of the two animations we have discussed, you can use the sister plotting functions, ``plot_local_pop_size()`` and ``plot_local_drive_allele_freq()``, included in the library. Please read the in-function documentation, particularly around the timestep argument t, as this may be non-intuitive.