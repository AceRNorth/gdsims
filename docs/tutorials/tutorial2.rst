
2. Running a custom set
=======================

This tutorial will describe how to run the model with a custom set of parameters, take a closer look at some of the recording parameters and create an animation from the local output data.

2.1 Run with custom parameters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

We will now run the program with a set of custom parameters carefully selected to make an interesting animation. 

Run the executable and enter the custom option number ``100``. This will display the list of parameters needed.

You will need to create a text file with the parameters, listed in the same order as above. 
For this example, you can include the following parameters. You can copy these into a file in the ``build`` directory or we can use the relative filepath to the ``params_set100.txt`` file in the GitHub ``docs/exercises`` directory. Let's try the latter - we can directly use the files we cloned from the repository. 

.. figure:: ../images/tut2_params_set100.png
    :scale: 70 %
    :align: left
    
    docs/exercises/params_set100.txt

.. note::
    The green labels are illustrative and should not be included in the file. 
    The values should be delimited by new lines. 

.. collapse:: Parameters

    .. code-block::
        :caption: docs/exercises/params_set100.txt

        1
        1400
        50
        0.05
        0.125
        100
        9
        0.0666666666666666
        10
        0.025
        0.2
        0.95
        100
        1000
        1
        0.01
        0.2
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
        1400
        1
        100
        1
        100

Let's have a look at the recording parameters at the end:

- ``rec_start`` : the time (i.e. simulation day number) to start recording
- ``rec_end`` : the time to stop recording
- ``rec_interval_global`` : the number of days to skip between datapoints in the global file
- ``rec_interval_local`` : the same as above but for the local file
- ``rec_sites_freq`` : the number of sites to skip between datapoints on each recorded day. Applies to the local data and the coordinates files. e.g. if ``rec_sites_freq`` = 2 and ``num_pat`` = 10, only the datapoints from sites 1, 3, 5, 7 and 9 would be written into the local file each day.
- ``set_label`` : the number given to the output filenames, in the format ``Totals{set_label}run1.txt``. Our filenames will now end in “100” to distinguish them from our Tutorial 1 run.

``rec_end`` and ``rec_interval_local`` will be particularly useful in making the following animation. Notice how the ``rec_end`` we have chosen matches ``max_t`` so we can later display the full run.

We can now proceed with the program and enter the parameter filepath. We will use the relative filepath ``../docs/exercises/params_set100.txt``, which will be relative to the ``build`` directory. The first ``../`` means we are going up a directory from our current directory ``build``. 

The interface will then confirm that we are happy with the parameters read in and finally, we will be asked if we want to choose advanced options - we will look at these later, so we can say no for this run. 

.. image:: ../images/tut2_confirm_params.png

The program should now start running in the same way as the pre-defined set run.

.. _tutorial-2.2: 

2.2 Creating a local data animation: population size (optional - python)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. note:: 

   This tutorial will require the same installations as Tutorial :ref:`tutorial-1.2`.

Now that we have our output files for the custom run, we can do something even more exciting. We can combine the local data and the coordinates files to create an animation showing how the population sizes evolve over time and space. We can use the following script:

.. code-block:: python

    import gdsimsplotlib as gdp
    import os

    os.chdir(r"C:\Users\MyUser\Projects\gdsims\build\output_files")  # change the current directory to the output_files directory
    anim = gdp.animate_local_pop_size("LocalData100run1.txt", "CoordinateList100run1.txt")  # assign return to a variable to ensure animation lives long enough to display
    anim.save("set100_pop_anim.gif")  # save to file if needed

This should produce the following animation:

.. image:: ../images/tut2_pop_size_anim.gif
    :scale: 90 %

.. tip::
    Depending on the IDE you are using, animations may not be displayed correctly in inline plot settings, so make sure to change the graphics view settings. The ``anim.save()`` command should have also saved a copy of the animation as a .gif file in your current directory. Other methods that can be used with this matplotlib FuncAnimation object are documented in `matplotlib's docs <https://matplotlib.org/stable/api/_as_gen/matplotlib.animation.FuncAnimation.html#matplotlib.animation.FuncAnimation>`_.

We can see the animation flows well at 100-day jumps, but it could be re-run with a smaller ``rec_interval_local`` if we want smoother changes. We could also start recording at a later ``rec_start`` closer to the release of the gene drive, or even include less sites in the animation with ``rec_sites_freq`` so the plot looks less cluttered. 
